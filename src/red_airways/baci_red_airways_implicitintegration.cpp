/*---------------------------------------------------------------------*/
/*! \file

\brief Associated with control routine for reduced dimensional airways
  solvers


\level 3

*/
/*---------------------------------------------------------------------*/
#include "baci_red_airways_implicitintegration.H"

#include "baci_io_control.H"
#include "baci_lib_condition_utils.H"
#include "baci_lib_function.H"
#include "baci_lib_globalproblem.H"
#include "baci_lib_utils_createdis.H"
#include "baci_lib_utils_parallel.H"
#include "baci_linalg_mapextractor.H"
#include "baci_linalg_utils_sparse_algebra_assemble.H"
#include "baci_linalg_utils_sparse_algebra_create.H"
#include "baci_linalg_utils_sparse_algebra_manipulation.H"
#include "baci_linear_solver_method_linalg.H"
#include "baci_mat_maxwell_0d_acinus_Ogden.H"
#include "baci_rebalance_utils.H"
#include "baci_red_airways_evaluation_data.h"
#include "baci_red_airways_resulttest.H"
#include "baci_utils_exceptions.H"

#include <stdio.h>

/*----------------------------------------------------------------------*
 |  Constructor (public)                                    ismail 01/10|
 *----------------------------------------------------------------------*/
AIRWAY::RedAirwayImplicitTimeInt::RedAirwayImplicitTimeInt(Teuchos::RCP<DRT::Discretization> actdis,
    std::unique_ptr<CORE::LINALG::Solver> solver, Teuchos::ParameterList& params,
    IO::DiscretizationWriter& output)
    :  // Call constructor for "nontrivial" objects
      discret_(actdis),
      solver_(std::move(solver)),
      params_(params),
      output_(output),
      time_(0.0),
      step_(0),
      uprestart_(params.get("write restart every", -1)),
      upres_(params.get("write solution every", -1)),
      coupledTo3D_(false)
{
  // Get the processor ID from the communicator
  myrank_ = discret_->Comm().MyPID();

  // Time measurement: initialization
  if (!coupledTo3D_)
  {
    // Time measurement: initialization
    TEUCHOS_FUNC_TIME_MONITOR(" + initialization");
  }

  // Get the basic parameters first
  // time-step size
  dtp_ = dta_ = params_.get<double>("time step size");
  // maximum number of timesteps
  stepmax_ = params_.get<int>("max number timesteps");
  // maximum simulation time
  maxtime_ = dtp_ * double(stepmax_);
  // maximum iteration steps
  maxiter_ = params_.get<int>("maximum iteration steps");
  // tolerance of nonlinear solution
  non_lin_tol_ = params_.get<double>("tolerance");
  // solve scatra
  solveScatra_ = params_.get<bool>("SolveScatra");
  // solve scatra
  compAwAcInter_ = params_.get<bool>("CompAwAcInter");

  // calculate acini volume0 flag; option for acini volume adjustment via prestress
  calcV0PreStress_ = params_.get<bool>("CalcV0PreStress");
  // transpulmonary pressure, only needed in case of prestressing
  if (calcV0PreStress_) transpulmpress_ = params_.get<double>("transpulmpress");

  // ensure that degrees of freedom in the discretization have been set
  if (!discret_->Filled() || !actdis->HaveDofs()) discret_->FillComplete();

  airway_acinus_dep_ = CORE::LINALG::CreateVector(*discret_->ElementColMap(), true);

  // extend ghosting of discretization to ensure correct neighbor search
  if (compAwAcInter_)
  {
    // to be filled with additional elements to be ghosted (needs to be done before
    // making discret fully overlapping)
    std::set<int> elecolset;
    const Epetra_Map* elecolmap = discret_->ElementColMap();
    for (int lid = 0; lid < elecolmap->NumMyElements(); ++lid)
    {
      int gid = elecolmap->GID(lid);
      elecolset.insert(gid);
    }

    // to be filled with additional nodes to be ghosted
    std::set<int> nodecolset;
    const Epetra_Map* nodecolmap = discret_->NodeColMap();
    for (int lid = 0; lid < nodecolmap->NumMyElements(); ++lid)
    {
      int gid = nodecolmap->GID(lid);
      nodecolset.insert(gid);
    }

    // make search discret fully overlapping on all procs
    DRT::UTILS::GhostDiscretizationOnAllProcs(discret_);
    discret_->FillComplete(false, false, false);

    // Get elements and nodes that need to be ghosted to have correct neighbor search
    // independent of number of procs
    ComputeNearestAcinus(discret_, &elecolset, &nodecolset, Teuchos::null);

    // extended ghosting for elements (also revert fully overlapping here)
    std::vector<int> coleles(elecolset.begin(), elecolset.end());
    Teuchos::RCP<const Epetra_Map> extendedelecolmap =
        Teuchos::rcp(new Epetra_Map(-1, coleles.size(), coleles.data(), 0, discret_->Comm()));

    discret_->ExportColumnElements(*extendedelecolmap);

    // extended ghosting for nodes
    std::vector<int> colnodes(nodecolset.begin(), nodecolset.end());
    Teuchos::RCP<const Epetra_Map> extendednodecolmap =
        Teuchos::rcp(new Epetra_Map(-1, colnodes.size(), colnodes.data(), 0, discret_->Comm()));

    discret_->ExportColumnNodes(*extendednodecolmap);

    // fill and inform user (not fully overlapping anymore at this point
    discret_->FillComplete();
    REBALANCE::UTILS::PrintParallelDistribution(*discret_);

    // Neighbouring acinus
    airway_acinus_dep_ = CORE::LINALG::CreateVector(*discret_->ElementColMap(), true);
    ComputeNearestAcinus(discret_, nullptr, nullptr, airway_acinus_dep_);
  }

  // -------------------------------------------------------------------
  // get a vector layout from the discretization to construct matching
  // vectors and matrices
  //                 local <-> global dof numbering
  // -------------------------------------------------------------------
  const Epetra_Map* dofrowmap = discret_->DofRowMap();
  const Epetra_Map* dofcolmap = discret_->DofColMap();
  const Epetra_Map* elementcolmap = discret_->ElementColMap();
  const Epetra_Map* elementrowmap = discret_->ElementRowMap();

  // -------------------------------------------------------------------
  // get a vector layout from the discretization for a vector which only
  // contains the volumetric flow rate dofs and for one vector which only
  // contains cross-sectional area degrees of freedom.
  // -------------------------------------------------------------------


  // This is a first estimate for the number of non zeros in a row of
  // the matrix. Each node has 3 adjacent nodes (including itself), each
  // with 1 dofs. (3*1=3)
  // We do not need the exact number here, just for performance reasons
  // a 'good' estimate

  // initialize standard (stabilized) system matrix
  sysmat_ = Teuchos::rcp(new CORE::LINALG::SparseMatrix(*dofrowmap, 3, false, true));

  // Vectors passed to the element
  // Pressures at time n+1, n and n-1
  pnp_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  pn_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  pnm_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  p_nonlin_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  n_intr_ac_ln_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  // Inlet volumetric flow rates at time n+1, n and n-1
  qin_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  qin_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  qin_nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // Trajectory vector x at time n+1 and n
  x_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  x_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // State of airway
  open_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // External pressure
  p_extnp_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  p_extn_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  pnp_colmap_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  pn_colmap_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // Outlet volumetric flow rates at time n+1, n and n-1
  qout_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  qout_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  qout_nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // This vector will be used for exportation and restart reasons
  qexp_ = CORE::LINALG::CreateVector(*elementrowmap, true);
  qexp2_ = CORE::LINALG::CreateVector(*elementrowmap, true);
  pexp_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  // Element volume at time n+1, n and n-1
  elemVolumenp_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  elemVolumen_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  elemVolumenm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  elemVolume0_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  elemArea0_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // Element radius at time n+1
  elemRadiusnp_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // This vector will be used to test convergence
  residual_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  bc_residual_ = CORE::LINALG::CreateVector(*dofcolmap, true);

  // Volumetric flow rates at time n+1, n and n-1
  //  qcnp_          = CORE::LINALG::CreateVector(*elementrowmap,true);
  //  qcn_           = CORE::LINALG::CreateVector(*elementrowmap,true);
  //  qcnm_          = CORE::LINALG::CreateVector(*elementrowmap,true);

  // Vectors for postprocessing, Element Node Ids, radii, generations, etc ...
  nodeIds_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  radii_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  generations_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // A vector of zeros to be used to enforce zero dirichlet boundary conditions
  // This part might be optimized later
  bcval_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  dbctog_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  acini_bc_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_e_volume0_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_e_volumenm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_e_volumen_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_e_volumenp_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_e_volume_strain_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  acini_max_strain_location_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // Initialize "scalar transpor" variables
  if (solveScatra_)
  {
    // Nodal values of the scalar transport
    scatraO2nm_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    scatraO2n_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    scatraO2np_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    dscatraO2_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    dVolumeO2_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    acinarDO2_ = CORE::LINALG::CreateVector(*dofrowmap, true);

    // Element values of the scalar transport (Needed to resolve the
    // the transport at the branching parts
    e1scatraO2nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
    e1scatraO2n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
    e1scatraO2np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

    e2scatraO2nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
    e2scatraO2n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
    e2scatraO2np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

    cfls_ = CORE::LINALG::CreateVector(*elementrowmap, true);

    // junctionVolumeInMix_ = CORE::LINALG::CreateVector(*dofcolmap,true);
    // junVolMix_Corrector_ = CORE::LINALG::CreateVector(*dofcolmap,true);
    // jVDofRowMix_         = CORE::LINALG::CreateVector(*dofrowmap,true);
    // diffusionArea_       = CORE::LINALG::CreateVector(*dofcolmap,true);

    junctionVolumeInMix_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    junVolMix_Corrector_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    jVDofRowMix_ = CORE::LINALG::CreateVector(*dofrowmap, true);
    diffusionArea_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  }

  // Vectors used for solution process
  // right hand side vector and right hand side corrector
  rhs_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  // ---------------------------------------------------------------------------------------
  // Initialize all the arteries' cross-sectional areas to the initial crossectional area Ao
  // and the volumetric flow rate to 0
  // ---------------------------------------------------------------------------------------
  Teuchos::ParameterList eleparams;

  // loop all elements and initialize all of the values

  // note: We use an RCP because ParameterList wants something printable and comparable
  DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();
  evaluation_data.p0np = pnp_;
  evaluation_data.p0n = pn_;
  evaluation_data.p0nm = pnm_;

  evaluation_data.generations = generations_;
  evaluation_data.acini_bc = acini_bc_;
  evaluation_data.acini_e_volume = acini_e_volumenp_;
  evaluation_data.solveScatra = solveScatra_;
  evaluation_data.elemVolume = elemVolumenp_;
  if (solveScatra_)
  {
    evaluation_data.junVolMix_Corrector = junVolMix_Corrector_;
    evaluation_data.scatranp = scatraO2np_;
    evaluation_data.e1scatranp = e1scatraO2np_;
    evaluation_data.e2scatranp = e2scatraO2np_;
  }
  evaluation_data.elemArea0 = elemArea0_;
  eleparams.set("action", "get_initial_state");

  Teuchos::RCP<Epetra_Vector> radii_in = CORE::LINALG::CreateVector(*dofrowmap, true);
  Teuchos::RCP<Epetra_Vector> radii_out = CORE::LINALG::CreateVector(*dofrowmap, true);

  discret_->Evaluate(eleparams, Teuchos::null, Teuchos::null, radii_in, radii_out, n_intr_ac_ln_);

  for (int i = 0; i < radii_->MyLength(); i++)
  {
    if ((*radii_in)[i] == 0.0)
    {
      (*radii_)[i] = (*radii_out)[i];
    }
    else if ((*radii_out)[i] == 0.0)
    {
      (*radii_)[i] = (*radii_in)[i];
    }
    else
    {
      (*radii_)[i] = 0.5 * ((*radii_in)[i] + (*radii_out)[i]);
    }
  }

  if (solveScatra_)
  {
    scatraO2n_->Update(1.0, *scatraO2np_, 0.0);
    scatraO2nm_->Update(1.0, *scatraO2np_, 0.0);

    e1scatraO2n_->Update(1.0, *e1scatraO2np_, 0.0);
    e1scatraO2nm_->Update(1.0, *e1scatraO2np_, 0.0);

    e2scatraO2n_->Update(1.0, *e2scatraO2np_, 0.0);
    e2scatraO2nm_->Update(1.0, *e2scatraO2np_, 0.0);
  }

  acini_e_volumen_->Update(1.0, *acini_e_volumenp_, 0.0);
  acini_e_volumenm_->Update(1.0, *acini_e_volumenp_, 0.0);
  acini_e_volume0_->Update(1.0, *acini_e_volumenp_, 0.0);
  elemVolumen_->Update(1.0, *elemVolumenp_, 0.0);
  elemVolumenm_->Update(1.0, *elemVolumenp_, 0.0);
  elemVolume0_->Update(1.0, *elemVolumenp_, 0.0);

  // Fill the NodeId vector
  for (int nele = 0; nele < discret_->NumMyColElements(); ++nele)
  {
    // get the element
    DRT::Element* ele = discret_->lColElement(nele);

    // get element location vector, dirichlet flags and ownerships
    std::vector<int> lm;
    std::vector<int> lmstride;
    // vector<int> lmowner;
    Teuchos::RCP<std::vector<int>> lmowner = Teuchos::rcp(new std::vector<int>);
    ele->LocationVector(*discret_, lm, *lmowner, lmstride);

    // loop all nodes of this element, add values to the global vectors

    if (myrank_ == (*lmowner)[0])
    {
      int gid = lm[0];
      double val = gid;
      nodeIds_->ReplaceGlobalValues(1, &val, &gid);
    }
    if (myrank_ == (*lmowner)[1])
    {
      int gid = lm[1];
      double val = gid;
      nodeIds_->ReplaceGlobalValues(1, &val, &gid);
    }
  }

  // Set initial open/close value for an airway
  for (int j = 0; j < (discret_->NumMyColElements()); j++)
  {
    // check if element is an airway
    if (((*generations_)[j] != -1) and ((*generations_)[j] != -2))
    {
      int GID = discret_->ElementColMap()->GID(j);  // global element ID
      const DRT::ElementType& ele_type = discret_->gElement(GID)->ElementType();
      if (ele_type == DRT::ELEMENTS::RedAirwayType::Instance())
      {
        // dynamic cast to airway element, since Elements base class does not have the functions
        // getParams and setParams
        DRT::ELEMENTS::RedAirway* ele =
            dynamic_cast<DRT::ELEMENTS::RedAirway*>(discret_->gElement(GID));
        // check if airway is collapsible
        double airwayColl = 0.0;
        ele->getParams("AirwayColl", airwayColl);
        if (airwayColl == 1)
        {
          double val;
          ele->getParams("Open_Init", val);

          // adjust airway states
          (*x_np_)[j] = val;
          (*x_n_)[j] = val;
          //(*open_)[j] = val;
        }
        double val;
        ele->getParams("Open_Init", val);
        (*open_)[j] = val;
      }
    }
  }

  std::vector<DRT::Condition*> conds;
  discret_->GetCondition("RedAirwayScatraExchangeCond", conds);
}  // RedAirwayImplicitTimeInt::RedAirwayImplicitTimeInt


