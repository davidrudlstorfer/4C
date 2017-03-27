/*-----------------------------------------------------------*/
/*!
\file str_model_evaluator_beaminteraction.cpp

\brief Evaluation of all beam interaction terms, managing
       everything that has to do with parallelity

\maintainer Jonas Eichinger

\level 3
*/
/*-----------------------------------------------------------*/

#include "str_model_evaluator_beaminteraction.H"

#include "../drt_structure_new/str_timint_base.H"
#include "../drt_structure_new/str_utils.H"
#include "../drt_lib/drt_utils_createdis.H"
#include "../drt_lib/drt_globalproblem.H"
#include "../drt_io/io.H"
#include "../drt_io/io_pstream.H"
#include "../linalg/linalg_utils.H"
#include "../linalg/linalg_serialdensematrix.H"
#include "../linalg/linalg_serialdensevector.H"
#include "../drt_inpar/inpar_beamcontact.H"

#include "../drt_fsi/fsi_matrixtransform.H"
#include "../drt_adapter/adapter_coupling.H"

#include "../drt_particle/particle_handler.H"
#include "../drt_beam3/beam3_base.H"

#include <Teuchos_TimeMonitor.hpp>
#include <Epetra_FEVector.h>

#include "../drt_beaminteraction/beaminteraction_submodel_evaluator_beamcontact.H"
#include "../drt_beaminteraction/beaminteraction_submodel_evaluator_crosslinking.H"
#include "../drt_beaminteraction/beaminteraction_submodel_evaluator_factory.H"
#include "../drt_beaminteraction/beaminteraction_submodel_evaluator_generic.H"
#include "../drt_beaminteraction/crosslinker_node.H"
#include "../drt_beaminteraction/periodic_boundingbox.H"
#include "../drt_beaminteraction/str_model_evaluator_beaminteraction_datastate.H"
#include "beaminteraction_calc_utils.H"

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
STR::MODELEVALUATOR::BeamInteraction::BeamInteraction() :
    discret_ptr_ ( Teuchos::null ),
    submodeltypes_ ( Teuchos::null ),
    me_map_ptr_ ( Teuchos::null ),
    me_vec_ptr_ ( Teuchos::null ),
    myrank_ ( -1 ),
    coupsia_ ( Teuchos::null ),
    siatransform_ ( Teuchos::null ),
    ia_discret_ ( Teuchos::null ),
    eletypeextractor_( Teuchos::null ),
    ia_state_ptr_ ( Teuchos::null ),
    ia_force_beaminteraction_ ( Teuchos::null ),
    force_beaminteraction_ ( Teuchos::null ),
    stiff_beaminteraction_ ( Teuchos::null ),
    particlehandler_ ( Teuchos::null ),
    binstrategy_ ( Teuchos::null ),
    bindis_ ( Teuchos::null ),
    rowbins_ ( Teuchos::null )
{
  // empty
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::Setup()
{
  CheckInit();

  // -------------------------------------------------------------------------
  // setup variables
  // -------------------------------------------------------------------------
  // discretization pointer
  discret_ptr_ = Teuchos::rcp_dynamic_cast<DRT::Discretization>( DiscretPtr(), true );
  // stiff
  stiff_beaminteraction_ = Teuchos::rcp( new
      LINALG::SparseMatrix( *GState().DofRowMapView(), 81, true, true ) );
  // force
  force_beaminteraction_ = Teuchos::rcp( new Epetra_Vector( *GState().DofRowMap(),true ) );
  // get myrank
  myrank_ = DiscretPtr()->Comm().MyPID();

  // print logo
  Logo();

  // set submodel types
  SetSubModelTypes();

  // -------------------------------------------------------------------------
  // clone problem discretization, the idea is simple: we redistribute only
  // the new discretization to enable all interactions (including the required
  // search), calculate the resulting force and stiffness contributions, export
  // them no our initial discretization where all evaluation, assembly and
  // solving is done. Therefore the maps of our initial discretization don't
  // change, i.e. there is no need to rebuild the global state.
  // -------------------------------------------------------------------------
  Teuchos::RCP<DRT::UTILS::DiscretizationCreatorBase>  discloner =
      Teuchos::rcp(new DRT::UTILS::DiscretizationCreatorBase());
  ia_discret_ = discloner->CreateMatchingDiscretization( discret_ptr_, "ia_structure", true, false, true );
  // create discretization writer
  ia_discret_->SetWriter( Teuchos::rcp( new IO::DiscretizationWriter( ia_discret_ ) ) );

  // init data container
  ia_state_ptr_ =  Teuchos::rcp(new STR::MODELEVALUATOR::BeamInteractionDataState() );
  ia_state_ptr_->Init();
  ia_state_ptr_->Setup( ia_discret_ );

  ia_state_ptr_->GetMutableDisNp() = Teuchos::rcp( new Epetra_Vector(
      *GStatePtr()->GetMutableDisNp() ) );

  // -------------------------------------------------------------------------
  // initialize coupling adapter to transform matrices between the two discrets
  // (with distinct parallel distribution)
  // -------------------------------------------------------------------------
  coupsia_ = Teuchos::rcp( new ADAPTER::Coupling() );
  siatransform_ = Teuchos::rcp( new FSI::UTILS::MatrixRowTransform );

  // -------------------------------------------------------------------------
  // initialize and setup binning strategy and particle handler
  // -------------------------------------------------------------------------
  Teuchos::RCP<Epetra_Comm> com = Teuchos::rcp(DiscretPtr()->Comm().Clone());
  bindis_ = Teuchos::rcp(new DRT::Discretization("particle" ,com));
  // create discretization writer
  bindis_->SetWriter( Teuchos::rcp(new IO::DiscretizationWriter(bindis_) ) );
  bindis_->FillComplete( false, false, false );

  // construct, init and setup particle handler and binning strategy
  particlehandler_ = Teuchos::rcp( new PARTICLE::ParticleHandler( myrank_ ) );
  particlehandler_->BinStrategy()->Init( bindis_, ia_discret_, ia_state_ptr_->GetMutableDisNp() );
  particlehandler_->BinStrategy()->Setup();
  binstrategy_ = particlehandler_->BinStrategy();

  // some screen output for binning
  PrintBinningInfoToScreen();

  // extract map for each eletype that is in discretization
  eletypeextractor_ = Teuchos::rcp( new BEAMINTERACTION::UTILS::MapExtractor );
  BEAMINTERACTION::UTILS::SetupEleTypeMapExtractor( ia_discret_, eletypeextractor_ );

  // initialize and setup submodel evaluators
  InitAndSetupSubModeEvaluators();

  // distribute problem according to bin distribution to procs ( in case of restart
  // partitioning is done during ReadRestart() )
  if( not DRT::Problem::Instance()->Restart() )
    PartitionProblem();

  // post setup submodel loop
  for ( Vector::iterator sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->PostSetup();

  issetup_ = true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::SetSubModelTypes()
{
  CheckInit();

  submodeltypes_ = Teuchos::rcp( new std::set< enum INPAR::BEAMINTERACTION::SubModelType >() );

  // ---------------------------------------------------------------------------
  // check for crosslinking in biopolymer networks
  // ---------------------------------------------------------------------------
  if ( DRT::INPUT::IntegralValue<int>( DRT::Problem::Instance()->BeamInteractionParams().sublist(
      "CONTRACTILE CELLS"), "CONTRACTILECELLS") )
    submodeltypes_->insert(INPAR::BEAMINTERACTION::submodel_contractilecells);

  // ---------------------------------------------------------------------------
  // check for crosslinking in biopolymer networks
  // ---------------------------------------------------------------------------
  if ( DRT::INPUT::IntegralValue<int>( DRT::Problem::Instance()->BeamInteractionParams().sublist(
      "CROSSLINKING"), "CROSSLINKER") )
    submodeltypes_->insert(INPAR::BEAMINTERACTION::submodel_crosslinking);

  // ---------------------------------------------------------------------------
  // check for beam contact
  // ---------------------------------------------------------------------------
  if ( DRT::INPUT::IntegralValue<INPAR::BEAMINTERACTION::Strategy>(
       DRT::Problem::Instance()->BeamInteractionParams().sublist("BEAM TO BEAM CONTACT"), "STRATEGY") != INPAR::BEAMINTERACTION::bstr_none or
       DRT::INPUT::IntegralValue<INPAR::BEAMINTERACTION::Strategy>(
       DRT::Problem::Instance()->BeamInteractionParams().sublist("BEAM TO SPHERE CONTACT"), "STRATEGY") != INPAR::BEAMINTERACTION::bstr_none or
       DRT::INPUT::IntegralValue<INPAR::BEAMINTERACTION::Strategy>(
       DRT::Problem::Instance()->BeamInteractionParams().sublist("BEAM TO SOLID CONTACT"), "STRATEGY") != INPAR::BEAMINTERACTION::bstr_none
     )
    submodeltypes_->insert(INPAR::BEAMINTERACTION::submodel_beamcontact);

  // ---------------------------------------------------------------------------
  // check for beam potential-based interactions
  // ---------------------------------------------------------------------------
  std::vector<DRT::Condition*> beampotconditions(0);
  Discret().GetCondition( "BeamPotentialLineCharge", beampotconditions );
  if ( beampotconditions.size() > 0 )
    submodeltypes_->insert( INPAR::BEAMINTERACTION::submodel_potential );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::InitAndSetupSubModeEvaluators()
{
  CheckInit();

  // model map
  me_map_ptr_ =
      BEAMINTERACTION::SUBMODELEVALUATOR::BuildModelEvaluators( *submodeltypes_ );
  std::vector< enum INPAR::BEAMINTERACTION::SubModelType > sorted_submodeltypes(0);

  // build and sort submodel vector
  me_vec_ptr_ = TransformToVector( *me_map_ptr_, sorted_submodeltypes );

  Vector::iterator sme_iter;
  for ( sme_iter = (*me_vec_ptr_).begin(); sme_iter != (*me_vec_ptr_).end(); ++sme_iter )
  {
    (*sme_iter)->Init( ia_discret_, bindis_, GStatePtr(), ia_state_ptr_, particlehandler_,
        TimInt().GetDataSDynPtr()->GetPeriodicBoundingBox(),
        Teuchos::rcp_dynamic_cast<BEAMINTERACTION::UTILS::MapExtractor>( eletypeextractor_, true ) );
    (*sme_iter)->Setup();
  }

  // submodels build their pointer to other submodel objects to enable submodel dependencies
  // this is not particularly nice, at least the nicest way to handle such dependencies
  Vector::const_iterator iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->InitSubmodelDependencies( me_map_ptr_ );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP< STR::MODELEVALUATOR::BeamInteraction::Vector >
    STR::MODELEVALUATOR::BeamInteraction::TransformToVector(
        STR::MODELEVALUATOR::BeamInteraction::Map submodel_map,
        std::vector<INPAR::BEAMINTERACTION::SubModelType>& sorted_submodel_types ) const
{
  Teuchos::RCP< STR::MODELEVALUATOR::BeamInteraction::Vector > me_vec_ptr =
      Teuchos::rcp( new STR::MODELEVALUATOR::BeamInteraction::Vector(0) );

  STR::MODELEVALUATOR::BeamInteraction::Map::iterator miter;

  // if there is a contractile cell submodel, put in in first place
  miter = submodel_map.find( INPAR::BEAMINTERACTION::submodel_contractilecells  );
  if ( miter != submodel_map.end() )
  {
    // put it in first place
    me_vec_ptr->push_back( miter->second );
    sorted_submodel_types.push_back( miter->first );
    submodel_map.erase( miter );
  }

  // insert the remaining model evaluators into the model vector
  for ( miter = submodel_map.begin(); miter != submodel_map.end(); ++miter )
  {
    me_vec_ptr->push_back(miter->second);
    sorted_submodel_types.push_back( miter->first );
  }

  return me_vec_ptr;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::HaveSubModelType(
    INPAR::BEAMINTERACTION::SubModelType const& submodeltype) const
{
  CheckInit();
  return ( submodeltypes_->find(submodeltype) != submodeltypes_->end() );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::PartitionProblem()
{
  CheckInit();

  // store structure discretization in vector
  std::vector< Teuchos::RCP< DRT::Discretization > > discret_vec( 1, ia_discret_);

  // displacement vector according to periodic boundary conditions
  std::vector< Teuchos::RCP<Epetra_Vector> > disnp( 1, Teuchos::rcp(new Epetra_Vector( *ia_discret_->DofColMap() ) ));
  LINALG::Export( *ia_state_ptr_->GetMutableDisNp(), *disnp[0] );

  // nodes, that are owned by a proc, are distributed to the bins of this proc
  std::vector< std::map< int, std::vector<int> > > nodesinbin(1);

  // weight for load balancing regarding the distribution of bins to procs
  // (this is experimental, choose what gives you best results)
  double const weight = 1.0;
  // get optimal row distribution of bins to procs
  rowbins_ = binstrategy_->WeightedDistributionOfBinsToProcs(
      discret_vec, disnp, nodesinbin, weight );

  // extract noderowmap because it will be called Reset() after adding elements
  Teuchos::RCP<Epetra_Map> noderowmap = Teuchos::rcp( new Epetra_Map( *bindis_->NodeRowMap() ) );
  // delete old bins ( in case you partition during your simulation or after a restart)
  bindis_->DeleteElements();
  binstrategy_->FillBinsIntoBinDiscretization( rowbins_ );

  // now node (=crosslinker) to bin (=element) relation needs to be
  // established in binning discretization. Therefore some nodes need to
  // change their owner according to the bins owner they reside in
  if( HaveSubModelType( INPAR::BEAMINTERACTION::submodel_crosslinking ) )
    particlehandler_->DistributeParticlesToBins( noderowmap );

  // determine boundary bins (physical boundary as well as boundary to other procs)
  binstrategy_->DetermineBoundaryRowBins();

  // determine one layer ghosting around boundary bins determined in previous step
  binstrategy_->DetermineBoundaryColBinsIds();

  // standard ghosting (if a proc owns a part of nodes (and therefore dofs) of
  // an element, the element and the rest of its nodes and dofs are ghosted
  Teuchos::RCP<Epetra_Map> stdelecolmap;
  Teuchos::RCP<Epetra_Map> stdnodecolmapdummy;
  binstrategy_->StandardDiscretizationGhosting( ia_discret_,
      rowbins_, ia_state_ptr_->GetMutableDisNp(), stdelecolmap, stdnodecolmapdummy );

  // distribute elements that can be cut by the periodic boundary to bins
  Teuchos::RCP<Epetra_Vector> iadiscolnp =
      Teuchos::rcp( new Epetra_Vector( *ia_discret_->DofColMap() ) );
  LINALG::Export( *ia_state_ptr_->GetMutableDisNp(), *iadiscolnp );

  binstrategy_->DistributeRowElementsToBinsUsingEleXAABB(
      ia_discret_, ia_state_ptr_->GetMutableBinToRowEleMap(), iadiscolnp );

  // build row elements to bin map
  BuildRowEleToBinMap();

  // extend ghosting
  std::map< int, std::set< int > > extbintoelemap;
  ExtendGhosting(extbintoelemap);

  // assign Elements to bins
  binstrategy_->RemoveAllElesFromBins();
  binstrategy_->AssignElesToBins( ia_discret_, extbintoelemap );

  // update maps of state vectors and matrices
  UpdateMaps();

  // reset transformation
  UpdateCouplingAdapterAndMatrixTransformation();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::ExtendGhosting(
    std::map< int, std::set< int > >& extbintoelemap)
{
  TEUCHOS_FUNC_TIME_MONITOR("STR::MODELEVALUATOR::BeamInteraction::ExtendGhosting");

  CheckInit();

  std::set<int> colbins;

  // first, add default one layer ghosting
  std::set<int> const& boundcolbins = binstrategy_->BoundaryColBinsIds();
  colbins.insert( boundcolbins.begin(), boundcolbins.end() );

  // if the bounding box of a row element of myrank touches a boundary col bin, we need
  // to ghost its neighborhood as well
  std::map< int, std::set< int > >::const_iterator it;
  std::vector<int> binvec(27);
  for( it = ia_state_ptr_->GetBinToRowEleMap().begin(); it != ia_state_ptr_->GetBinToRowEleMap().end() ; ++it )
  {
    // not the following if is only valid if you ensure that the largest element
    // in the discretization (in deformed state) is smaller than the cutoff
    // which is not necessarily needed e.g. for beam contact
//    if( boundcolbins.find( it->first ) != boundcolbins.end() )
    {
      binstrategy_->GetNeighborAndOwnBinIds( it->first, binvec );
      colbins.insert( binvec.begin(), binvec.end() );
      binvec.clear();
    }
  }

  // enable submodel specific ghosting contributions to bin col map
  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->AddBinsToBinColMap( colbins );

  // 1) extend ghosting of bin discretization
  // todo: think about if you really need to assign degrees of freedom for crosslinker
  // (now only needed in case you want to write output)
  binstrategy_->ExtendBinGhosting( rowbins_, colbins , true );

  // add submodel specific bins whose content should be ghosted in problem discret
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->AddBinsWithRelevantContentForIaDiscretColMap( colbins );

  // build auxiliary bin col map
  std::vector<int> auxgids( colbins.begin(), colbins.end() );
  Teuchos::RCP<Epetra_Map> auxmap = Teuchos::rcp( new Epetra_Map( -1,
      static_cast<int>( auxgids.size() ), &auxgids[0], 0, bindis_->Comm() ) );

  Teuchos::RCP<Epetra_Map> ia_elecolmap =
  binstrategy_->ExtendGhosting( ia_state_ptr_->GetMutableBinToRowEleMap(),
      extbintoelemap, auxmap );

  // 2) extend ghosting of discretization
  binstrategy_->ExtendDiscretizationGhosting( ia_discret_, ia_elecolmap , true );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::Reset(const Epetra_Vector& x)
{
  CheckInitSetup();

  // get current displacement state and export to interaction discretization dofmap
  UpdateDofMapOfVector( ia_discret_, ia_state_ptr_->GetMutableDisNp(), GState().GetMutableDisNp() );
  // update colume vector
  ia_state_ptr_->GetMutableDisColNp() = Teuchos::rcp( new Epetra_Vector( *ia_discret_->DofColMap() ) );
  LINALG::Export(*ia_state_ptr_->GetDisNp(), *ia_state_ptr_->GetMutableDisColNp());

  // submodel loop
  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end() ; ++sme_iter )
    (*sme_iter)->Reset();

  // Zero out force and stiffness contributions
  force_beaminteraction_->PutScalar(0.0);
  ia_force_beaminteraction_->PutScalar(0.0);
  ia_state_ptr_->GetMutableForceNp()->PutScalar(0.0);
  stiff_beaminteraction_->Zero();
  ia_state_ptr_->GetMutableStiff()->Zero();

  // update gidmap_ and exporter in matrix transform object
  // Note: we need this in every evaluation call (i.e. every iteration) because a change
  //       in the active set of element pairs changes the entries of the used coarse
  //       system stiffness matrix (because we only assemble non-zero values).
  //       Therefore, the graph of the matrix changes and also the required gidmap
  //       (even in computation with one processor)
  UpdateCouplingAdapterAndMatrixTransformation();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::EvaluateForce()
{
  CheckInitSetup();

  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->EvaluateForce();

  // do communication
  if( ia_state_ptr_->GetMutableForceNp()->GlobalAssemble( Add, false ) != 0 )
    dserror("GlobalAssemble failed");
  // add to non fe vector
  if ( ia_force_beaminteraction_->Update( 1., *ia_state_ptr_->GetMutableForceNp(), 1. ) )
    dserror("update went wrong");

  // transformation from ia_discret to problem discret
  TransformForce();

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::EvaluateStiff()
{
  CheckInitSetup();

  ia_state_ptr_->GetMutableStiff()->UnComplete();

  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->EvaluateStiff();

  if ( not ia_state_ptr_->GetMutableStiff()->Filled() )
    ia_state_ptr_->GetMutableStiff()->Complete();

  TransformStiff();

  if ( not stiff_beaminteraction_->Filled() )
    stiff_beaminteraction_->Complete();

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::EvaluateForceStiff()
{
  CheckInitSetup();

  ia_state_ptr_->GetMutableStiff()->UnComplete();

  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->EvaluateForceStiff();

  // do communication
  if( ia_state_ptr_->GetMutableForceNp()->GlobalAssemble( Add, false ) != 0 )
    dserror("GlobalAssemble failed");
  // add to non fe vector
  if ( ia_force_beaminteraction_->Update( 1., *ia_state_ptr_->GetMutableForceNp(), 1. ) )
    dserror("update went wrong");

  if ( not ia_state_ptr_->GetMutableStiff()->Filled() )
    ia_state_ptr_->GetMutableStiff()->Complete();

  TransformForceStiff();

  if ( not stiff_beaminteraction_->Filled() )
    stiff_beaminteraction_->Complete();

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::AssembleForce(Epetra_Vector& f,
    const double & timefac_np) const
{
  CheckInitSetup();

  LINALG::AssembleMyVector( 1.0, f, timefac_np, *force_beaminteraction_ );

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool STR::MODELEVALUATOR::BeamInteraction::AssembleJacobian(
    LINALG::SparseOperator& jac,
    const double & timefac_np) const
{
  CheckInitSetup();

  Teuchos::RCP<LINALG::SparseMatrix> jac_dd_ptr = GState().ExtractDisplBlock(jac);
  jac_dd_ptr->Add( *stiff_beaminteraction_, false, timefac_np, 1.0 );

  // no need to keep it
  stiff_beaminteraction_->Zero();
  ia_state_ptr_->GetMutableStiff()->Zero();

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::WriteRestart(
    IO::DiscretizationWriter& iowriter,
    const bool& forced_writerestart ) const
{
  CheckInitSetup();

  // write (restart) output
  OutputStepState();

  // sub model loop
  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->WriteRestart( iowriter, forced_writerestart );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::ReadRestart(
    IO::DiscretizationReader& ioreader )
{
  CheckInitSetup();

  // read interaction discretization
  IO::DiscretizationReader reader( ia_discret_, GState().GetStepN() );
  reader.ReadHistoryData( GState().GetStepN() );
  PartitionProblem();

  // sub model loop
  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->ReadRestart( ioreader );

  // rebuild binning, redistribute problem, build ghosting, assign elements
  PartitionProblem();

  // sub model loop
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->PostReadRestart();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::RecoverState(
    const Epetra_Vector& xold,
    const Epetra_Vector& dir,
    const Epetra_Vector& xnew)
{
 // empty
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::UpdateStepState(
    const double& timefac_n)
{
  CheckInitSetup();

  // add the old time factor scaled contributions to the residual
  Teuchos::RCP<Epetra_Vector>& fstructold_ptr = GState().GetMutableFstructureOld();

  fstructold_ptr->Update(timefac_n, *force_beaminteraction_, 1.0 );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::UpdateStepElement()
{
  CheckInitSetup();

  // submodel loop
  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->PreUpdateStepElement();

  binstrategy_->TransferNodesAndElements( ia_discret_, ia_state_ptr_->GetMutableDisColNp(),
      ia_state_ptr_->GetMutableBinToRowEleMap() );

  BuildRowEleToBinMap();

  // extend ghosting
  std::map< int, std::set< int > > extbintoelemap;
  ExtendGhosting(extbintoelemap);

  // assign Elements to bins
  binstrategy_->RemoveAllElesFromBins();
  binstrategy_->AssignElesToBins( ia_discret_, extbintoelemap );

  // update maps of state vectors and matrices
  UpdateMaps();

  // submodel loop update
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->UpdateStepElement();

  // sumodel post update
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->PostUpdateStepElement();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::DetermineStressStrain()
{
  // empty
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::DetermineEnergy()
{
  CheckInitSetup();
  dserror("Not yet implemented!");
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::OutputStepState(
    IO::DiscretizationWriter& iowriter) const
{
  CheckInitSetup();

  Vector::iterator sme_iter;
  for ( sme_iter = me_vec_ptr_->begin(); sme_iter != me_vec_ptr_->end(); ++sme_iter )
    (*sme_iter)->OutputStepState(iowriter);

  OutputStepState();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::OutputStepState() const
{
  CheckInitSetup();

  int const stepn = GState().GetStepN();
  double const timen = GState().GetTimeN();

  // write output of ia_discret
  Teuchos::RCP< IO::DiscretizationWriter > ia_writer = ia_discret_->Writer();
  ia_writer->WriteMesh( stepn, timen );
  ia_writer->NewStep( stepn, timen );
  ia_writer->WriteVector( "displacement", ia_state_ptr_->GetMutableDisNp() );
//  ia_writer->WriteElementData( true );

  // as we know that our maps have changed every time we write output, we can empty
  // the map cache as we can't get any advantage saving the maps anyway
  ia_writer->ClearMapCache();

  // visualize bins according to specification in input file ( MESHFREE -> WRITEBINS "" )
  binstrategy_->WriteBinOutput( GState().GetStepN(), GState().GetTimeN() );

  // write periodic bounding box output
  TimInt().GetDataSDynPtr()->GetPeriodicBoundingBox()->Output( stepn, timen );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Map> STR::MODELEVALUATOR::BeamInteraction::
    GetBlockDofRowMapPtr() const
{
  CheckInitSetup();
  return GState().DofRowMap();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Vector> STR::MODELEVALUATOR::BeamInteraction::
    GetCurrentSolutionPtr() const
{
  // there are no model specific solution entries
  return Teuchos::null;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Vector> STR::MODELEVALUATOR::BeamInteraction::
    GetLastTimeStepSolutionPtr() const
{
  // there are no model specific solution entries
  return Teuchos::null;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::PostOutput()
{
  TimInt().GetDataSDynPtr()->GetPeriodicBoundingBox()->ApplyDirichlet( GState().GetTimeN() );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::ResetStepState()
{
  // empty
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::UpdateCouplingAdapterAndMatrixTransformation()
{
  CheckInit();

  // reset transformation member variables (eg. exporter) by rebuilding
  // and provide new maps for coupling adapter
  siatransform_ = Teuchos::rcp( new FSI::UTILS::MatrixRowTransform );
  coupsia_->SetupCoupling( *ia_discret_, *discret_ptr_ );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::CreateNewBins(bool newxaabb,
                                                         bool newcutoff)
{
  CheckInitSetup();

  // todo: unshifted current positions are needed here
  if(newcutoff)
    dserror("unshifted configuration is needed (not yet here) for calculation of cutoff.");

  // store structure discretization in vector
  std::vector< Teuchos::RCP< DRT::Discretization > > discret_vec( 1, ia_discret_);
  // displacement vector according to periodic boundary conditions
  std::vector< Teuchos::RCP< Epetra_Vector > > disnp( 1, ia_state_ptr_->GetMutableDisNp());

  // create XAABB and optionally set cutoff radius
  if(newxaabb)
    binstrategy_->ComputeMinXAABBContainingAllElementsOfInputDiscrets(
        discret_vec, disnp, binstrategy_->XAABB(), newcutoff );
  // just set cutoff radius
  else if (newcutoff)
    binstrategy_->SetCutoffRadius( binstrategy_->ComputeMinCutoffAsMaxEdgeLengthOfXAABBOfLargestEle( discret_vec, disnp ) );

  binstrategy_->CreateBinsBasedOnCutoffAndXAABB();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::BuildRowEleToBinMap()
{
  CheckInit();

  // delete old map
  ia_state_ptr_->GetMutableRowEleToBinMap().clear();
  // loop over bins
  std::map<int, std::set<int> >::const_iterator biniter;
  for( biniter = ia_state_ptr_->GetBinToRowEleMap().begin() ; biniter != ia_state_ptr_->GetBinToRowEleMap().end(); ++biniter )
  {
    // loop over ele content of this bin
    std::set<int>::const_iterator eleiter;
    for( eleiter = biniter->second.begin(); eleiter != biniter->second.end(); ++eleiter )
    {
      int elegid = *eleiter;
      int bingid = biniter->first;
      // assign bins to elements
      ia_state_ptr_->GetMutableRowEleToBinMap()[elegid].insert(bingid);
    }
  }
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::UpdateMaps()
{
  CheckInit();

  // todo: performance improvement by using the same exporter object every time
  // and not doing the safety checks in Linalg::Export. See in particle_timint
  // how this can be done.
  //todo: check if update is necessary (->SameAs())

  // beam displacement
  UpdateDofMapOfVector( ia_discret_, ia_state_ptr_->GetMutableDisNp() );

  // get current displacement state and export to interaction discretization dofmap
  UpdateDofMapOfVector( ia_discret_, ia_state_ptr_->GetMutableDisNp(), GState().GetMutableDisNp() );
  // update colume vector
  ia_state_ptr_->GetMutableDisColNp() = Teuchos::rcp( new Epetra_Vector( *ia_discret_->DofColMap() ) );
  LINALG::Export(*ia_state_ptr_->GetDisNp(), *ia_state_ptr_->GetMutableDisColNp());

  // force
  ia_force_beaminteraction_ = Teuchos::rcp( new Epetra_Vector( *ia_discret_->DofRowMap() ,true ) );
  ia_state_ptr_->GetMutableForceNp() = Teuchos::rcp( new Epetra_FEVector( *ia_discret_->DofRowMap() ,true) );

  // stiff
  ia_state_ptr_->GetMutableStiff() = Teuchos::rcp( new LINALG::SparseMatrix(
      *ia_discret_->DofRowMap(), 81, true, true, LINALG::SparseMatrix::FE_MATRIX ) );

  BEAMINTERACTION::UTILS::SetupEleTypeMapExtractor( ia_discret_, eletypeextractor_ );
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::UpdateDofMapOfVector(
    Teuchos::RCP<DRT::Discretization> discret,
    Teuchos::RCP<Epetra_Vector>&      dofmapvec,
    Teuchos::RCP<Epetra_Vector>       old)
{
  CheckInit();

  // todo: performance improvement by using the same exporter object every time
  // and not doing the safety checks in Linalg::Export. See in particle_timint
  // how this can be done.

  if ( dofmapvec != Teuchos::null )
  {
    if( old == Teuchos::null )
      old = dofmapvec;
    dofmapvec = LINALG::CreateVector( *discret->DofRowMap(),true );
    LINALG::Export( *old, *dofmapvec );
  }
}

/*-----------------------------------------------------------------------------*
 *-----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::TransformForce()
{
  CheckInit();

  TEUCHOS_FUNC_TIME_MONITOR("STR::MODELEVALUATOR::BeamInteraction::TransformForce");

  // transform force vector to problem discret layout/distribution
  force_beaminteraction_ = coupsia_->MasterToSlave( ia_force_beaminteraction_ );
}

/*-----------------------------------------------------------------------------*
 *-----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::TransformStiff()
{
  CheckInit();

  TEUCHOS_FUNC_TIME_MONITOR("STR::MODELEVALUATOR::BeamInteraction::TransformStiff");

  stiff_beaminteraction_->UnComplete();
  // transform stiffness matrix to problem discret layout/distribution
  (*siatransform_)( *ia_state_ptr_->GetMutableStiff(), 1.0,
      ADAPTER::CouplingMasterConverter( *coupsia_ ), *stiff_beaminteraction_, false );
}

/*-----------------------------------------------------------------------------*
 *-----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::TransformForceStiff()
{
  CheckInit();

  TransformForce();
  TransformStiff();
}

/*-----------------------------------------------------------------------------*
 *-----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::PrintBinningInfoToScreen() const
{
  std::vector< Teuchos::RCP< DRT::Discretization > > discret_vec( 1, ia_discret_);
  std::vector< Teuchos::RCP<Epetra_Vector> > disnp_vec( 1, Teuchos::null );

  double calc_cutoff = binstrategy_->ComputeMinCutoffAsMaxEdgeLengthOfXAABBOfLargestEle(
      discret_vec, disnp_vec );
  LINALG::Matrix<3,2> XAABB(true);
  binstrategy_->ComputeMinXAABBContainingAllElementsOfInputDiscrets(
      discret_vec, disnp_vec, XAABB, false );
  if( GState().GetMyRank() == 0 )
  {
    IO::cout(IO::verbose) << " \n---------------------------------------------------------- " << IO::endl;
    IO::cout(IO::verbose) << " chosen/computed cutoff radius                      : " <<
        binstrategy_->CutoffRadius() << IO::endl;
    IO::cout(IO::verbose) << " largest edge length of largest element xaabb       : " <<
        calc_cutoff << IO::endl;
    IO::cout(IO::verbose) << "BOUNDINGBOX containing all elements of input discretization:\n " <<
        XAABB(0, 0) << " " << XAABB(1, 0) << " " << XAABB(2, 0) << " " <<
        XAABB(0, 1) << " " << XAABB(1, 1) << " " << XAABB(2, 1) << IO::endl;
    IO::cout(IO::verbose) << " ----------------------------------------------------------\n " << IO::endl;
  }
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::MODELEVALUATOR::BeamInteraction::Logo() const
{
  CheckInit();

  if( myrank_ == 0 )
  {
    IO::cout << "\n****************************************************************" << IO::endl;
    IO::cout << "*                                                              *" << IO::endl;
    IO::cout << "*          Welcome to the Beam Interaction Model Evaluator     *" << IO::endl;
    IO::cout << "*                                                              *" << IO::endl;
    IO::cout << "****************************************************************" << IO::endl;
    IO::cout << "                                                                  " << IO::endl;
    IO::cout << "                                                                  " << IO::endl;
    IO::cout << "                      0=========================0                 " << IO::endl;
    IO::cout << "                    //|   \\            /       /||                " << IO::endl;
    IO::cout << "                   // |    \\ |       |/       //||                " << IO::endl;
    IO::cout << "                  //  |  /  \\|       /       // ||                " << IO::endl;
    IO::cout << "                 //   |  \\   \\   /  /|\\     //  ||                " << IO::endl;
    IO::cout << "                //    |  /   |\\ /  / | \\   //   ||                " << IO::endl;
    IO::cout << "               //     |  \\   | \\     |  \\ //  / ||                " << IO::endl;
    IO::cout << "              //  \\  /|  /   |/      |   //  /  ||                " << IO::endl;
    IO::cout << "              0=========================0 \\ /   ||                " << IO::endl;
    IO::cout << "             ||    /\\ |____          |  || \\    ||                " << IO::endl;
    IO::cout << "             ||   /  \\|    \\   ------   ||/ \\   ||                " << IO::endl;
    IO::cout << "             ||  /    |                 ||      ||                " << IO::endl;
    IO::cout << "             || /     0----------/------||------0-                " << IO::endl;
    IO::cout << "             ||      /   /       \\      ||     //                 " << IO::endl;
    IO::cout << "             ||     /___/  \\     /    / ||    //                  " << IO::endl;
    IO::cout << "             ||    /        \\    \\   /  ||   //                   " << IO::endl;
    IO::cout << "             ||   /  \\/\\/\\/  \\   /  /   ||  //                    " << IO::endl;
    IO::cout << "             ||  /      /     \\  \\ /    || //                     " << IO::endl;
    IO::cout << "             || /      /         /      ||//                      " << IO::endl;
    IO::cout << "             ||/                       /||/                       " << IO::endl;
    IO::cout << "              0=========================0                         " << IO::endl;
    IO::cout << "                                                                     " << IO::endl;
    IO::cout << "                                                                     " << IO::endl;
  }
}
