/*----------------------------------------------------------------------*/
/*!
 \file porofluidmultiphase_timint_implicit.cpp

 \brief base class of implicit integration schemes for porous multiphase
        flow problems

   \level 3

   \maintainer  Anh-Tu Vuong
                vuong@lnm.mw.tum.de
                http://www.lnm.mw.tum.de
                089 - 289-15251
 *----------------------------------------------------------------------*/


#include "porofluidmultiphase_timint_implicit.H"
#include "porofluidmultiphase_utils.H"

#include "../drt_porofluidmultiphase/porofluidmultiphase_resulttest.H"


#include "../drt_porofluidmultiphase_ele/porofluidmultiphase_ele_action.H"

#include "../linalg/linalg_solver.H"
#include "../linalg/linalg_utils.H"

#include "../drt_lib/drt_globalproblem.H"
#include "../drt_lib/drt_assemblestrategy.H"

#include "../drt_inpar/drt_validparameters.H"

#include "../drt_io/io.H"
#include "../drt_io/io_control.H"
#include "../drt_io/io_gmsh.H"

#include <Teuchos_TimeMonitor.hpp>

/*==========================================================================*/
// Constructors and destructors and related methods
/*==========================================================================*/

/*----------------------------------------------------------------------*
 | constructor                                     (public) vuong 08/16 |
 *----------------------------------------------------------------------*/
POROFLUIDMULTIPHASE::TimIntImpl::TimIntImpl(
    Teuchos::RCP<DRT::Discretization>      actdis,
    const int                              linsolvernumber,
    const Teuchos::ParameterList&          probparams,
    const Teuchos::ParameterList&          poroparams,
    FILE*                                  errfile,
    Teuchos::RCP<IO::DiscretizationWriter> output) :
  // call constructor for "nontrivial" objects
  solver_ (Teuchos::null),
  linsolvernumber_(linsolvernumber),
  params_ (probparams),
  poroparams_ (poroparams),
  myrank_ (actdis->Comm().MyPID()),
  errfile_  (errfile),
  isale_    (false),
//  outmean_  (DRT::INPUT::IntegralValue<int>(*params,"OUTMEAN")),
  outmean_  (false),
  calcerr_(DRT::INPUT::IntegralValue<INPAR::POROFLUIDMULTIPHASE::CalcError>(poroparams_,"CALCERROR")),
  fluxrecon_(DRT::INPUT::IntegralValue<INPAR::POROFLUIDMULTIPHASE::FluxReconstructionMethod>(poroparams_,"FLUX_PROJ_METHOD")),
  fluxreconsolvernum_(poroparams_.get<int>("FLUX_PROJ_SOLVER")),
  divcontype_(DRT::INPUT::IntegralValue<INPAR::POROFLUIDMULTIPHASE::DivContAct>(poroparams_,"DIVERCONT")),
  fdcheck_(DRT::INPUT::IntegralValue<INPAR::POROFLUIDMULTIPHASE::FDCheck>(poroparams_,"FDCHECK")),
  fdcheckeps_(poroparams_.get<double>("FDCHECKEPS")),
  fdchecktol_(poroparams_.get<double>("FDCHECKTOL")),
  time_   (0.0),
  maxtime_  (params_.get<double>("MAXTIME")),
  step_   (0),
  stepmax_  (params_.get<int>("NUMSTEP")),
  dt_      (params_.get<double>("TIMESTEP")),
  dtele_(0.0),
  dtsolve_(0.0),
  iternum_(0),
  itemax_(poroparams_.get<int>("ITEMAX")),
  nsd_(DRT::Problem::Instance()->NDim()),
  // Initialization of degrees of freedom variables
  phin_(Teuchos::null),
  phinp_(Teuchos::null),
  phidtn_(Teuchos::null),
  phidtnp_(Teuchos::null),
  hist_(Teuchos::null),
  pressure_(Teuchos::null),
  saturation_(Teuchos::null),
  solidpressure_(Teuchos::null),
  flux_(Teuchos::null),
  nds_disp_(-1),
  nds_vel_(-1),
  nds_solidpressure_(-1),
  discret_(actdis),
  output_ (output),
  sysmat_(Teuchos::null),
  zeros_(Teuchos::null),
  dbcmaps_(Teuchos::null),
  neumann_loads_(Teuchos::null),
  residual_(Teuchos::null),
  trueresidual_(Teuchos::null),
  increment_(Teuchos::null),
  prei_(Teuchos::null),
  upres_    (params_.get<int>("RESULTSEVRY")),
  uprestart_(params_.get<int>("RESTARTEVRY"))
{
  return;
}