/*----------------------------------------------------------------------*
 | Integrate () routine to start the time integration.                  |
 |                                                          ismail 01/10|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::Integrate()
{
  Teuchos::RCP<Teuchos::ParameterList> param;
  Integrate(false, param);
}  // RedAirwayImplicitTimeInt::Integrate()


/*----------------------------------------------------------------------*
 | Integrate () routine to start the time integration.                  |
 |                                                          ismail 01/10|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::Integrate(
    bool CoupledTo3D, Teuchos::RCP<Teuchos::ParameterList> CouplingParams)
{
  // Do prestressing if required
  if (calcV0PreStress_)
  {
    ComputeVol0ForPreStress();
  }

  // Get coupling parameters in case of 3D/0D coupling
  coupledTo3D_ = CoupledTo3D;
  if (CoupledTo3D && CouplingParams.get() == nullptr)
  {
    dserror(
        "Coupling parameter list is not allowed to be empty, If a 3-D/reduced-D coupling is "
        "defined\n");
  }

  // Start time loop
  TimeLoop(CoupledTo3D, CouplingParams);

  // Print the results of time measurements at the end of the simulation
  {
    Teuchos::TimeMonitor::summarize();
  }

  return;
}  // RedAirwayImplicitTimeInt::Integrate


/*-----------------------------------------------------------------------------*
 | Prestress the lung to a given transpulmonary pressure given in the .dat file|
 | This will shink the lung before the first timestep in such a way that the   |
 | volume of each acinus reaches the given volume in the .dat file when p_tp is|
 | applied                                                        roth 05/2015 |
 *-----------------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::ComputeVol0ForPreStress()
{
  double p = transpulmpress_;

  // loop over number of elements (on processor)
  for (int i = 0; i < (discret_->NumMyColElements()); i++)
  {
    // check if element is an acinus
    if ((*generations_)[i] == -1)
    {
      int GID = discret_->ElementColMap()->GID(i);  // global element ID

      // check if aciuns is ogden type material
      if (discret_->gElement(GID)->Material(0)->MaterialType() ==
          INPAR::MAT::m_0d_maxwell_acinus_ogden)
      {
        // get material parameters kappa and beta
        Teuchos::RCP<MAT::Maxwell_0d_acinus_Ogden> mymat =
            Teuchos::rcp_dynamic_cast<MAT::Maxwell_0d_acinus_Ogden>(
                discret_->gElement(GID)->Material(0));
        double kappa = mymat->GetParams("kappa");
        double beta = mymat->GetParams("beta");

        // calculate alpha
        double alpha = 0.1;  // starting value for approximation
        double f;
        double f_der;

        // get approximation for alpha with newton-raphson method
        while (std::abs(p - kappa / beta / alpha * (1 - std::pow(alpha, -beta))) > 1e-06)
        {
          f = p - kappa / beta / alpha * (1 - std::pow(alpha, -beta));
          f_der = kappa / beta * std::pow(alpha, -2.0) +
                  kappa / beta * (1 - beta) * std::pow(alpha, -beta);
          // get new alpha
          alpha = alpha - f / f_der;
        }

        // adjust acinus volume 0 in the elements parameters
        // additional check whether element is RedAcinusType
        const DRT::ElementType& ele_type = discret_->gElement(GID)->ElementType();
        if (ele_type == DRT::ELEMENTS::RedAcinusType::Instance())
        {
          // dynamic cast to aciunus element, since Elements base class does not have the functions
          // getParams and setParams
          DRT::ELEMENTS::RedAcinus* acini_ele =
              dynamic_cast<DRT::ELEMENTS::RedAcinus*>(discret_->gElement(GID));
          // get original value for aciuns volume (entered in dat file)
          double val;
          acini_ele->getParams("AcinusVolume_Init", val);
          // calculate new value for aciuns volume with alpha and set in element parameters
          val = val / alpha;
          acini_ele->setParams("AcinusVolume", val);

          // adjust acini volumes in the vectors used in this function
          if (not DRT::Problem::Instance()->Restart())
          {
            (*acini_e_volumenp_)[i] = val;
            (*acini_e_volumen_)[i] = val;
            (*acini_e_volumenm_)[i] = val;
            (*acini_e_volume0_)[i] = val;
          }
        }
      }
      else
      {
        std::cout << "Warning! Acinus " << GID
                  << " is not Ogden type material! Initial volume cannot be adjusted!" << std::endl;
      }
    }
  }
}

/*-----------------------------------------------------------------------------*
 |                                                                roth 02/2016 |
 *-----------------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::ComputeNearestAcinus(
    Teuchos::RCP<DRT::Discretization const> search_discret, std::set<int>* elecolset,
    std::set<int>* nodecolset, Teuchos::RCP<Epetra_Vector> airway_acinus_dep)
{
  // Loop over all airways contained on this proc
  for (int j = 0; j < (search_discret->NumMyColElements()); j++)
  {
    // global element ID airway
    int GID1 = search_discret->ElementColMap()->GID(j);

    // check if element is airway element
    DRT::ELEMENTS::RedAirway* ele_aw =
        dynamic_cast<DRT::ELEMENTS::RedAirway*>(search_discret->gElement(GID1));

    // check if element j is an airway
    if (ele_aw != nullptr)
    {
      double diff_norm = 1e3;
      double min_norm = 1e3;
      int min_index = 0;

      // Get coordinates for airway center point
      double node_coords1[3];
      node_coords1[0] = ele_aw->Nodes()[0]->X()[0];
      node_coords1[1] = ele_aw->Nodes()[0]->X()[1];
      node_coords1[2] = ele_aw->Nodes()[0]->X()[2];

      double node_coords2[3];
      node_coords2[0] = ele_aw->Nodes()[1]->X()[0];
      node_coords2[1] = ele_aw->Nodes()[1]->X()[1];
      node_coords2[2] = ele_aw->Nodes()[1]->X()[2];


      double node_coords_center[3];
      for (int p = 0; p < 3; p++) node_coords_center[p] = (node_coords1[p] + node_coords2[p]) / 2;

      // Loop over all acinus elements (on processor)
      for (int i = 0; i < (search_discret->NumMyColElements()); i++)
      {
        // global acinus element ID
        int GID2 = search_discret->ElementColMap()->GID(i);

        DRT::ELEMENTS::RedAcinus* ele_ac =
            dynamic_cast<DRT::ELEMENTS::RedAcinus*>(search_discret->gElement(GID2));

        // Check if element is an acinus
        if (ele_ac != nullptr)
        {
          // Get coordinates of acini
          double ac_coords[3];
          ac_coords[0] = ele_ac->Nodes()[1]->X()[0];
          ac_coords[1] = ele_ac->Nodes()[1]->X()[1];
          ac_coords[2] = ele_ac->Nodes()[1]->X()[2];

          double diff_vec[3];  // = ac_coords - airway_node_coords_center;
          for (int k = 0; k < 3; k++) diff_vec[k] = ac_coords[k] - node_coords_center[k];
          double accum = 0;

          for (int m = 0; m < 3; m++) accum += diff_vec[m] * diff_vec[m];
          diff_norm = std::sqrt(accum);

          if (diff_norm < min_norm)
          {
            min_norm = diff_norm;
            min_index = i;
          }
        }
      }

      // global element ID
      int GID3 = search_discret->ElementColMap()->GID(min_index);

      // why cast
      DRT::ELEMENTS::RedAcinus* ele_acinus =
          dynamic_cast<DRT::ELEMENTS::RedAcinus*>(search_discret->gElement(GID3));

      // extend ele and node col map
      if (elecolset != nullptr and nodecolset != nullptr)
      {
        elecolset->insert(GID3);
        const int* nodeids = ele_acinus->NodeIds();
        for (int inode = 0; inode < ele_acinus->NumNode(); ++inode)
          nodecolset->insert(nodeids[inode]);
      }

      if (airway_acinus_dep != Teuchos::null)
        (*airway_acinus_dep)[j] = (ele_acinus->Nodes()[1])->LID();
    }
  }
}

/*----------------------------------------------------------------------*
 | Time loop for red_airway problems                                    |
 |                                                          ismail 01/10|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::TimeLoop(
    bool CoupledTo3D, Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  coupledTo3D_ = CoupledTo3D;

  // Time measurement: time loop
  if (!coupledTo3D_)
  {
    TEUCHOS_FUNC_TIME_MONITOR(" + time loop");
  }

  // Do the time-stepping
  while (step_ < stepmax_ and time_ < maxtime_)
  {
    // Calculate a single timestep
    this->TimeStep(CoupledTo3D, CouplingTo3DParams);

    // Stop-criterion for timeloop
    if (CoupledTo3D)
    {
      break;
    }
  }

}  // RedAirwayImplicitTimeInt::TimeLoop



/*----------------------------------------------------------------------*
 | Contains one timestep: Prepare, Solve, Update, Output                |
 |                                                          ismail 09/12|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::TimeStep(
    bool CoupledTo3D, Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  coupledTo3D_ = CoupledTo3D;

  double time3D = time_;
  if (coupledTo3D_)
  {
    time3D = CouplingTo3DParams->get<double>("time");
    time_ = time3D - dta_;
  }

  // Prepare current timestep
  PrepareTimeStep();

  // Output to screen
  if (myrank_ == 0)
  {
    if (!coupledTo3D_)
    {
      printf(
          "TIME: %11.4E/%11.4E  DT = %11.4E   Solving Reduced Dimensional Airways    STEP = "
          "%4d/%4d \n",
          time_, maxtime_, dta_, step_, stepmax_);
    }
    else
    {
      printf(
          "SUBSCALE_TIME: %11.4E/%11.4E  SUBSCALE_DT = %11.4E   Solving Reduced Dimensional "
          "Airways    SUBSCALE_STEP = %4d/%4d \n",
          time_, maxtime_, dta_, step_, stepmax_);
    }
  }

  // Get the solver type parameter: linear or nonlinear solver and solve current timestep
  if (params_.get<std::string>("solver type") == "Nonlinear")
  {
    // Nonlinear solve of current timestep
    NonLin_Solve(CouplingTo3DParams);
    if (!myrank_) std::cout << std::endl;
  }
  else if (params_.get<std::string>("solver type") == "Linear")
  {
    // Linear solve of current timestep
    Solve(CouplingTo3DParams);
    if (!myrank_) std::cout << std::endl << std::endl;
  }
  else
  {
    dserror("[%s] is not a defined solver", (params_.get<std::string>("solver type")).c_str());
  }

  // Solve scatra if required
  if (solveScatra_)
  {
    this->SolveScatra(CouplingTo3DParams);
  }

  // Update solution: current solution becomes old solution of next timestep
  TimeUpdate();


  // Normal red_airway Output
  if (!CoupledTo3D)
  {
    Output(CoupledTo3D, CouplingTo3DParams);
  }

  // Update time step sizes
  dtp_ = dta_;

}  // RedAirwayImplicitTimeInt::TimeStep


/*----------------------------------------------------------------------*
 | Contains the one step time loop for red_airway_tissue problems       |
 |                                                          ismail 09/12|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::IntegrateStep(
    Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  // Output to screen
  if (myrank_ == 0)
  {
    printf(
        "----------------------------------------- STARTING NEW ITERATION "
        "-----------------------------------------\n");
    printf(
        "TIME: %11.4E/%11.4E  DT = %11.4E   Solving Reduced Dimensional Airways    STEP = %4d/%4d "
        "\n",
        time_, maxtime_, dta_, step_, stepmax_);
  }

  // Get the solver type parameter: linear or nonlinear solver and solve current timestep
  if (params_.get<std::string>("solver type") == "Nonlinear")
  {
    NonLin_Solve(CouplingTo3DParams);
    if (!myrank_) std::cout << std::endl;
  }
  else if (params_.get<std::string>("solver type") == "Linear")
  {
    Solve(CouplingTo3DParams);
    if (!myrank_) std::cout << std::endl << std::endl;
  }
  else
  {
    dserror("[%s] is not a defined solver", (params_.get<std::string>("solver type")).c_str());
  }

}  // RedAirwayImplicitTimeInt::IntegrateStep


/*----------------------------------------------------------------------*
 | Setup the variables to do a new time step                ismail 01/10|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::PrepareTimeStep()
{
  rhs_->PutScalar(0.0);
  // Set time dependent parameters
  step_ += 1;
  time_ += dta_;
}  // RedAirwayImplicitTimeInt::PrepareTimeStep


/*----------------------------------------------------------------------*
 | Nonlinear iterative solver for reduced-dimensional airway problem    |
 |                                                         ismail 01/11 |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::NonLin_Solve(
    Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  double error_norm1 = 1.e7;
  double error_norm2 = 1.e7;

  // Evaluate total acinar volume
  double acinar_volume_np = 0.0;
  bool err1 = this->SumAllColElemVal(acini_e_volumenp_, acini_bc_, acinar_volume_np);
  if (err1)
  {
    dserror("Error in summing acinar volumes");
  }

  // Evaluate total airway volume
  double airway_volume_np = 0.0;
  bool err2 = this->SumAllColElemVal(elemVolumenp_, open_, airway_volume_np);
  if (err2)
  {
    dserror("Error in summing airway volumes");
  }

  // Evaluate total lung volume (in acini and airways)
  double lung_volume_np = acinar_volume_np + airway_volume_np;

  // Print out the different volumes
  if (!myrank_)
  {
    std::cout << "time: " << time_ - dta_ << "\t\tTotalLungVolume: " << lung_volume_np
              << "\tAirwayVolume: " << airway_volume_np << "\tAcinarVolume: " << acinar_volume_np
              << std::endl;
  }

  // Loop over nonlinear iterations
  for (int i = 1; i <= maxiter_; i++)
  {
    // Update the pressures of the previous time step
    p_nonlin_->Update(1.0, *pnp_, 0.0);

    // Solve the reduced dimensional model
    this->Solve(CouplingTo3DParams);

    // Find the change of pressure between the last two iteration steps
    p_nonlin_->Update(1.0, *pnp_, -1.0);

    // Evaluate the L2 norm of the pressure difference
    p_nonlin_->Norm2(&error_norm1);

    // Evaluate the residual (=flow) and compute the L2 norm
    this->EvalResidual(CouplingTo3DParams);
    residual_->Norm2(&error_norm2);

    // Print output to screen
    if (!myrank_)
    {
      printf("Nonlinear iteration step %4d/%4d ", i, maxiter_);
      printf(" | ||P{%d}-P{%d}||_L2 = %10.3E\t\t|Qresidual|_2 = %10.3E\n", i - 1, i, error_norm1,
          error_norm2);
    }

    // If L2 norm is smaller than tolerance then proceed
    if (error_norm1 <= non_lin_tol_) break;
  }

  // Compute maximal and minimal pressure pnp_ and flux q_in for screen information
  {
    double maxQ = 0.0;
    double maxP = 0.0;
    double minQ = 0.0;
    double minP = 0.0;
    Teuchos::RCP<Epetra_Vector> qabs = Teuchos::rcp(new Epetra_Vector(*qin_np_));
    Teuchos::RCP<Epetra_Vector> pabs = Teuchos::rcp(new Epetra_Vector(*pnp_));
    qabs->Abs(*qin_np_);
    pabs->Abs(*pnp_);

    qabs->MaxValue(&maxQ);
    pabs->MaxValue(&maxP);
    qabs->MinValue(&minQ);
    pabs->MinValue(&minP);
    if (!myrank_)
    {
      printf(" |Pressure|_max: %10.3E \t\t\t |Q|_max: %10.3E\n", maxP, maxQ);
      printf(" |Pressure|_min: %10.3E \t\t\t |Q|_min: %10.3E\n", minP, minQ);
    }
  }

  if (!myrank_) printf("\n");
}


/*----------------------------------------------------------------------*
 | Single Newton step for reduced dimensional airways                   |
 |                                                         ismail 01/10 |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::Solve(
    Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  // Time measurement:  solving reduced dimensional airways
  if (!coupledTo3D_)
  {
    TEUCHOS_FUNC_TIME_MONITOR("   + solving reduced dimensional airways");
  }

  /***
   * 1. Call elements to calculate system matrix and rhs
   ***/
  {
    // Time measurement: element calls
    if (!coupledTo3D_)
    {
      TEUCHOS_FUNC_TIME_MONITOR("      + element calls");
    }

    // Set both system matrix and rhs vector to zero
    sysmat_->Zero();
    rhs_->PutScalar(0.0);

    // Create the element parameters for the discretization
    Teuchos::ParameterList eleparams;

    // Set action for elements: calc_sys_matrix_rhs
    eleparams.set("action", "calc_sys_matrix_rhs");

    // Set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);
    discret_->SetState("pn", pn_);
    discret_->SetState("pnm", pnm_);
    discret_->SetState("intr_ac_link", n_intr_ac_ln_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qin_nm = qin_nm_;

    evaluation_data.x_np = x_np_;
    evaluation_data.x_n = x_n_;
    evaluation_data.open = open_;

    evaluation_data.airway_acinus_dep = airway_acinus_dep_;
    evaluation_data.p_extnp = p_extnp_;
    evaluation_data.p_extn = p_extn_;
    evaluation_data.compute_awacinter = compAwAcInter_;

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.qout_np = qout_np_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qout_nm = qout_nm_;

    evaluation_data.elemVolumen = elemVolumen_;
    evaluation_data.elemVolumenp = elemVolumenp_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    // Evaluate Lung volumes nm, n, np and set to eleparams
    double lung_volume_np = 0.0;
    bool err = this->SumAllColElemVal(acini_e_volumenp_, acini_bc_, lung_volume_np);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    double lung_volume_n = 0.0;
    err = this->SumAllColElemVal(acini_e_volumen_, acini_bc_, lung_volume_n);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    double lung_volume_nm = 0.0;
    err = this->SumAllColElemVal(acini_e_volumenm_, acini_bc_, lung_volume_nm);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    evaluation_data.lungVolume_np = lung_volume_np;
    evaluation_data.lungVolume_n = lung_volume_n;
    evaluation_data.lungVolume_nm = lung_volume_nm;


    // Call standard loop over all elements
    discret_->Evaluate(eleparams, sysmat_, rhs_);
    discret_->ClearState();

    // finalize the complete matrix
    sysmat_->Complete();
    discret_->ClearState();

  }  // end time measurement for element

  /***
   * 2. Solve the boundary conditions
   ***/
  bcval_->PutScalar(0.0);
  dbctog_->PutScalar(0.0);
  {
    // Create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // Set action for elements: set_bc
    eleparams.set("action", "set_bc");

    // Set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);
    discret_->SetState("pn", pn_);
    discret_->SetState("pnm", pnm_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;

    evaluation_data.qout_np = qout_np_;
    evaluation_data.qout_n = qout_n_;

    evaluation_data.airway_acinus_dep = airway_acinus_dep_;
    evaluation_data.p_extnp = p_extnp_;
    evaluation_data.p_extn = p_extn_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;
    evaluation_data.bcval = bcval_;
    evaluation_data.dbctog = dbctog_;

    // Evaluate Lung volumes n, np and set to eleparams
    double lung_volume_np = 0.0;
    bool err = this->SumAllColElemVal(acini_e_volumenp_, acini_bc_, lung_volume_np);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }
    double lung_volume_n = 0.0;
    err = this->SumAllColElemVal(acini_e_volumen_, acini_bc_, lung_volume_n);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }
    evaluation_data.lungVolume_np = lung_volume_np;
    evaluation_data.lungVolume_n = lung_volume_n;

    // Add the parameters to solve terminal BCs coupled to 3D fluid boundary
    eleparams.set("coupling with 3D fluid params", CouplingTo3DParams);

    // Call standard loop over all elements
    discret_->Evaluate(eleparams, sysmat_, rhs_);
    discret_->ClearState();

  }  // end of solving terminal BCs

  /*std::cout<<"----------------------- My SYSMAT IS
  ("<<myrank_<<"-----------------------"<<std::endl; Teuchos::RCP<CORE::LINALG::SparseMatrix>
  A_debug = Teuchos::rcp_dynamic_cast<CORE::LINALG::SparseMatrix>(sysmat_); if (A_debug !=
  Teuchos::null)
  {
     (A_debug->EpetraMatrix())->Print(std::cout);
  }
   //               std::cout<<"Map is:
  ("<<myrank_<<")"<<std::endl<<*(discret_->DofRowMap())<<std::endl;
  std::cout<<"---------------------------------------("<<myrank_<<"------------------------"<<std::endl;

  std::cout << "rhs_ = " << std::endl;
  rhs_->Print(std::cout);*/

  // double norm_bc_tog = 0.0;
  // rhs_->Norm1(&norm_bc_tog);

  /***
   * 3. Apply the BCs to the system matrix and rhs
   ***/
  {
    // Time measurement: application of dbc
    if (!coupledTo3D_)
    {
      TEUCHOS_FUNC_TIME_MONITOR("      + apply DBC");
    }

    CORE::LINALG::ApplyDirichletToSystem(*sysmat_, *pnp_, *rhs_, *bcval_, *dbctog_);
  }

  /***
   * 4. Solve for total new velocities and pressures
   ***/
  // Get cpu time
  const double tcpusolve = Teuchos::Time::wallTime();
  {
    // Time measurement: solver
    if (!coupledTo3D_)
    {
      TEUCHOS_FUNC_TIME_MONITOR("      + solver calls");
    }
    // Call solver
    solver_->Solve(sysmat_->EpetraOperator(), pnp_, rhs_, true, true);
  }

  // end time measurement for solver
  dtsolve_ = Teuchos::Time::wallTime() - tcpusolve;

  if (myrank_ == 0) printf("ts=%4.7f |", dtsolve_);

  /***
   * 5. Compute the flow rates
   ***/
  {
    // Create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // Set action for elements: claculate flow rates
    eleparams.set("action", "calc_flow_rates");

    // Set solution type
    eleparams.set("solver type", params_.get<std::string>("solver type"));

    // Set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);
    discret_->SetState("pn", pn_);
    discret_->SetState("pnm", pnm_);
    discret_->SetState("intr_ac_link", n_intr_ac_ln_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_nm = qin_nm_;
    evaluation_data.qout_nm = qout_nm_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;
    evaluation_data.elemVolumen = elemVolumen_;
    evaluation_data.elemVolumenp = elemVolumenp_;
    evaluation_data.acinar_vnp_strain = acini_e_volume_strain_;
    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.x_n = x_n_;
    evaluation_data.x_np = x_np_;
    evaluation_data.open = open_;
    evaluation_data.airway_acinus_dep = airway_acinus_dep_;
    evaluation_data.p_extnp = p_extnp_;
    evaluation_data.p_extn = p_extn_;
    evaluation_data.compute_awacinter = compAwAcInter_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    // Call standard loop over all elements
    discret_->Evaluate(
        eleparams, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
    discret_->ClearState();
  }

  /***
   * 6. Compute the element volume
   ***/
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // action for elements
    eleparams.set("action", "calc_elem_volumes");

    // set vector values needed by elements
    discret_->ClearState();

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.acinar_vn = acini_e_volumen_;

    evaluation_data.dt = dta_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;
    evaluation_data.elemVolumen = elemVolumen_;
    evaluation_data.elemVolumenp = elemVolumenp_;
    evaluation_data.elemRadiusnp = elemRadiusnp_;

    // call standard loop over all elements
    discret_->Evaluate(
        eleparams, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
    discret_->ClearState();
  }

  /***
   * 7. In case of coupling to 3D fluid
   ***/
  if (coupledTo3D_)
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // action for elements
    eleparams.set("action", "get_coupled_values");

    // set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;

    evaluation_data.qout_np = qout_np_;
    evaluation_data.qout_n = qout_n_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    evaluation_data.x_n = x_n_;
    evaluation_data.x_np = x_np_;
    evaluation_data.open = open_;
    evaluation_data.airway_acinus_dep = airway_acinus_dep_;
    evaluation_data.p_extnp = p_extnp_;
    evaluation_data.p_extn = p_extn_;
    evaluation_data.compute_awacinter = compAwAcInter_;
    // Add the parameters to solve terminal BCs coupled to 3D fluid boundary
    eleparams.set("coupling with 3D fluid params", CouplingTo3DParams);

    // call standard loop over all elements
    discret_->Evaluate(eleparams, sysmat_, rhs_);
    discret_->ClearState();
  }
}  // RedAirwayImplicitTimeInt::Solve