/*------------------------------------------------------------------------*
 | initialize time integration                                vuong 08/16 |
 *------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::Init(
    bool isale,
    int nds_disp,
    int nds_vel,
    int nds_solidpressure)
{
  //set flags
  isale_             = isale;
  nds_disp_          = nds_disp;
  nds_vel_           = nds_vel;
  nds_solidpressure_ = nds_solidpressure;

  // make sure the values make sense
  // -1 is the default value, meaning that there is no coupling
  if(nds_disp_!=-1)
    if( nds_disp_<0 or nds_disp_>discret_->NumDofSets()-1 )
      dserror("invalid number of dofset for mesh displacements!");

  // make sure the values make sense
  // -1 is the default value, meaning that there is no coupling
  if(nds_vel_!=-1)
    if( nds_vel_<0 or nds_vel_>discret_->NumDofSets()-1 )
      dserror("invalid number of dofset for mesh velocities!");

  // make sure the values make sense
  // there has to be a valid number for the solid pressure in all cases
  if( nds_solidpressure_<0 or nds_solidpressure_>discret_->NumDofSets()-1 )
    dserror("invalid number of dofset for solid pressure!");

  // -------------------------------------------------------------------
  // create a solver
  // -------------------------------------------------------------------
  solver_ =
    Teuchos::rcp(new LINALG::Solver(DRT::Problem::Instance()->SolverParams(linsolvernumber_),
                                    discret_->Comm(),
                                    errfile_));
  discret_->ComputeNullSpaceIfNecessary(solver_->Params());

  // ensure that degrees of freedom in the discretization have been set
  if ((not discret_->Filled()) or (not discret_->HaveDofs()))
    discret_->FillComplete();

  // -------------------------------------------------------------------
  // get a vector layout from the discretization to construct matching
  // vectors and matrices: local <-> global dof numbering
  // -------------------------------------------------------------------
  const Epetra_Map* dofrowmap = discret_->DofRowMap();

  // -------------------------------------------------------------------
  // create empty system matrix (27 adjacent nodes as 'good' guess)
  // -------------------------------------------------------------------
  sysmat_ = Teuchos::rcp(new LINALG::SparseMatrix(*(discret_->DofRowMap()),27,false,true));

  // -------------------------------------------------------------------
  // create vectors containing problem variables
  // -------------------------------------------------------------------
  // solutions at time n+1
  phinp_ = LINALG::CreateVector(*dofrowmap,true);
  // solutions at time n
  phin_ = LINALG::CreateVector(*dofrowmap,true);
  // time derivative of solutions at time n
  phidtn_ = LINALG::CreateVector(*dofrowmap,true);
  // time derivative of solutions at time n+1
  phidtnp_ = LINALG::CreateVector(*dofrowmap,true);

  // history vector
  hist_ = LINALG::CreateVector(*dofrowmap,true);

  // pressure at time n+1
  pressure_ = LINALG::CreateVector(*dofrowmap,true);
  // saturation at time n+1
  saturation_ = LINALG::CreateVector(*dofrowmap,true);
  // solid pressure at time n+1
  solidpressure_ = LINALG::CreateVector(*discret_->DofRowMap(nds_solidpressure_),true);

  // -------------------------------------------------------------------
  // create vectors associated to boundary conditions
  // -------------------------------------------------------------------
  // a vector of zeros to be used to enforce zero dirichlet boundary conditions
  zeros_ = LINALG::CreateVector(*dofrowmap,true);

  // object holds maps/subsets for DOFs subjected to Dirichlet BCs and otherwise
  dbcmaps_ = Teuchos::rcp(new LINALG::MapExtractor());
  {
    Teuchos::ParameterList eleparams;
    // other parameters needed by the elements
    eleparams.set("total time",time_);
    discret_->EvaluateDirichlet(eleparams, zeros_, Teuchos::null, Teuchos::null,
                                Teuchos::null, dbcmaps_);
    zeros_->PutScalar(0.0); // just in case of change
  }

  // -------------------------------------------------------------------
  // create vectors associated to solution process
  // -------------------------------------------------------------------
  // the vector containing body and surface forces
  neumann_loads_= LINALG::CreateVector(*dofrowmap,true);

  // the residual vector --- more or less the rhs
  residual_ = LINALG::CreateVector(*dofrowmap,true);

  // residual vector containing the normal boundary fluxes
  trueresidual_ = LINALG::CreateVector(*dofrowmap,true);

  // incremental solution vector
  increment_ = LINALG::CreateVector(*dofrowmap,true);

  // iterative pressure increments Incp_{n+1}
  // also known as residual pressures
  prei_ = LINALG::CreateVector(*dofrowmap, true);

  // -------------------------------------------------------------------
  // set initial field
  // -------------------------------------------------------------------
  SetInitialField(
      DRT::INPUT::IntegralValue<INPAR::POROFLUIDMULTIPHASE::InitialField>(
          poroparams_, "INITIALFIELD"), poroparams_.get<int>("INITFUNCNO"));

  // -------------------------------------------------------------------
  // set element parameters
  // -------------------------------------------------------------------
  SetElementGeneralParameters();

  return;
} // TimIntImpl::Init()

/*----------------------------------------------------------------------*
 | Destructor dtor                                 (public) vuong 08/16 |
 *----------------------------------------------------------------------*/
POROFLUIDMULTIPHASE::TimIntImpl::~TimIntImpl()
{
  return;
}


/*========================================================================*/
//! set element parameters
/*========================================================================*/

/*----------------------------------------------------------------------*
 | set all general parameters for element                   vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::SetElementGeneralParameters() const
{
  Teuchos::ParameterList eleparams;

  eleparams.set<int>("action",POROFLUIDMULTIPHASE::set_general_parameter);

  eleparams.set<bool>("isale",isale_);
  eleparams.set<int>("nds_disp",nds_disp_);
  eleparams.set<int>("nds_vel",nds_vel_);
  eleparams.set<int>("nds_solidpressure",nds_solidpressure_);

  eleparams.set<bool>("using generalized-alpha time integration",false);
  eleparams.set<bool>("using stationary formulation",false);
  eleparams.set<double>("alpha_F",1.0);

  // call standard loop over elements
  discret_->Evaluate(eleparams,Teuchos::null,Teuchos::null,Teuchos::null,Teuchos::null,Teuchos::null);

  return;
}


/*==========================================================================*/
// general framework
/*==========================================================================*/

/*--- set, prepare, and predict --------------------------------------------*/

/*----------------------------------------------------------------------*
 | prepare time loop                                        vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::PrepareTimeLoop()
{
  // provide information about initial field (do not do for restarts!)
  if(step_ == 0)
  {
    // compute pressure and saturations from initial state
    ReconstructPressuresAndSaturations();

    // compute velocities from initial state
    ReconstructFlux();

    // write out initial state
    Output();

    // compute error for problems with analytical solution (initial field!)
    EvaluateErrorComparedToAnalyticalSol();
  }

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrepareTimeLoop


/*----------------------------------------------------------------------*
 | setup the variables to do a new time step       (public) vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::PrepareTimeStep()
{
  // time measurement: prepare time step
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:    + prepare time step");

  // -------------------------------------------------------------------
  //                       initialization
  // -------------------------------------------------------------------
  if (step_ == 0)
    PrepareFirstTimeStep();

  // -------------------------------------------------------------------
  //              set time dependent parameters
  // -------------------------------------------------------------------
  // note the order of the following three functions is important
  IncrementTimeAndStep();

  // -------------------------------------------------------------------
  // set part of the rhs vector belonging to the old timestep
  // -------------------------------------------------------------------
  SetOldPartOfRighthandside();
  // reset every parameter that potentially changes for every time step
  SetElementTimeStepParameter();

  // -------------------------------------------------------------------
  //         evaluate Dirichlet and Neumann boundary conditions
  // -------------------------------------------------------------------
  // TODO: Dirichlet auch im Fall von genalpha prenp
  // Neumann(n + alpha_f)
  ApplyDirichletBC(time_,phinp_,Teuchos::null);
  ApplyNeumannBC(neumann_loads_);

  return;
} // TimIntImpl::PrepareTimeStep


/*------------------------------------------------------------------------------*
 | initialization procedure prior to evaluation of first time step  vuong 08/16 |
 *------------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::PrepareFirstTimeStep()
{
  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrepareFirstTimeStep


/*----------------------------------------------------------------------*
 | contains the time loop                                   vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::TimeLoop()
{
  // time measurement: time loop
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:  + time loop");

  // prepare time loop
  PrepareTimeLoop();

  while ((step_<stepmax_) and ((time_+ EPS12) < maxtime_))
  {
    // -------------------------------------------------------------------
    // prepare time step
    // -------------------------------------------------------------------
    PrepareTimeStep();

    // -------------------------------------------------------------------
    //                  solve nonlinear / linear equation
    // -------------------------------------------------------------------
    Solve();

    // -------------------------------------------------------------------
    //                         update solution
    //        current solution becomes old solution of next timestep
    // -------------------------------------------------------------------
    Update();

    // -------------------------------------------------------------------
    // evaluate error for problems with analytical solution
    // -------------------------------------------------------------------
    if(calcerr_!= INPAR::POROFLUIDMULTIPHASE::calcerror_no)
      EvaluateErrorComparedToAnalyticalSol();

    // -------------------------------------------------------------------
    //                         output of solution
    // -------------------------------------------------------------------
    Output();

  } // while

  // print the results of time measurements
  Teuchos::TimeMonitor::summarize();

  return;
} // TimIntImpl::TimeLoop


/*----------------------------------------------------------------------*
 | contains the call of linear/nonlinear solver             vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::Solve()
{
  // -----------------------------------------------------------------
  //                    always solve nonlinear equation
  // -----------------------------------------------------------------
  NonlinearSolve();

  // reconstruct pressures and saturations
  ReconstructPressuresAndSaturations();

  // reconstruct velocities
  ReconstructFlux();

  return;
}


/*----------------------------------------------------------------------*
 | apply moving mesh data                                     vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ApplyMeshMovement(
    Teuchos::RCP<const Epetra_Vector> dispnp
)
{
  //---------------------------------------------------------------------------
  // only required in ALE case
  //---------------------------------------------------------------------------
  if (isale_)
  {
    TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE: apply mesh movement");

    if(nds_disp_==-1) dserror("Dof set number of displacment related dofs"
        " has not been set!");

    // check existence of displacement vector
    if (dispnp == Teuchos::null) dserror("Got null pointer for displacements!");

    // provide POROFLUIDMULTIPHASE discretization with displacement field
    discret_->SetState(nds_disp_,"dispnp",dispnp);
  } // if (isale_)

  return;
} // TimIntImpl::ApplyMeshMovement


/*----------------------------------------------------------------------*
 |  print information about current time step to screen     vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::PrintTimeStepInfo()
{
  if (myrank_==0)
    printf("TIME: %11.4E/%11.4E  DT = %11.4E  Stationary  STEP = %4d/%4d \n",
           time_,maxtime_,dt_,step_,stepmax_);
} // POROFLUIDMULTIPHASE::TimIntImpl::PrintTimeStepInfo


/*----------------------------------------------------------------------*
 | output of solution vector to BINIO                       vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::Output()
{
  // time measurement: output of solution
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:    + output of solution");

  // solution output and potentially restart data and/or flux data
  if (DoOutput())
  {
    // step number and time (only after that data output is possible)
    output_->NewStep(step_,time_);

    // write domain decomposition for visualization (only once at step "upres"!)
    if (step_==upres_) output_->WriteElementData(true);

    // write state vectors
    OutputState();

    // add restart data
    if (step_%uprestart_==0 and step_ !=0) OutputRestart();
  }

  return;
} // TimIntImpl::Output

/*----------------------------------------------------------------------*
 | output of solution vector to BINIO                       vuong 08/16 |
 *----------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Map> POROFLUIDMULTIPHASE::TimIntImpl::DofRowMap(unsigned nds) const
{
  return Teuchos::rcp(discret_->DofRowMap(nds),false);
}


/*==========================================================================*
 |                                                                          |
 | protected:                                                               |
 |                                                                          |
 *==========================================================================*/

/*==========================================================================*/
// general framework
/*==========================================================================*/


/*----------------------------------------------------------------------*
 | evaluate Dirichlet boundary conditions at t_{n+1}        vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ApplyDirichletBC
(
  const double time,
  Teuchos::RCP<Epetra_Vector> prenp,
  Teuchos::RCP<Epetra_Vector> predt
)
{
  // time measurement: apply Dirichlet conditions
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:      + apply dirich cond.");

  // needed parameters
  Teuchos::ParameterList p;
  p.set("total time",time);  // actual time t_{n+1}

  // predicted Dirichlet values
  // \c  prenp then also holds prescribed new Dirichlet values
  discret_->ClearState();
  discret_->EvaluateDirichlet(p,prenp,predt,Teuchos::null,Teuchos::null,dbcmaps_);
  discret_->ClearState();

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::ApplyDirichletBC


/*----------------------------------------------------------------------*
 | contains the residual scaling and addition of Neumann terms          |
 |                                                          vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ScalingAndNeumann()
{
  // scaling to get true residual vector for all time integration schemes
  trueresidual_->Update(ResidualScaling(),*residual_,0.0);

  // add Neumann b.c. scaled with a factor due to time discretization
  AddNeumannToResidual();

  return;
} // TimIntImpl::ScalingAndNeumann


/*----------------------------------------------------------------------*
 | evaluate Neumann boundary conditions                     vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ApplyNeumannBC
(
    const Teuchos::RCP<Epetra_Vector>&   neumann_loads //!< Neumann loads
)
{
  // prepare load vector
  neumann_loads->PutScalar(0.0);

  // create parameter list
  Teuchos::ParameterList condparams;

  // action for elements
  condparams.set<int>("action",POROFLUIDMULTIPHASE::bd_calc_Neumann);

  // set time for evaluation of point Neumann conditions as parameter depending on time integration scheme
  // line/surface/volume Neumann conditions use the time stored in the time parameter class
  SetTimeForNeumannEvaluation(condparams);

  // evaluate Neumann boundary conditions at time t_{n+alpha_F} (generalized alpha) or time t_{n+1} (otherwise)
  discret_->EvaluateNeumann(condparams,*neumann_loads);
  discret_->ClearState();

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::ApplyNeumannBC


/*----------------------------------------------------------------------*
 | contains the assembly process for matrix and rhs         vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::AssembleMatAndRHS()
{
  // time measurement: element calls
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:       + element calls");

  // get cpu time
  const double tcpuele = Teuchos::Time::wallTime();

  // zero out matrix entries
  sysmat_->Zero();

  // reset the residual vector
  residual_->PutScalar(0.0);

  // create parameter list for elements
  Teuchos::ParameterList eleparams;

  // action for elements
  eleparams.set<int>("action",POROFLUIDMULTIPHASE::calc_mat_and_rhs);

  // clean up, just in case ...
  discret_->ClearState();

  // set vector values needed by elements
  // add state vectors according to time-integration scheme
  AddTimeIntegrationSpecificVectors();

  // call loop over elements (with or without subgrid-diffusivity(-scaling) vector)
  discret_->Evaluate(eleparams,sysmat_,residual_);

  // clean up
  discret_->ClearState();

  // potential residual scaling and potential addition of Neumann terms
  ScalingAndNeumann();

  // finalize assembly of system matrix
  sysmat_->Complete();

  // end time measurement for element
  dtele_=Teuchos::Time::wallTime()-tcpuele;

  return;
} // TimIntImpl::AssembleMatAndRHS


/*----------------------------------------------------------------------*
 | contains the nonlinear iteration loop                    vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::NonlinearSolve()
{
  // time measurement: nonlinear iteration
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:   + nonlin. iteration/lin. solve");

  // out to screen
  PrintTimeStepInfo();

  // print header of convergence table to screen
  PrintConvergenceHeader();

  // ---------------------------------------------- nonlinear iteration
  // stop nonlinear iteration when increment-norm is below this bound
  const double  ittol = poroparams_.get<double>("CONVTOL");

  //------------------------------ turn adaptive solver tolerance on/off
  const bool   isadapttol    = (DRT::INPUT::IntegralValue<int>(poroparams_,"ADAPTCONV"));
  const double adaptolbetter = poroparams_.get<double>("ADAPTCONV_BETTER");
  const double abstolres = poroparams_.get<double>("ABSTOLRES");
  double       actresidual(0.0);

  // prepare Newton-Raphson iteration
  iternum_ = 0;

  // start Newton-Raphson iteration
  while(true)
  {
    iternum_++;

    // call elements to calculate system matrix and rhs and assemble
    Evaluate();

    // Apply Dirichlet boundary conditions to system of equations
    // residual values are supposed to be zero at Dirichlet boundaries
    {
      // time measurement: application of DBC to system
      TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:       + apply DBC to system");

      LINALG::ApplyDirichlettoSystem(sysmat_,increment_,residual_,zeros_,*(dbcmaps_->CondMap()));
    }

    // abort nonlinear iteration if desired
    if (AbortNonlinIter(iternum_,itemax_,ittol,abstolres,actresidual))
      break;

    // initialize increment vector
    increment_->PutScalar(0.0);

    {
      // get cpu time
      const double tcpusolve=Teuchos::Time::wallTime();

      // time measurement: call linear solver
      TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE:       + call linear solver");

      // do adaptive linear solver tolerance (not in first solve)
      if (isadapttol && iternum_>1)
      {
        solver_->AdaptTolerance(ittol,actresidual,adaptolbetter);
      }

      // strategy_->Solve(solver_,sysmat_,increment_,residual_,phinp_,iternum_,projector_);
      solver_->Solve(sysmat_->EpetraOperator(),increment_,residual_,true,1,Teuchos::null);

      solver_->ResetTolerance();

      // end time measurement for solver
      dtsolve_=Teuchos::Time::wallTime()-tcpusolve;
    }

    //------------------------------------------------ update solution vector
    UpdateIter(increment_);

  } // nonlinear iteration

  return;
} // TimIntImpl::NonlinearSolve


/*----------------------------------------------------------------------*
 | check if to stop the nonlinear iteration                 vuong 08/16 |
 *----------------------------------------------------------------------*/