//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
/*----------------------------------------------------------------------*
 | the solver for solving the scalar transport             ismail 02/13 |
 *----------------------------------------------------------------------*/
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
void AIRWAY::RedAirwayImplicitTimeInt::SolveScatra(
    Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  //---------------------------------------------------------------------
  // Get the largest CFL number in the airways to scale down
  // the time if CFL>1
  //---------------------------------------------------------------------
  double cflmax = 0.0;
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "calc_cfl");

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.elemVolumenp = elemVolumenp_;
    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    evaluation_data.cfl = cfls_;

    cfls_->PutScalar(0.0);

    discret_->Evaluate(
        eleparams, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
    discret_->ClearState();

    // get largest CFL number
    cfls_->NormInf(&cflmax);

    if (!myrank_) std::cout << "MAX CFL NUMBER IS!!!" << cflmax << std::endl;
  }

  //---------------------------------------------------------------------
  // Get the junctions area in which a scatran is flowing
  //---------------------------------------------------------------------
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "get_junction_volume_mix");

    // set vector values needed to evaluate O2 transport elements
    discret_->ClearState();

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    junctionVolumeInMix_->PutScalar(0.0);
    evaluation_data.elemVolumenp = elemVolumenp_;

    discret_->Evaluate(
        eleparams, sysmat_, Teuchos::null, junctionVolumeInMix_, Teuchos::null, Teuchos::null);
  }
  //---------------------------------------------------------------------
  // Find/ Solve the scatran that is flowing forward
  //---------------------------------------------------------------------
  {
    scatraO2np_->PutScalar(0.0);
    e1scatraO2np_->PutScalar(0.0);
    e2scatraO2np_->PutScalar(0.0);
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "solve_scatra");

    // set vector values needed to evaluate O2 transport elements
    discret_->ClearState();
    discret_->SetState("scatran", scatraO2n_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.e1scatran = e1scatraO2n_;
    evaluation_data.e1scatranp = e1scatraO2np_;

    evaluation_data.e2scatran = e2scatraO2n_;
    evaluation_data.e2scatranp = e2scatraO2np_;

    discret_->SetState("junctionVolumeInMix", junctionVolumeInMix_);

    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.elemVolumenp = elemVolumenp_;
    evaluation_data.elemVolumen = elemVolumen_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    const Epetra_Map* dofrowmap = discret_->DofRowMap();
    Teuchos::RCP<Epetra_Vector> dummy = CORE::LINALG::CreateVector(*dofrowmap, true);
    discret_->Evaluate(eleparams, sysmat_, Teuchos::null, scatraO2np_, dummy, Teuchos::null);
    discret_->ClearState();
  }
  //---------------------------------------------------------------------
  // Reconvert the scatran from 'number of moles' into a concentration
  //---------------------------------------------------------------------
  {
    Epetra_Export exporter(junctionVolumeInMix_->Map(), jVDofRowMix_->Map());
    int err = jVDofRowMix_->Export(*junctionVolumeInMix_, exporter, Zero);
    if (err) dserror("Export using exporter returned err=%d", err);
  }
  for (int i = 0; i < scatraO2np_->MyLength(); i++)
  {
    if ((*jVDofRowMix_)[i] != 0.0)
    {
      (*scatraO2np_)[i] /= (*jVDofRowMix_)[i];
    }
  }

  //---------------------------------------------------------------------
  // Solve the scatran for the nodes that recieve their values from the
  // neighbouring element/airway-branch
  //---------------------------------------------------------------------
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "solve_junction_scatra");

    // set vector values needed to evaluate O2 transport elements
    discret_->ClearState();

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.e1scatranp = e1scatraO2np_;
    evaluation_data.e2scatranp = e2scatraO2np_;

    discret_->SetState("scatranp", scatraO2np_);

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qout_np = qout_np_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    evaluation_data.elemVolumenp = elemVolumenp_;
    discret_->SetState("junctionVolumeInMix", junctionVolumeInMix_);

    discret_->Evaluate(
        eleparams, sysmat_, Teuchos::null, scatraO2np_, junctionVolumeInMix_, Teuchos::null);
    discret_->ClearState();
  }

  //---------------------------------------------------------------------
  // Solve the scatra between air and blood region
  //---------------------------------------------------------------------
  // define an empty capillary flowrate vector
  const Epetra_Map* dofrowmap = discret_->DofRowMap();
  // Diffusion surface (from the acinar side)
  Teuchos::RCP<Epetra_Vector> nodal_surfaces = CORE::LINALG::CreateVector(*dofrowmap, true);
  // Fluid volume
  Teuchos::RCP<Epetra_Vector> nodal_volumes = CORE::LINALG::CreateVector(*dofrowmap, true);
  // Average concentration in Acini and in Capillar
  Teuchos::RCP<Epetra_Vector> nodal_avg_conc = CORE::LINALG::CreateVector(*dofrowmap, true);

  {
    // get the diffusion surfaces at the acini
    Teuchos::ParameterList eleparams;
    eleparams.set("action", "eval_nodal_essential_values");

    // set vector values of flow rates
    discret_->SetState("scatranp", scatraO2np_);
    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();
    evaluation_data.acinar_v = acini_e_volumenp_;

    // set vector values of flow rates
    eleparams.set("time step size", dta_);
    eleparams.set("time step size", dta_);
    // TODO used to be "qnp" and was likely unused
    evaluation_data.qin_np = qin_np_;

    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.elemVolumenp = elemVolumenp_;

    discret_->Evaluate(
        eleparams, Teuchos::null, Teuchos::null, nodal_surfaces, nodal_volumes, nodal_avg_conc);
    discret_->ClearState();
  }

  // evaluate the transport of O2 between air and blood
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "solve_blood_air_transport");

    discret_->SetState("areanp", nodal_surfaces);
    discret_->SetState("volumenp", nodal_volumes);
    discret_->SetState("scatranp", nodal_avg_conc);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();
    evaluation_data.elemVolumenp = elemVolumenp_;
    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    dscatraO2_->PutScalar(0.0);
    dVolumeO2_->PutScalar(0.0);
    acinarDO2_->PutScalar(0.0);
    discret_->Evaluate(eleparams, Teuchos::null, Teuchos::null, dscatraO2_, dVolumeO2_, acinarDO2_);
    discret_->ClearState();
  }

  //---------------------------------------------------------------------
  // Update scatra
  //---------------------------------------------------------------------
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "update_scatra");

    // set vector values needed to evaluate O2 transport elements
    discret_->ClearState();
    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.e1scatranp = e1scatraO2np_;
    evaluation_data.e2scatranp = e2scatraO2np_;
    evaluation_data.dscatranp = dscatraO2_;
    discret_->SetState("dscatranp", dscatraO2_);
    discret_->SetState("avg_scatranp", nodal_avg_conc);
    discret_->SetState("scatranp", scatraO2np_);
    evaluation_data.elemVolumenp = elemVolumenp_;

    discret_->Evaluate(
        eleparams, sysmat_, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
    discret_->ClearState();
  }
  scatraO2np_->Update(1.0, *dscatraO2_, 1.0);
  //---------------------------------------------------------------------
  // Update element12 scatra
  //---------------------------------------------------------------------
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;
    // action for elements
    eleparams.set("action", "update_elem12_scatra");

    // set vector values needed to evaluate O2 transport elements
    discret_->ClearState();

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.e1scatranp = e1scatraO2np_;
    evaluation_data.e2scatranp = e2scatraO2np_;
    discret_->SetState("dscatranp", dscatraO2_);
    discret_->SetState("scatranp", scatraO2np_);
    discret_->SetState("junctionVolumeInMix", junctionVolumeInMix_);
    evaluation_data.elemVolumenp = elemVolumenp_;
    discret_->Evaluate(
        eleparams, sysmat_, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
    discret_->ClearState();
  }
}  // RedAirwayImplicitTimeInt::SolveScatra