bool POROFLUIDMULTIPHASE::TimIntImpl::AbortNonlinIter(
    const int itnum,
    const int itemax,
    const double ittol,
    const double abstolres,
    double& actresidual)
{
  //----------------------------------------------------- compute norms
  double incprenorm_L2(0.0);

  double prenorm_L2(0.0);

  double preresnorm(0.0);

  double preresnorminf(0.0);

  // Calculate problem-specific norms
  CalcProblemSpecificNorm(preresnorm,incprenorm_L2,prenorm_L2,preresnorminf);

  // care for the case that nothing really happens in the pressure
  if (prenorm_L2 < 1e-5)
  {
    prenorm_L2 = 1.0;
  }

  //-------------------------------------------------- output to screen
  // special case of very first iteration step: solution increment is not yet available
  if (itnum == 1)
    // print first line of convergence table to screen
    PrintConvergenceValuesFirstIter(itnum,itemax,ittol,preresnorm,preresnorminf);

  // ordinary case later iteration steps: solution increment can be printed and convergence check should be done
  else
  {
    // print current line of convergence table to screen
    PrintConvergenceValues(itnum,itemax,ittol,preresnorm,incprenorm_L2,prenorm_L2,preresnorminf);

    // convergence check
    if (preresnorm <= ittol and incprenorm_L2/prenorm_L2 <= ittol )
    {
      // print finish line of convergence table to screen
      PrintConvergenceFinishLine();

      // write info to error file
      if (myrank_ == 0)
        if (errfile_!=NULL)
          fprintf(errfile_,"solve:   %3d/%3d  tol=%10.3E[L_2 ]  pres=%10.3E  pinc=%10.3E\n",
              itnum,itemax,ittol,preresnorm,incprenorm_L2/prenorm_L2);

      return true;
    }
  }

  // abort iteration, when there's nothing more to do! -> more robustness
  // absolute tolerance for deciding if residual is (already) zero
  // prevents additional solver calls that will not improve the residual anymore
  if ((preresnorm < abstolres))
  {
    // print finish line of convergence table to screen
    PrintConvergenceFinishLine();

    return true;
  }


  if ((itnum == itemax))
  {
    switch(divcontype_)
    {
    case INPAR::POROFLUIDMULTIPHASE::divcont_continue:
    {
      // warn if itemax is reached without convergence, but proceed to
      // next timestep...
      if (myrank_ == 0)
      {
        std::cout << "+---------------------------------------------------------------+" << std::endl;
        std::cout << "|            >>>>>> not converged in itemax steps!              |" << std::endl;
        std::cout << "+---------------------------------------------------------------+" << std::endl << std::endl;

        if (errfile_!=NULL)
        {
          fprintf(errfile_,"divergent solve continued:   %3d/%3d  tol=%10.3E[L_2 ]  pres=%10.3E  pinc=%10.3E\n",
              itnum,itemax,ittol,preresnorm,incprenorm_L2/prenorm_L2);
        }
      }
      break;
    }
    case INPAR::POROFLUIDMULTIPHASE::divcont_stop:
    {
      if (errfile_!=NULL)
      {
        fprintf(errfile_,"divergent solve stoped:   %3d/%3d  tol=%10.3E[L_2 ]  pres=%10.3E  pinc=%10.3E\n",
            itnum,itemax,ittol,preresnorm,incprenorm_L2/prenorm_L2);
      }
      dserror("Porofluid multiphase solver not converged in itemax steps!");
      break;
    }
    default:
      dserror("unknown divercont action!");
      break;
    }
    // yes, we stop the iteration
    return true;
  }

  // return the maximum residual value -> used for adaptivity of linear solver tolerance
  actresidual = std::max(preresnorm,incprenorm_L2/prenorm_L2);

  // check for INF's and NaN's before going on...
  if (std::isnan(incprenorm_L2) or
      std::isnan(prenorm_L2) or
      std::isnan(preresnorm))
    dserror("calculated vector norm is NaN.");

  if (std::isinf(incprenorm_L2) or
      std::isinf(prenorm_L2) or
      std::isinf(preresnorm))
    dserror("calculated vector norm is INF.");

  return false;
} // TimIntImpl::AbortNonlinIter