/*----------------------------------------------------------------------*
 | Call elements to calculate system matrix/rhs and assemble.           |
 | This function is currently not used but will be kept empty until     |
 | further use.                                            ismail 01/10 |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::AssembleMatAndRHS()
{
  dtele_ = 0.0;
  dtfilter_ = 0.0;
  // time measurement: element
  if (!coupledTo3D_)
  {
    TEUCHOS_FUNC_TIME_MONITOR("      + element calls");
  }

}  // RedAirwayImplicitTimeInt::AssembleMatAndRHS


/*----------------------------------------------------------------------*
 | build system matrix and rhs                              ismail 01/10|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::Evaluate(Teuchos::RCP<const Epetra_Vector> qael) {}


/*----------------------------------------------------------------------*
 | Current solution becomes most recent solution of next timestep       |
 |                                                                      |
 |  pnm_  =  pn_                                                        |
 |  pn_   =  pnp_                                                       |
 |                                                          ismail 06/09|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::TimeUpdate()
{
  // Volumetric Flow rate and acini volume of this step become most recent
  pnm_->Update(1.0, *pn_, 0.0);
  pn_->Update(1.0, *pnp_, 0.0);

  qin_nm_->Update(1.0, *qin_n_, 0.0);
  qin_n_->Update(1.0, *qin_np_, 0.0);

  // Timeupdate x-vector x_np ->x_n
  x_n_->Update(1.0, *x_np_, 0.0);

  qout_nm_->Update(1.0, *qout_n_, 0.0);
  qout_n_->Update(1.0, *qout_np_, 0.0);

  acini_e_volumenm_->Update(1.0, *acini_e_volumen_, 0.0);
  acini_e_volumen_->Update(1.0, *acini_e_volumenp_, 0.0);

  elemVolumenm_->Update(1.0, *elemVolumen_, 0.0);
  elemVolumen_->Update(1.0, *elemVolumenp_, 0.0);

  if (solveScatra_)
  {
    scatraO2nm_->Update(1.0, *scatraO2n_, 0.0);
    scatraO2n_->Update(1.0, *scatraO2np_, 0.0);

    e1scatraO2nm_->Update(1.0, *e1scatraO2n_, 0.0);
    e1scatraO2n_->Update(1.0, *e1scatraO2np_, 0.0);

    e2scatraO2nm_->Update(1.0, *e2scatraO2n_, 0.0);
    e2scatraO2n_->Update(1.0, *e2scatraO2np_, 0.0);
  }

  return;
}  // RedAirwayImplicitTimeInt::TimeUpdate


/*----------------------------------------------------------------------*
 | Initializes state saving vectors                                     |
 |                                                                      |
 |  This is currently needed for strongly coupling 3D-0D fields         |
 |                                                                      |
 |                                                          ismail 04/14|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::InitSaveState()
{
  // Get discretizations DOF row map
  const Epetra_Map* dofrowmap = discret_->DofRowMap();

  // Get discretizations element row map
  const Epetra_Map* elementcolmap = discret_->ElementColMap();

  // saving vector for pressure
  saved_pnm_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  saved_pn_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  saved_pnp_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  // saving vector for inflow rate
  saved_qin_nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_qin_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_qin_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for outflow rate
  saved_qout_nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_qout_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_qout_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for trajectory
  saved_x_n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_x_np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for acinar volume
  saved_acini_e_volumenm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_acini_e_volumen_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_acini_e_volumenp_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for element volume
  saved_elemVolumenm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_elemVolumen_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_elemVolumenp_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for nodal O2 concentration
  saved_scatraO2nm_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  saved_scatraO2n_ = CORE::LINALG::CreateVector(*dofrowmap, true);
  saved_scatraO2np_ = CORE::LINALG::CreateVector(*dofrowmap, true);

  // saving vector for element inlet O2 concentration
  saved_e1scatraO2nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_e1scatraO2n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_e1scatraO2np_ = CORE::LINALG::CreateVector(*elementcolmap, true);

  // saving vector for element outlet O2 concentration
  saved_e2scatraO2nm_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_e2scatraO2n_ = CORE::LINALG::CreateVector(*elementcolmap, true);
  saved_e2scatraO2np_ = CORE::LINALG::CreateVector(*elementcolmap, true);
}  // RedAirwayImplicitTimeInt::InitSaveState()


/*----------------------------------------------------------------------*
 | Saves and backs up the current state.                                |
 |                                                                      |
 |  This is currently needed for stronly coupling 3D-0D fields          |
 |  example:                                                            |
 |  saved_pn_   =  pn_                                                  |
 |  saved_qn_   =  qn_                                                  |
 |                                                                      |
 |                                                          ismail 04/14|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::SaveState()
{
  // save pressure vectors
  saved_pnm_->Update(1.0, *pnm_, 0.0);
  saved_pn_->Update(1.0, *pn_, 0.0);
  saved_pnp_->Update(1.0, *pnp_, 0.0);

  // save inflow rate vectors
  saved_qin_nm_->Update(1.0, *qin_nm_, 0.0);
  saved_qin_n_->Update(1.0, *qin_n_, 0.0);
  saved_qin_np_->Update(1.0, *qin_np_, 0.0);

  // save outflow rate vectors
  saved_qout_nm_->Update(1.0, *qout_nm_, 0.0);
  saved_qout_n_->Update(1.0, *qout_n_, 0.0);
  saved_qout_np_->Update(1.0, *qout_np_, 0.0);

  // save trajectory vectors
  saved_x_n_->Update(1.0, *x_n_, 0.0);
  saved_x_np_->Update(1.0, *x_np_, 0.0);

  // save acinar volume vectors
  saved_acini_e_volumenm_->Update(1.0, *acini_e_volumenm_, 0.0);
  saved_acini_e_volumen_->Update(1.0, *acini_e_volumen_, 0.0);
  saved_acini_e_volumenp_->Update(1.0, *acini_e_volumenp_, 0.0);

  // save element volume vectors
  saved_elemVolumenm_->Update(1.0, *elemVolumenm_, 0.0);
  saved_elemVolumen_->Update(1.0, *elemVolumen_, 0.0);
  saved_elemVolumenp_->Update(1.0, *elemVolumenp_, 0.0);

  if (solveScatra_)
  {
    // save nodal O2 concentration
    saved_scatraO2nm_->Update(1.0, *scatraO2nm_, 0.0);
    saved_scatraO2n_->Update(1.0, *scatraO2n_, 0.0);
    saved_scatraO2np_->Update(1.0, *scatraO2np_, 0.0);

    // save element inlet O2 concentration
    saved_e1scatraO2nm_->Update(1.0, *e1scatraO2nm_, 0.0);
    saved_e1scatraO2n_->Update(1.0, *e1scatraO2n_, 0.0);
    saved_e1scatraO2np_->Update(1.0, *e1scatraO2np_, 0.0);

    // save element outlet O2 concentration
    saved_e2scatraO2nm_->Update(1.0, *e2scatraO2nm_, 0.0);
    saved_e2scatraO2n_->Update(1.0, *e2scatraO2n_, 0.0);
    saved_e2scatraO2np_->Update(1.0, *e2scatraO2np_, 0.0);
  }

  return;
}  // RedAirwayImplicitTimeInt::SaveState


/*----------------------------------------------------------------------*
 | Loads backed up states.                                              |
 |                                                                      |
 |  This is currently needed for stronly coupling 3D-0D fields          |
 |  example:                                                            |
 |  pn_   =  saved_pn_                                                  |
 |  qn_   =  saved_qn_                                                  |
 |                                                                      |
 |                                                          ismail 04/14|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::LoadState()
{
  // save pressure vectors
  pnm_->Update(1.0, *saved_pnm_, 0.0);
  pn_->Update(1.0, *saved_pn_, 0.0);
  pnp_->Update(1.0, *saved_pnp_, 0.0);

  // save inflow rate vectors
  qin_nm_->Update(1.0, *saved_qin_nm_, 0.0);
  qin_n_->Update(1.0, *saved_qin_n_, 0.0);
  qin_np_->Update(1.0, *saved_qin_np_, 0.0);

  // save outflow rate vectors
  qout_nm_->Update(1.0, *saved_qout_np_, 0.0);
  qout_n_->Update(1.0, *saved_qout_n_, 0.0);
  qout_np_->Update(1.0, *saved_qout_np_, 0.0);

  // save trajectory vectors
  x_n_->Update(1.0, *saved_x_n_, 0.0);
  x_np_->Update(1.0, *saved_x_np_, 0.0);

  // save acinar volume vectors
  acini_e_volumenm_->Update(1.0, *saved_acini_e_volumenm_, 0.0);
  acini_e_volumen_->Update(1.0, *saved_acini_e_volumen_, 0.0);
  acini_e_volumenp_->Update(1.0, *saved_acini_e_volumenp_, 0.0);

  // save element volume vectors
  elemVolumenm_->Update(1.0, *saved_elemVolumenm_, 0.0);
  elemVolumen_->Update(1.0, *saved_elemVolumen_, 0.0);
  elemVolumenp_->Update(1.0, *saved_elemVolumenp_, 0.0);

  if (solveScatra_)
  {
    // save nodal O2 concentration
    scatraO2nm_->Update(1.0, *saved_scatraO2nm_, 0.0);
    scatraO2n_->Update(1.0, *saved_scatraO2n_, 0.0);
    scatraO2np_->Update(1.0, *saved_scatraO2np_, 0.0);

    // save element inlet O2 concentration
    e1scatraO2nm_->Update(1.0, *saved_e1scatraO2nm_, 0.0);
    e1scatraO2n_->Update(1.0, *saved_e1scatraO2n_, 0.0);
    e1scatraO2np_->Update(1.0, *saved_e1scatraO2np_, 0.0);

    // save element outlet O2 concentration
    e2scatraO2nm_->Update(1.0, *saved_e2scatraO2nm_, 0.0);
    e2scatraO2n_->Update(1.0, *saved_e2scatraO2n_, 0.0);
    e2scatraO2np_->Update(1.0, *saved_e2scatraO2np_, 0.0);
  }

  return;
}  // RedAirwayImplicitTimeInt::LoadState


/*----------------------------------------------------------------------*
 | Output of solution vector to binio                       ismail 07/09|
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::Output(
    bool CoupledTo3D, Teuchos::RCP<Teuchos::ParameterList> CouplingParams)
{
  int step = 0;
  int upres = 0;
  int uprestart = 0;
  double time_backup = 0.0;

  // If coupled to 3D problem, then get the export information from
  // the 3D problem
  if (CoupledTo3D)
  {
    step_ = CouplingParams->get<int>("step");
    upres_ = CouplingParams->get<int>("upres");
    uprestart_ = CouplingParams->get<int>("uprestart");
    time_ = CouplingParams->get<double>("time");
  }

  if (step_ % upres_ == 0)
  {
    // step number and time
    output_.NewStep(step_, time_);

    // "pressure" vectors
    output_.WriteVector("pnm", pnm_);
    output_.WriteVector("pn", pn_);
    output_.WriteVector("pnp", pnp_);
    output_.WriteVector("p_nonlin", p_nonlin_);

    if (solveScatra_)
    {
      output_.WriteVector("scatraO2np", scatraO2np_);
      output_.WriteVector("scatraO2n", scatraO2n_);
      output_.WriteVector("scatraO2nm", scatraO2nm_);
      output_.WriteVector("dVO2", dVolumeO2_);
      {
        // Export PO2
        // create the parameters for the discretization
        Teuchos::ParameterList eleparams;

        // note: We use an RCP because ParameterList wants something printable and comparable
        DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();
        // action for elements
        evaluation_data.elemVolumenp = elemVolumenp_;
        eleparams.set("action", "eval_PO2_from_concentration");

        const Epetra_Map* dofrowmap = discret_->DofRowMap();
        Teuchos::RCP<Epetra_Vector> po2 = CORE::LINALG::CreateVector(*dofrowmap, true);
        discret_->ClearState();

        evaluation_data.po2 = po2;
        discret_->SetState("scatranp", scatraO2np_);
        evaluation_data.acinar_vnp = acini_e_volumenp_;

        discret_->Evaluate(
            eleparams, sysmat_, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
        discret_->ClearState();
        output_.WriteVector("PO2", po2);
      }
      // Export acinar PO2
      {
        // create the parameters for the discretization
        Teuchos::ParameterList eleparams;
        // note: We use an RCP because ParameterList wants something printable and comparable
        DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();
        // action for elements
        evaluation_data.elemVolumenp = elemVolumenp_;
        eleparams.set("action", "eval_PO2_from_concentration");

        const Epetra_Map* dofrowmap = discret_->DofRowMap();
        Teuchos::RCP<Epetra_Vector> po2 = CORE::LINALG::CreateVector(*dofrowmap, true);
        discret_->ClearState();

        evaluation_data.po2 = po2;
        discret_->SetState("scatranp", acinarDO2_);
        evaluation_data.acinar_vnp = acini_e_volumenp_;

        discret_->Evaluate(
            eleparams, sysmat_, Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);
        discret_->ClearState();
        output_.WriteVector("AcinarPO2", po2);
      }
      {
        Epetra_Export exporter(e1scatraO2np_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2np_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2np", qexp_);
      {
        Epetra_Export exporter(e1scatraO2n_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2n_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2n", qexp_);
      {
        Epetra_Export exporter(e1scatraO2nm_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2nm_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2nm", qexp_);

      {
        Epetra_Export exporter(e2scatraO2np_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2np_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2np", qexp_);
      {
        Epetra_Export exporter(e2scatraO2n_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2n_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2n", qexp_);
      {
        Epetra_Export exporter(e2scatraO2nm_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2nm_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2nm", qexp_);
      {
        Epetra_Export exporter(junctionVolumeInMix_->Map(), jVDofRowMix_->Map());
        int err = jVDofRowMix_->Export(*junctionVolumeInMix_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("juncVolMix", jVDofRowMix_);
    }

    // write the flow values
    CORE::LINALG::Export(*qin_nm_, *qexp_);
    output_.WriteVector("qin_nm", qexp_);
    CORE::LINALG::Export(*qin_n_, *qexp_);
    output_.WriteVector("qin_n", qexp_);
    CORE::LINALG::Export(*qin_np_, *qexp_);
    output_.WriteVector("qin_np", qexp_);

    CORE::LINALG::Export(*qout_nm_, *qexp_);
    output_.WriteVector("qout_nm", qexp_);
    CORE::LINALG::Export(*qout_n_, *qexp_);
    output_.WriteVector("qout_n", qexp_);
    CORE::LINALG::Export(*qout_np_, *qexp_);
    output_.WriteVector("qout_np", qexp_);

    CORE::LINALG::Export(*x_n_, *qexp_);
    output_.WriteVector("x_n", qexp_);
    CORE::LINALG::Export(*x_np_, *qexp_);
    output_.WriteVector("x_np", qexp_);
    CORE::LINALG::Export(*open_, *qexp_);
    output_.WriteVector("open", qexp_);
    CORE::LINALG::Export(*p_extnp_, *qexp_);
    output_.WriteVector("p_extnp", qexp_);
    CORE::LINALG::Export(*p_extn_, *qexp_);
    output_.WriteVector("p_extn", qexp_);
    CORE::LINALG::Export(*airway_acinus_dep_, *qexp_);
    output_.WriteVector("airway_acinus_dep", qexp_);

    CORE::LINALG::Export(*elemVolumenm_, *qexp_);
    output_.WriteVector("elemVolumenm", qexp_);
    CORE::LINALG::Export(*elemVolumen_, *qexp_);
    output_.WriteVector("elemVolumen", qexp_);
    CORE::LINALG::Export(*elemVolumenp_, *qexp_);
    output_.WriteVector("elemVolumenp", qexp_);

    CORE::LINALG::Export(*elemRadiusnp_, *qexp_);
    output_.WriteVector("elemRadius_current", qexp_);

    {
      Epetra_Export exporter(acini_e_volumenm_->Map(), qexp_->Map());
      int err = qexp_->Export(*acini_e_volumenm_, exporter, Zero);
      if (err) dserror("Export using exporter returned err=%d", err);
      output_.WriteVector("acini_vnm", qexp_);
    }
    {
      Epetra_Export exporter(acini_e_volumen_->Map(), qexp_->Map());
      int err = qexp_->Export(*acini_e_volumen_, exporter, Zero);
      if (err) dserror("Export using exporter returned err=%d", err);
      output_.WriteVector("acini_vn", qexp_);
    }
    {
      Epetra_Export exporter(acini_e_volumenp_->Map(), qexp_->Map());
      int err = qexp_->Export(*acini_e_volumenp_, exporter, Zero);
      if (err) dserror("Export using exporter returned err=%d", err);
      output_.WriteVector("acini_vnp", qexp_);
    }
    {
      Epetra_Export exporter(acini_e_volume_strain_->Map(), qexp_->Map());
      int err = qexp_->Export(*acini_e_volume_strain_, exporter, Zero);
      if (err) dserror("Export using exporter returned err=%d", err);
      output_.WriteVector("acini_volumetric_strain", qexp_);
    }
    {
      Epetra_Export exporter(acini_e_volume0_->Map(), qexp_->Map());
      int err = qexp_->Export(*acini_e_volume0_, exporter, Zero);
      if (err) dserror("Export using exporter returned err=%d", err);
      output_.WriteVector("acini_v0", qexp_);
    }

    if (step_ == upres_)
    {
      CORE::LINALG::Export(*elemVolume0_, *qexp_);
      output_.WriteVector("elemVolume0", qexp_);
      output_.WriteVector("NodeIDs", nodeIds_);
      output_.WriteVector("radii", radii_);
      CORE::LINALG::Export(*generations_, *qexp_);
      output_.WriteVector("generations", qexp_);
      CORE::LINALG::Export(*acini_bc_, *qexp_);
      output_.WriteVector("acin_bc", qexp_);
      output_.WriteElementData(true);
      CORE::LINALG::Export(*elemArea0_, *qexp_);
      output_.WriteVector("elemArea0", qexp_);
    }

    // write mesh in each restart step --- the elements are required since
    // they contain history variables (the time dependent subscales)
    //    output_.WriteMesh(step_,time_);

    if (CoupledTo3D)
    {
      output_.WriteInt("Actual_RedD_step", step);
    }
  }
  // write restart also when uprestart_ is not a integer multiple of upres_
  else if (uprestart_ != 0 && step_ % uprestart_ == 0)
  {
    // step number and time
    output_.NewStep(step_, time_);

    // "pressure" vectors
    output_.WriteVector("pnm", pnm_);
    output_.WriteVector("pn", pn_);
    output_.WriteVector("pnp", pnp_);
    output_.WriteVector("p_nonlin", p_nonlin_);

    if (solveScatra_)
    {
      output_.WriteVector("scatraO2np", scatraO2np_);
      output_.WriteVector("scatraO2n", scatraO2n_);
      output_.WriteVector("scatraO2nm", scatraO2nm_);
      output_.WriteVector("dVO2", dVolumeO2_);
      {
        Epetra_Export exporter(e1scatraO2np_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2np_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2np", qexp_);
      {
        Epetra_Export exporter(e1scatraO2n_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2n_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2n", qexp_);
      {
        Epetra_Export exporter(e1scatraO2nm_->Map(), qexp_->Map());
        int err = qexp_->Export(*e1scatraO2nm_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e1scatraO2nm", qexp_);

      {
        Epetra_Export exporter(e2scatraO2np_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2np_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2np", qexp_);
      {
        Epetra_Export exporter(e2scatraO2n_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2n_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2n", qexp_);
      {
        Epetra_Export exporter(e2scatraO2nm_->Map(), qexp_->Map());
        int err = qexp_->Export(*e2scatraO2nm_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("e2scatraO2nm", qexp_);
      {
        Epetra_Export exporter(junctionVolumeInMix_->Map(), jVDofRowMix_->Map());
        int err = jVDofRowMix_->Export(*junctionVolumeInMix_, exporter, Zero);
        if (err) dserror("Export using exporter returned err=%d", err);
      }
      output_.WriteVector("juncVolMix", jVDofRowMix_);
    }
    // write the flow values
    CORE::LINALG::Export(*qin_nm_, *qexp_);
    output_.WriteVector("qin_nm", qexp_);
    CORE::LINALG::Export(*qin_n_, *qexp_);
    output_.WriteVector("qin_n", qexp_);
    CORE::LINALG::Export(*qin_np_, *qexp_);
    output_.WriteVector("qin_np", qexp_);
    //
    CORE::LINALG::Export(*qout_nm_, *qexp_);
    output_.WriteVector("qout_nm", qexp_);
    CORE::LINALG::Export(*qout_n_, *qexp_);
    output_.WriteVector("qout_n", qexp_);
    CORE::LINALG::Export(*qout_np_, *qexp_);
    output_.WriteVector("qout_np", qexp_);

    CORE::LINALG::Export(*x_n_, *qexp_);
    output_.WriteVector("x_n", qexp_);
    CORE::LINALG::Export(*x_np_, *qexp_);
    output_.WriteVector("x_np", qexp_);
    CORE::LINALG::Export(*open_, *qexp_);
    output_.WriteVector("open", qexp_);
    CORE::LINALG::Export(*p_extnp_, *qexp_);
    output_.WriteVector("p_extnp", qexp_);
    CORE::LINALG::Export(*p_extn_, *qexp_);
    output_.WriteVector("p_extn", qexp_);
    CORE::LINALG::Export(*airway_acinus_dep_, *qexp_);
    output_.WriteVector("airway_acinus_dep", qexp_);

    CORE::LINALG::Export(*elemVolumenm_, *qexp_);
    output_.WriteVector("elemVolumenm", qexp_);
    CORE::LINALG::Export(*elemVolumen_, *qexp_);
    output_.WriteVector("elemVolumen", qexp_);
    CORE::LINALG::Export(*elemVolumenp_, *qexp_);
    output_.WriteVector("elemVolumenp", qexp_);

    //
    CORE::LINALG::Export(*acini_e_volumenm_, *qexp_);
    output_.WriteVector("acini_vnm", qexp_);
    CORE::LINALG::Export(*acini_e_volumen_, *qexp_);
    output_.WriteVector("acini_vn", qexp_);
    CORE::LINALG::Export(*acini_e_volumenp_, *qexp_);
    output_.WriteVector("acini_vnp", qexp_);
    CORE::LINALG::Export(*acini_e_volume_strain_, *qexp_);
    output_.WriteVector("acini_volumetric_strain", qexp_);
    CORE::LINALG::Export(*acini_e_volume0_, *qexp_);
    output_.WriteVector("acini_v0", qexp_);

    // write mesh in each restart step --- the elements are required since
    // they contain history variables (the time dependent subscales)
    output_.WriteMesh(step_, time_);

    if (CoupledTo3D)
    {
      output_.WriteInt("Actual_RedD_step", step);
    }
  }

  // If coupled to 3D problem, then retrieve the old information of the
  // the reduced model problem
  if (CoupledTo3D)
  {
    step_ = step;
    upres_ = upres;
    uprestart_ = uprestart;
    time_ = time_backup;
  }
  return;
}  // RedAirwayImplicitTimeInt::Output

/*----------------------------------------------------------------------*
 | ReadRestart (public)                                     ismail 01/10|
 -----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::ReadRestart(int step, bool coupledTo3D)
{
  coupledTo3D_ = coupledTo3D;
  IO::DiscretizationReader reader(discret_, step);
  time_ = reader.ReadDouble("time");

  if (coupledTo3D_)
  {
    step_ = reader.ReadInt("Actual_RedD_step");
  }
  else
  {
    step_ = reader.ReadInt("step");
  }

  reader.ReadVector(pnp_, "pnp");
  reader.ReadVector(pn_, "pn");
  reader.ReadVector(pnm_, "pnm");
  reader.ReadVector(p_nonlin_, "p_nonlin");

  reader.ReadVector(qexp_, "acini_vnm");
  CORE::LINALG::Export(*qexp_, *acini_e_volumenm_);
  reader.ReadVector(qexp_, "acini_vn");
  CORE::LINALG::Export(*qexp_, *acini_e_volumen_);
  reader.ReadVector(qexp_, "acini_vnp");
  CORE::LINALG::Export(*qexp_, *acini_e_volumenp_);
  reader.ReadVector(qexp_, "acini_volumetric_strain");
  CORE::LINALG::Export(*qexp_, *acini_e_volume_strain_);
  reader.ReadVector(qexp_, "acini_v0");
  CORE::LINALG::Export(*qexp_, *acini_e_volume0_);

  reader.ReadVector(qexp_, "qin_nm");
  CORE::LINALG::Export(*qexp_, *qin_nm_);
  reader.ReadVector(qexp_, "qin_n");
  CORE::LINALG::Export(*qexp_, *qin_n_);
  reader.ReadVector(qexp_, "qin_np");
  CORE::LINALG::Export(*qexp_, *qin_np_);

  reader.ReadVector(qexp_, "qout_nm");
  CORE::LINALG::Export(*qexp_, *qout_nm_);
  reader.ReadVector(qexp_, "qout_n");
  CORE::LINALG::Export(*qexp_, *qout_n_);
  reader.ReadVector(qexp_, "qout_np");
  CORE::LINALG::Export(*qexp_, *qout_np_);

  reader.ReadVector(qexp_, "elemVolumenm");
  CORE::LINALG::Export(*qexp_, *elemVolumenm_);
  reader.ReadVector(qexp_, "elemVolumen");
  CORE::LINALG::Export(*qexp_, *elemVolumen_);
  reader.ReadVector(qexp_, "elemVolumenp");
  CORE::LINALG::Export(*qexp_, *elemVolumenp_);

  reader.ReadVector(qexp_, "x_n");
  CORE::LINALG::Export(*qexp_, *x_n_);
  reader.ReadVector(qexp_, "x_np");
  CORE::LINALG::Export(*qexp_, *x_np_);
  reader.ReadVector(qexp_, "open");
  CORE::LINALG::Export(*qexp_, *open_);
  reader.ReadVector(qexp_, "p_extn");
  CORE::LINALG::Export(*qexp_, *p_extn_);
  reader.ReadVector(qexp_, "p_extnp");
  CORE::LINALG::Export(*qexp_, *p_extnp_);
  reader.ReadVector(qexp_, "airway_acinus_dep");
  CORE::LINALG::Export(*qexp_, *airway_acinus_dep_);


  // read the previously written elements including the history data
  // reader.ReadMesh(step_);
  if (solveScatra_)
  {
    reader.ReadVector(scatraO2np_, "scatraO2np");
    reader.ReadVector(scatraO2n_, "scatraO2n");
    reader.ReadVector(scatraO2nm_, "scatraO2nm");

    reader.ReadVector(qexp_, "e1scatraO2np");
    CORE::LINALG::Export(*qexp_, *e1scatraO2np_);
    reader.ReadVector(qexp_, "e1scatraO2n");
    CORE::LINALG::Export(*qexp_, *e1scatraO2n_);
    reader.ReadVector(qexp_, "e1scatraO2nm");
    CORE::LINALG::Export(*qexp_, *e1scatraO2nm_);

    reader.ReadVector(qexp_, "e2scatraO2np");
    CORE::LINALG::Export(*qexp_, *e2scatraO2np_);
    reader.ReadVector(qexp_, "e2scatraO2n");
    CORE::LINALG::Export(*qexp_, *e2scatraO2n_);
    reader.ReadVector(qexp_, "e2scatraO2nm");
    CORE::LINALG::Export(*qexp_, *e2scatraO2nm_);

    reader.ReadVector(jVDofRowMix_, "juncVolMix");
    CORE::LINALG::Export(*jVDofRowMix_, *junctionVolumeInMix_);
  }

}  // RedAirwayImplicitTimeInt::ReadRestart


/*----------------------------------------------------------------------*
 | Create the field test for redairway field                 roth 10/13 |
 *----------------------------------------------------------------------*/
Teuchos::RCP<DRT::ResultTest> AIRWAY::RedAirwayImplicitTimeInt::CreateFieldTest()
{
  return Teuchos::rcp(new RedAirwayResultTest(*this));
}


/*----------------------------------------------------------------------*
 |                                                                      |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::EvalResidual(
    Teuchos::RCP<Teuchos::ParameterList> CouplingTo3DParams)
{
  residual_->PutScalar(0.0);

  // Call elements to calculate system matrix
  {
    // set both system matrix and rhs vector to zero
    sysmat_->Zero();
    rhs_->PutScalar(0.0);

    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // action for elements
    eleparams.set("action", "calc_sys_matrix_rhs");

    // set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);
    discret_->SetState("pn", pn_);
    discret_->SetState("pnm", pnm_);
    discret_->SetState("intr_ac_link", n_intr_ac_ln_);

    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;
    evaluation_data.qin_nm = qin_nm_;

    evaluation_data.qout_np = qout_np_;
    evaluation_data.qout_n = qout_n_;
    evaluation_data.qout_nm = qout_nm_;

    evaluation_data.x_np = x_np_;
    evaluation_data.x_n = x_n_;
    evaluation_data.open = open_;
    evaluation_data.airway_acinus_dep = airway_acinus_dep_;
    evaluation_data.p_extnp = p_extnp_;
    evaluation_data.p_extn = p_extn_;
    evaluation_data.compute_awacinter = compAwAcInter_;

    evaluation_data.elemVolumen = elemVolumen_;
    evaluation_data.elemVolumenp = elemVolumenp_;

    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    // get lung volume
    double lung_volume_np = 0.0;
    bool err = this->SumAllColElemVal(acini_e_volumenp_, acini_bc_, lung_volume_np);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    double lung_volume_n = 0.0;
    err = this->SumAllColElemVal(acini_e_volumen_, acini_bc_, lung_volume_n);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    double lung_volume_nm = 0.0;
    err = this->SumAllColElemVal(acini_e_volumenm_, acini_bc_, lung_volume_nm);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }

    evaluation_data.lungVolume_np = lung_volume_np;
    evaluation_data.lungVolume_n = lung_volume_n;
    evaluation_data.lungVolume_nm = lung_volume_nm;


    // call standard loop over all elements
    discret_->Evaluate(eleparams, sysmat_, rhs_);
    discret_->ClearState();

    // finalize the complete matrix
    discret_->ClearState();
  }

  // Solve the boundary conditions
  bcval_->PutScalar(0.0);
  dbctog_->PutScalar(0.0);
  // Solve terminal BCs
  {
    // create the parameters for the discretization
    Teuchos::ParameterList eleparams;

    // action for elements
    eleparams.set("action", "set_bc");

    // set vector values needed by elements
    discret_->ClearState();
    discret_->SetState("pnp", pnp_);
    discret_->SetState("pn", pn_);
    discret_->SetState("pnm", pnm_);
    //    discret_->SetState("qcnp",qcnp_);
    //    discret_->SetState("qcn" ,qcn_ );
    //    discret_->SetState("qcnm",qcnm_);
    // note: We use an RCP because ParameterList wants something printable and comparable
    DRT::REDAIRWAYS::EvaluationData& evaluation_data = DRT::REDAIRWAYS::EvaluationData::get();

    evaluation_data.acinar_vn = acini_e_volumen_;
    evaluation_data.acinar_vnp = acini_e_volumenp_;

    evaluation_data.qin_np = qin_np_;
    evaluation_data.qin_n = qin_n_;

    evaluation_data.qout_np = qout_np_;
    evaluation_data.qout_n = qout_n_;

    evaluation_data.bcval = bcval_;
    evaluation_data.dbctog = dbctog_;
    evaluation_data.dt = dta_;
    evaluation_data.time = time_;

    // Add the parameters to solve terminal BCs coupled to 3D fluid boundary
    eleparams.set("coupling with 3D fluid params", CouplingTo3DParams);

    // get lung volume
    double lung_volume_np = 0.0;
    bool err = this->SumAllColElemVal(acini_e_volumenp_, acini_bc_, lung_volume_np);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }
    double lung_volume_n = 0.0;
    err = this->SumAllColElemVal(acini_e_volumen_, acini_bc_, lung_volume_n);
    if (err)
    {
      dserror("Error by summing all acinar volumes");
    }
    evaluation_data.lungVolume_np = lung_volume_np;
    evaluation_data.lungVolume_n = lung_volume_n;


    // call standard loop over all elements
    discret_->Evaluate(eleparams, sysmat_, rhs_);
    discret_->ClearState();
  }

  // Apply the BCs to the system matrix and rhs
  {
    CORE::LINALG::ApplyDirichletToSystem(*sysmat_, *pnp_, *rhs_, *bcval_, *dbctog_);
  }

  // Evaluate Residual
  sysmat_->Multiply(false, *pnp_, *residual_);
  residual_->Update(-1.0, *rhs_, 1.0);
}  // EvalResidual


/*----------------------------------------------------------------------*
 | Destructor dtor (public)                                 ismail 01/10|
 *----------------------------------------------------------------------*/