/*----------------------------------------------------------------------*
 | Calculate problem specific norm                          vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::CalcProblemSpecificNorm(
    double& preresnorm,
    double& incprenorm_L2,
    double& prenorm_L2,
    double& preresnorminf)
{
  residual_ ->Norm2(&preresnorm);
  increment_->Norm2(&incprenorm_L2);
  phinp_    ->Norm2(&prenorm_L2);
  residual_ ->NormInf(&preresnorminf);

  return;
}

/*----------------------------------------------------------------------------*
 | reconstruct pressures and saturation from current solution     vuong 08/16 |
 *--------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ReconstructPressuresAndSaturations()
{
  //reset
  pressure_->PutScalar(0.0);
  saturation_->PutScalar(0.0);

  // create parameter list for elements
  Teuchos::ParameterList eleparams;

  // action for elements
  eleparams.set<int>("action",POROFLUIDMULTIPHASE::calc_pres_and_sat);

  // set vector values needed by elements
  discret_->ClearState();

  // add state vectors according to time-integration scheme
  AddTimeIntegrationSpecificVectors();

  // initialize counter vector (will store how many times the node has been evaluated)
  Teuchos::RCP<Epetra_Vector> counter = LINALG::CreateVector(*discret_->DofRowMap(),true);;

  // call loop over elements
  discret_->Evaluate(eleparams, Teuchos::null, Teuchos::null, pressure_, saturation_, counter);

  discret_->ClearState();

  // dummy way: the values have been assembled too many times -> just divide by number of evaluations
  for(int i=0; i<discret_->DofRowMap()->NumMyElements();i++)
  {
    (*pressure_)[i]   *=  1.0/(*counter)[i];
    (*saturation_)[i] *=  1.0/(*counter)[i];
  }

  // reconstruct also the solid pressures
  ReconstructSolildPressures();

  return;
}

/*----------------------------------------------------------------------------*
 | reconstruct pressures from current solution     vuong 08/16 |
 *--------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ReconstructSolildPressures()
{
  //reset
  solidpressure_->PutScalar(0.0);

  // create parameter list for elements
  Teuchos::ParameterList eleparams;

  // action for elements
  eleparams.set<int>("action",POROFLUIDMULTIPHASE::calc_solidpressure);

  // set vector values needed by elements
  discret_->ClearState();

  // add state vectors according to time-integration scheme
  AddTimeIntegrationSpecificVectors();

  // initialize counter vector (will store how many times the node has been evaluated)
  Teuchos::RCP<Epetra_Vector> counter = LINALG::CreateVector(*discret_->DofRowMap(nds_solidpressure_),true);;

  // create strategy for assembly of solid pressure
  DRT::AssembleStrategy strategysolidpressure(
      nds_solidpressure_,
      0,
      Teuchos::null,
      Teuchos::null,
      solidpressure_,
      counter,
      Teuchos::null
      );

  // call loop over elements
  discret_->Evaluate(eleparams,strategysolidpressure);

  discret_->ClearState();

  // dummy way: the values have been assembled too many times -> just divide by number of evaluations
  for(int i=0; i<discret_->DofRowMap(nds_solidpressure_)->NumMyElements();i++)
  {
    (*solidpressure_)[i]   *=  1.0/(*counter)[i];
  }
}

/*----------------------------------------------------------------------------*
 | reconstruct velocicities from current solution                vuong 08/16 |
 *--------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::ReconstructFlux()
{
  if(fluxrecon_==INPAR::POROFLUIDMULTIPHASE::gradreco_none)
    return;

  // create the parameters for the discretization
  Teuchos::ParameterList eleparams;

  // action for elements
  eleparams.set<int>("action",POROFLUIDMULTIPHASE::recon_flux_at_nodes);

  const int dim = DRT::Problem::Instance()->NDim();
  // we assume same number of dofs per node in the whole dis here
  const int numphases = discret_->NumDof(discret_->lRowNode(0));
  const int numvec = numphases*dim;

  // add state vectors according to time-integration scheme
  AddTimeIntegrationSpecificVectors();

  switch(fluxrecon_)
  {
  case INPAR::POROFLUIDMULTIPHASE::gradreco_l2:
    flux_ =  DRT::UTILS::ComputeNodalL2Projection(
                    discret_,
                    "phinp",
                    numvec,
                    eleparams,
                    fluxreconsolvernum_);
    break;
  default:
    dserror("unknown method for recovery of fluxes!");
    break;
  }
}

/*----------------------------------------------------------------------*
 | print header of convergence table to screen              vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceHeader()
{
  if (myrank_ == 0)
    std::cout << "+------------+-------------------+--------------+--------------+------------------+\n"
             << "|- step/max -|- tol      [norm] -|-- pre-res ---|-- pre-inc ---|-- pre-res-inf ---|" << std::endl;

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceHeader


/*----------------------------------------------------------------------*
 | print first line of convergence table to screen          vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceValuesFirstIter(
    const int&              itnum,          //!< current Newton-Raphson iteration step
    const int&              itemax,         //!< maximum number of Newton-Raphson iteration steps
    const double&           ittol,          //!< relative tolerance for Newton-Raphson scheme
    const double&           preresnorm,     //!< L2 norm of pressure residual
    const double&           preresnorminf   //!< infinity norm of pressure residual
)
{
  if (myrank_ == 0)
    std::cout << "|  " << std::setw(3) << itnum << "/" << std::setw(3) << itemax << "   | "
             << std::setw(10) << std::setprecision(3) << std::scientific << ittol << "[L_2 ]  | "
             << std::setw(10) << std::setprecision(3) << std::scientific << preresnorm << "   |      --      | "
             << std::setw(10) << std::setprecision(3) << std::scientific << preresnorminf << "       | (      --     ,te="
             << std::setw(10) << std::setprecision(3) << std::scientific << dtele_ << ")" << std::endl;

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceValuesFirstIter


/*----------------------------------------------------------------------*
 | print current line of convergence table to screen        vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceValues(
    const int&              itnum,           //!< current Newton-Raphson iteration step
    const int&              itemax,          //!< maximum number of Newton-Raphson iteration steps
    const double&           ittol,           //!< relative tolerance for Newton-Raphson scheme
    const double&           preresnorm,      //!< L2 norm of pressure residual
    const double&           incprenorm_L2,   //!< L2 norm of pressure increment
    const double&           prenorm_L2,      //!< L2 norm of pressure state vector
    const double&           preresnorminf    //!< infinity norm of pressure residual
)
{
  if (myrank_ == 0)
    std::cout << "|  " << std::setw(3) << itnum << "/" << std::setw(3) << itemax << "   | "
             << std::setw(10) << std::setprecision(3) << std::scientific << ittol << "[L_2 ]  | "
             << std::setw(10) << std::setprecision(3) << std::scientific << preresnorm << "   | "
             << std::setw(10) << std::setprecision(3) << std::scientific << incprenorm_L2/prenorm_L2 << "   | "
             << std::setw(10) << std::setprecision(3) << std::scientific << preresnorminf << "       | (ts="
             << std::setw(10) << std::setprecision(3) << std::scientific << dtsolve_ << ",te="
             << std::setw(10) << std::setprecision(3) << std::scientific << dtele_ << ")" << std::endl;

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceValues


/*----------------------------------------------------------------------*
 | print finish line of convergence table to screen         vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceFinishLine()
{
  if (myrank_ == 0)
    std::cout << "+------------+-------------------+--------------+--------------+------------------+" << std::endl << std::endl;

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::PrintConvergenceFinishLine


/*----------------------------------------------------------------------*
 |  write current state to BINIO                            vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::OutputState()
{
  // solution
  output_->WriteVector("phinp", phinp_);
  // time derivative of solution
  // output_->WriteVector("phidtnp", phidtnp_);
  // pressures
  output_->WriteVector("pressure", pressure_);
  // saturations
  output_->WriteVector("saturation", saturation_);

  // solid pressure
  {
    // convert dof-based Epetra vector into node-based Epetra multi-vector for postprocessing
    Teuchos::RCP<Epetra_MultiVector> solidpressure_multi =
        POROFLUIDMULTIPHASE::UTILS::ConvertDofVectorToNodeBasedMultiVector(*discret_,*solidpressure_,nds_solidpressure_,1);

    output_->WriteVector("solidpressure", solidpressure_multi,IO::DiscretizationWriter::nodevector);
  }

  // displacement field
  if (isale_)
  {
    Teuchos::RCP<const Epetra_Vector> dispnp = discret_->GetState(nds_disp_,"dispnp");
    if (dispnp == Teuchos::null)
      dserror("Cannot extract displacement field from discretization");

    // convert dof-based Epetra vector into node-based Epetra multi-vector for postprocessing
    Teuchos::RCP<Epetra_MultiVector> dispnp_multi =
        POROFLUIDMULTIPHASE::UTILS::ConvertDofVectorToNodeBasedMultiVector(*discret_,*dispnp,nds_disp_,nsd_);

    output_->WriteVector("dispnp", dispnp_multi, IO::DiscretizationWriter::nodevector);
  }
  // fluxes
  if(flux_!=Teuchos::null)
  {
    // post_drt_ensight does not support multivectors based on the dofmap
    // for now, I create single vectors that can be handled by the filter

    const int dim = DRT::Problem::Instance()->NDim();
    const int numdof = discret_->NumDof(discret_->lRowNode(0));
    // get the noderowmap
    const Epetra_Map* noderowmap = discret_->NodeRowMap();
    for (int k=0; k<numdof; k++)
    {
      Teuchos::RCP<Epetra_MultiVector> velocity_k = Teuchos::rcp(new Epetra_MultiVector(*noderowmap,3,true));

      std::ostringstream temp;
      temp << k+1;
      std::string name = "flux_"+temp.str();
      for (int i = 0;i<velocity_k->MyLength();++i)
      {
        // get value for each component of velocity vector
        for(int idim=0;idim<dim;idim++)
        {
          double value = ((*flux_)[k*dim+idim])[i];
          int err = velocity_k->ReplaceMyValue(i,idim,value);
          if (err!=0) dserror("Detected error in ReplaceMyValue");
        }
      }
      output_->WriteVector(name, velocity_k, IO::DiscretizationWriter::nodevector);
    }
  }

  return;
} // TimIntImpl::OutputState


/*----------------------------------------------------------------------*
 | increment time and step for next iteration               vuong 08/16 |
 *----------------------------------------------------------------------*/