AIRWAY::RedAirwayImplicitTimeInt::~RedAirwayImplicitTimeInt() { return; }


/*----------------------------------------------------------------------*
 |                                                                      |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::SetAirwayFluxFromTissue(Teuchos::RCP<Epetra_Vector> coupflux)
{
  const Epetra_BlockMap& condmap = coupflux->Map();

  for (int i = 0; i < condmap.NumMyElements(); ++i)
  {
    int condID = condmap.GID(i);
    DRT::Condition* cond = coupcond_[condID];
    std::vector<double> newval(1, 0.0);
    newval[0] = (*coupflux)[i];
    cond->Add("val", newval);
  }
}


/*----------------------------------------------------------------------*
 |                                                                      |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::SetupForCoupling()
{
  std::vector<DRT::Condition*> nodecond;
  discret_->GetCondition("RedAirwayPrescribedCond", nodecond);
  unsigned int numnodecond = nodecond.size();
  if (numnodecond == 0) dserror("no redairway prescribed conditions");

  std::vector<int> tmp;
  for (unsigned int i = 0; i < numnodecond; ++i)
  {
    DRT::Condition* actcond = nodecond[i];
    if (actcond->Type() == DRT::Condition::RedAirwayNodeTissue)
    {
      int condID = actcond->GetInt("coupling id");
      coupcond_[condID] = actcond;
      tmp.push_back(condID);
      pres_[condID] = 0.0;
    }
  }
  unsigned int numcond = tmp.size();
  if (numcond == 0) dserror("no coupling conditions found");
  coupmap_ = Teuchos::rcp(new Epetra_Map(tmp.size(), tmp.size(), tmp.data(), 0, discret_->Comm()));
}


/*----------------------------------------------------------------------*
 |                                                                      |
 *----------------------------------------------------------------------*/