inline void POROFLUIDMULTIPHASE::TimIntImpl::IncrementTimeAndStep()
{
  step_ += 1;
  time_ += dt_;
}


/*----------------------------------------------------------------------*
 |  calculate error compared to analytical solution         vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::EvaluateErrorComparedToAnalyticalSol()
{

  if(calcerr_== INPAR::POROFLUIDMULTIPHASE::calcerror_no)
    return;
  dserror("Error calculation not yet implemented! Element evaluation is missing.");

  // create the parameters for the error calculation
  Teuchos::ParameterList eleparams;
  eleparams.set<int>("action",POROFLUIDMULTIPHASE::calc_error);
  eleparams.set("total time",time_);
  eleparams.set<int>("calcerrorflag",calcerr_);

  switch (calcerr_)
  {
  case INPAR::POROFLUIDMULTIPHASE::calcerror_byfunction:
  {
    const int errorfunctnumber = poroparams_.get<int>("CALCERRORNO");
    if(errorfunctnumber<1)
      dserror("invalid value of paramter CALCERRORNO for error function evaluation!");

    eleparams.set<int>("error function number",errorfunctnumber);
  break;
  }
  default:
    dserror("Cannot calculate error. Unknown type of analytical test problem"); break;
  }

  // set vector values needed by elements
  discret_->ClearState();
  discret_->SetState("phinp",phinp_);

  // get (squared) error values
  Teuchos::RCP<Epetra_SerialDenseVector> errors
    = Teuchos::rcp(new Epetra_SerialDenseVector(4));
  discret_->EvaluateScalars(eleparams, errors);
  discret_->ClearState();

  // std::vector containing
  // [0]: relative L2 pressure error
  // [1]: relative H1 pressure error
  Teuchos::RCP<std::vector<double> > relerror = Teuchos::rcp(new std::vector<double>(2));

    if( std::abs((*errors)[2])>1e-14 )
      (*relerror)[0] = sqrt((*errors)[0])/sqrt((*errors)[2]);
    else
      (*relerror)[0] = sqrt((*errors)[0]);
    if( std::abs((*errors)[2])>1e-14 )
      (*relerror)[1] = sqrt((*errors)[1])/sqrt((*errors)[3]);
    else
      (*relerror)[1] = sqrt((*errors)[1]);

    if (myrank_ == 0)
    {

      // print last error in a separate file

      const std::string simulation = DRT::Problem::Instance()->OutputControlFile()->FileName();
      const std::string fname = simulation+"_pressure_time.relerror";

      if(step_==0)
      {
        std::ofstream f;
        f.open(fname.c_str());
        f << "#| Step | Time | rel. L2-error  | rel. H1-error  |\n";
        f << std::setprecision(10) << step_ << " " << std::setw(1)<< std::setprecision(5)
          << time_ << std::setw(1) << std::setprecision(6) << " "
          << (*relerror)[0] << std::setw(1) << std::setprecision(6) << " " << (*relerror)[1] << "\n";

        f.flush();
        f.close();
      }
      else
      {
        std::ofstream f;
        f.open(fname.c_str(),std::fstream::ate | std::fstream::app);
        f << std::setprecision(10) << step_ << " " << std::setw(3)<< std::setprecision(5)
        << time_ << std::setw(1) << std::setprecision(6) << " "
        << (*relerror)[0] << std::setw(1) << std::setprecision(6) << " " << (*relerror)[1] <<"\n";

        f.flush();
        f.close();
      }
    }

  return;
} // POROFLUIDMULTIPHASE::TimIntImpl::EvaluateErrorComparedToAnalyticalSol


/*----------------------------------------------------------------------*
 | build linear system tangent matrix, rhs/force residual   vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::Evaluate()
{

  // call elements to calculate system matrix and rhs and assemble
  AssembleMatAndRHS();

  // perform finite difference check on time integrator level
  if(fdcheck_ == INPAR::POROFLUIDMULTIPHASE::fdcheck_global)
    FDCheck();

}

/*----------------------------------------------------------------------*
 | iterative update of scalars                              vuong 08/16  |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::UpdateIter(
    const Teuchos::RCP<const Epetra_Vector> inc)
{
  // store incremental vector to be available for convergence check
  // if incremental vector is received from outside for coupled problem
  increment_->Update(1.0,*inc,0.0);

  // update scalar values by adding increments
  phinp_->Update(1.0,*inc,1.0);

  // compute time derivative at time n+1
  ComputeTimeDerivative();

} // UpdateIter


/*----------------------------------------------------------------------*
 | set convective velocity field (+ pressure and acceleration field as  |
 | well as fine-scale velocity field, if required)          vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::SetVelocityField(
    Teuchos::RCP<const Epetra_Vector>   vel            //!< velocity vector
    )
{
  // time measurement
  TEUCHOS_FUNC_TIME_MONITOR("POROFLUIDMULTIPHASE: set convective velocity field");

  //---------------------------------------------------------------------------
  // preliminaries
  //---------------------------------------------------------------------------
  if (nds_vel_ == -1)
    dserror("Dof set for velocity degreess of freedom has not been assigned!");

  if (vel == Teuchos::null)
    dserror("Velocity state is Teuchos::null");

  if (nds_vel_ >= discret_->NumDofSets())
    dserror("Too few dofsets on poro fluid discretization!");

  if(not vel->Map().SameAs(*discret_->DofRowMap(nds_vel_)))
    dserror("Map of given velocity and associated dof row map in poro fluid discretization"
        " do not match!");

  // provide discretization with velocity
  discret_->SetState(nds_vel_,"velocity field",vel);


  return;

} // TimIntImpl::SetVelocityField


/*----------------------------------------------------------------------*
 |  set initial field for phi                                 vuong 08/16 |
 *----------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::SetInitialField(
    const INPAR::POROFLUIDMULTIPHASE::InitialField init,
    const int startfuncno)
{
  switch(init)
  {
  case INPAR::POROFLUIDMULTIPHASE::initfield_zero_field:
  {
    phin_-> PutScalar(0.0);
    phinp_-> PutScalar(0.0);
    break;
  }
  case INPAR::POROFLUIDMULTIPHASE::initfield_field_by_function:
  {
    const Epetra_Map* dofrowmap = discret_->DofRowMap();

    // loop all nodes on the processor
    for(int lnodeid=0;lnodeid<discret_->NumMyRowNodes();lnodeid++)
    {
      // get the processor local node
      DRT::Node*  lnode      = discret_->lRowNode(lnodeid);
      // the set of degrees of freedom associated with the node
      std::vector<int> nodedofset = discret_->Dof(0,lnode);

      int numdofs = nodedofset.size();
      for (int k=0;k< numdofs;++k)
      {
        const int dofgid = nodedofset[k];
        int doflid = dofrowmap->LID(dofgid);
        // evaluate component k of spatial function
        double initialval = DRT::Problem::Instance() ->Funct(startfuncno-1).Evaluate(k,lnode->X(),time_,NULL);
        int err = phin_->ReplaceMyValues(1,&initialval,&doflid);
        if (err != 0) dserror("dof not on proc");
      }
    }

    // initialize also the solution vector. These values are a pretty good guess for the
    // solution after the first time step (much better than starting with a zero vector)
    phinp_->Update(1.0,*phin_ ,0.0);

    break;
  }
  case INPAR::POROFLUIDMULTIPHASE::initfield_field_by_condition:
  {
    dserror("'initfield_field_by_condition' not yet implemented");
    break;
  }
  default:
    dserror("Unknown option for initial field: %d", init); break;
  } // switch(init)

  return;
} // TimIntImpl::SetInitialField

/*----------------------------------------------------------------------*
 | create result test for this field                        vuong 08/16  |
 *----------------------------------------------------------------------*/