void AIRWAY::RedAirwayImplicitTimeInt::ExtractPressure(Teuchos::RCP<Epetra_Vector> couppres)
{
  for (int i = 0; i < coupmap_->NumMyElements(); i++)
  {
    int condgid = coupmap_->GID(i);
    DRT::Condition* cond = coupcond_[condgid];
    const std::vector<int>* nodes = cond->Nodes();
    if (nodes->size() != 1)
      dserror("Too many nodes on coupling with tissue condition ID=[%d]\n", condgid);

    int gid = (*nodes)[0];
    double pressure = 0.0;
    if (discret_->HaveGlobalNode(gid))
    {
      DRT::Node* node = discret_->gNode(gid);
      if (myrank_ == node->Owner())
      {
        int giddof = discret_->Dof(node, 0);
        int liddof = pnp_->Map().LID(giddof);
        pressure = (*pnp_)[liddof];
      }
    }
    double parpres = 0.;
    discret_->Comm().SumAll(&pressure, &parpres, 1);
    (*couppres)[i] = parpres;
  }
}


/*----------------------------------------------------------------------*
 | Sum all ColElement values                                            |
 |                                                          ismail 11/12|
 *----------------------------------------------------------------------*/
bool AIRWAY::RedAirwayImplicitTimeInt::SumAllColElemVal(
    Teuchos::RCP<Epetra_Vector> vec, Teuchos::RCP<Epetra_Vector> sumCond, double& sum)
{
  // Check if the vector is a ColElement vector
  const Epetra_Map* elementcolmap = discret_->ElementColMap();
  if (!vec->Map().SameAs(*elementcolmap) && !sumCond->Map().SameAs(*elementcolmap))
  {
    return true;
  }

  // Since the acinar_volume vector is a ColMap, we first need to export
  // it to a RowMap and eliminate the ghosted values
  {
    // define epetra exporter
    Epetra_Export exporter(vec->Map(), qexp_->Map());
    // export from ColMap to RowMap
    int err = qexp_->Export(*vec, exporter, Zero);
    if (err) dserror("Export using exporter returned err=%d", err);

    Epetra_Export exporter2(sumCond->Map(), qexp2_->Map());
    // export from ColMap to RowMap
    err = qexp2_->Export(*sumCond, exporter2, Zero);
    if (err) dserror("Export using exporter returned err=%d", err);
  }

  // Get the mean acinar volume on the current processor
  qexp_->Dot(*qexp2_, &sum);

  // return all is fine
  return false;
}