Teuchos::RCP<DRT::ResultTest> POROFLUIDMULTIPHASE::TimIntImpl::CreateFieldTest()
{
  return Teuchos::rcp(new POROFLUIDMULTIPHASE::ResultTest(*this));

}

/*----------------------------------------------------------------------------------------------*
 | finite difference check for scalar transport system matrix (for debugging only)   vuong 08/16 |
 *----------------------------------------------------------------------------------------------*/
void POROFLUIDMULTIPHASE::TimIntImpl::FDCheck()
{
  // initial screen output
  if(myrank_ == 0)
    std::cout << std::endl << "FINITE DIFFERENCE CHECK FOR SYSTEM MATRIX" << std::endl;

  // make a copy of state variables to undo perturbations later
  Teuchos::RCP<Epetra_Vector> phinp_original = Teuchos::rcp(new Epetra_Vector(*phinp_));

  // make a copy of system matrix as Epetra_CrsMatrix
  Teuchos::RCP<Epetra_CrsMatrix> sysmat_original = Teuchos::null;
  if(Teuchos::rcp_dynamic_cast<LINALG::SparseMatrix>(sysmat_) != Teuchos::null)
    sysmat_original = (new LINALG::SparseMatrix(*(Teuchos::rcp_static_cast<LINALG::SparseMatrix>(sysmat_))))->EpetraMatrix();
  else if(Teuchos::rcp_dynamic_cast<LINALG::BlockSparseMatrixBase>(sysmat_) != Teuchos::null)
    sysmat_original = (new LINALG::SparseMatrix(*(Teuchos::rcp_static_cast<LINALG::BlockSparseMatrixBase>(sysmat_)->Merge())))->EpetraMatrix();
  else
    dserror("Type of system matrix unknown!");
  sysmat_original->FillComplete();

  // make a copy of system right-hand side vector
  Teuchos::RCP<Epetra_Vector> rhs_original = Teuchos::rcp(new Epetra_Vector(*residual_));

  // initialize counter for system matrix entries with failing finite difference check
  int counter(0);

  // initialize tracking variable for maximum absolute and relative errors
  double maxabserr(0.);
  double maxrelerr(0.);

  for (int colgid=0; colgid<=sysmat_original->ColMap().MaxAllGID(); ++colgid)
  {
    // check whether current column index is a valid global column index and continue loop if not
    int collid(sysmat_original->ColMap().LID(colgid));
    int maxcollid(-1);
    discret_->Comm().MaxAll(&collid,&maxcollid,1);
    if(maxcollid < 0)
      continue;

    // fill state vector with original state variables
    phinp_->Update(1.,*phinp_original,0.);

    // impose perturbation
    if(phinp_->Map().MyGID(colgid))
      if(phinp_->SumIntoGlobalValue(colgid,0,fdcheckeps_))
        dserror("Perturbation could not be imposed on state vector for finite difference check!");

    // carry perturbation over to state vectors at intermediate time stages if necessary
    ComputeIntermediateValues();
    ComputeTimeDerivative();

    // calculate element right-hand side vector for perturbed state
    AssembleMatAndRHS();

    // Now we compare the difference between the current entries in the system matrix
    // and their finite difference approximations according to
    // entries ?= (residual_perturbed - residual_original) / epsilon

    // Note that the residual_ vector actually denotes the right-hand side of the linear
    // system of equations, i.e., the negative system residual.
    // To account for errors due to numerical cancellation, we additionally consider
    // entries + residual_original / epsilon ?= residual_perturbed / epsilon

    // Note that we still need to evaluate the first comparison as well. For small entries in the system
    // matrix, the second comparison might yield good agreement in spite of the entries being wrong!
    for(int rowlid=0; rowlid<discret_->DofRowMap()->NumMyElements(); ++rowlid)
    {
      // get global index of current matrix row
      const int rowgid = sysmat_original->RowMap().GID(rowlid);
      if(rowgid < 0)
        dserror("Invalid global ID of matrix row!");

      // get relevant entry in current row of original system matrix
      double entry(0.);
      int length = sysmat_original->NumMyEntries(rowlid);
      int numentries;
      std::vector<double> values(length);
      std::vector<int> indices(length);
      sysmat_original->ExtractMyRowCopy(rowlid,length,numentries,&values[0],&indices[0]);
      for(int ientry=0; ientry<length; ++ientry)
      {
        if(sysmat_original->ColMap().GID(indices[ientry]) == colgid)
        {
          entry = values[ientry];
          break;
        }
      }

      // finite difference suggestion (first divide by epsilon and then add for better conditioning)
      const double fdval = -(*residual_)[rowlid] / fdcheckeps_ + (*rhs_original)[rowlid] / fdcheckeps_;

      // confirm accuracy of first comparison
      if(abs(fdval) > 1.e-17 and abs(fdval) < 1.e-15)
        dserror("Finite difference check involves values too close to numerical zero!");

      // absolute and relative errors in first comparison
      const double abserr1 = entry - fdval;
      if(abs(abserr1) > maxabserr)
        maxabserr = abs(abserr1);
      double relerr1(0.);
      if(abs(entry) > 1.e-17)
        relerr1 = abserr1 / abs(entry);
      else if(abs(fdval) > 1.e-17)
        relerr1 = abserr1 / abs(fdval);
      if(abs(relerr1) > maxrelerr)
        maxrelerr = abs(relerr1);

      // evaluate first comparison
      if(abs(relerr1) > fdchecktol_)
      {
        std::cout << "sysmat[" << rowgid << "," << colgid << "]:  " << entry << "   ";
        std::cout << "finite difference suggestion:  " << fdval << "   ";
        std::cout << "absolute error:  " << abserr1 << "   ";
        std::cout << "relative error:  " << relerr1 << std::endl;

        counter++;
      }

      // first comparison OK
      else
      {
        // left-hand side in second comparison
        const double left  = entry - (*rhs_original)[rowlid] / fdcheckeps_;

        // right-hand side in second comparison
        const double right = -(*residual_)[rowlid] / fdcheckeps_;

        // confirm accuracy of second comparison
        if(abs(right) > 1.e-17 and abs(right) < 1.e-15)
          dserror("Finite difference check involves values too close to numerical zero!");

        // absolute and relative errors in second comparison
        const double abserr2 = left - right;
        if(abs(abserr2) > maxabserr)
          maxabserr = abs(abserr2);
        double relerr2(0.);
        if(abs(left) > 1.e-17)
          relerr2 = abserr2 / abs(left);
        else if(abs(right) > 1.e-17)
          relerr2 = abserr2 / abs(right);
        if(abs(relerr2) > maxrelerr)
          maxrelerr = abs(relerr2);

        // evaluate second comparison
        if(abs(relerr2) > fdchecktol_)
        {
          std::cout << "sysmat[" << rowgid << "," << colgid << "]-rhs[" << rowgid << "]/eps:  " << left << "   ";
          std::cout << "-rhs_perturbed[" << rowgid << "]/eps:  " << right << "   ";
          std::cout << "absolute error:  " << abserr2 << "   ";
          std::cout << "relative error:  " << relerr2 << std::endl;

          counter++;
        }
      }
    }
  }

  // communicate tracking variables
  int counterglobal(0);
  discret_->Comm().SumAll(&counter,&counterglobal,1);
  double maxabserrglobal(0.);
  discret_->Comm().MaxAll(&maxabserr,&maxabserrglobal,1);
  double maxrelerrglobal(0.);
  discret_->Comm().MaxAll(&maxrelerr,&maxrelerrglobal,1);

  // final screen output
  if(myrank_ == 0)
  {
    if(counterglobal)
    {
      printf("--> FAILED AS LISTED ABOVE WITH %d CRITICAL MATRIX ENTRIES IN TOTAL\n\n",counterglobal);
      dserror("Finite difference check failed for scalar transport system matrix!");
    }
    else
      printf("--> PASSED WITH MAXIMUM ABSOLUTE ERROR %+12.5e AND MAXIMUM RELATIVE ERROR %+12.5e\n\n",maxabserrglobal,maxrelerrglobal);
  }

  // undo perturbations of state variables
  phinp_->Update(1.,*phinp_original,0.);
  ComputeIntermediateValues();
  ComputeTimeDerivative();

  // recompute system matrix and right-hand side vector based on original state variables
  AssembleMatAndRHS();

  return;
}