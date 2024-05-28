/*-----------------------------------------------------------*/
/*! \file

\brief explicit structural time integration


\level 3

*/
/*-----------------------------------------------------------*/


#include "4C_structure_new_timint_explicit.hpp"

#include "4C_solver_nonlin_nox_group.hpp"
#include "4C_solver_nonlin_nox_linearsystem.hpp"
#include "4C_structure_new_nln_solver_factory.hpp"
#include "4C_structure_new_timint_noxinterface.hpp"

#include <NOX_Abstract_Group.H>

FOUR_C_NAMESPACE_OPEN


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
STR::TIMINT::Explicit::Explicit() : STR::TIMINT::Base()
{
  // empty
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::Setup()
{
  // safety check
  check_init();
  STR::TIMINT::Base::Setup();
  // ---------------------------------------------------------------------------
  // cast the base class integrator
  // ---------------------------------------------------------------------------
  explint_ptr_ = Teuchos::rcp_dynamic_cast<STR::EXPLICIT::Generic>(integrator_ptr(), true);
  // ---------------------------------------------------------------------------
  // build NOX interface
  // ---------------------------------------------------------------------------
  Teuchos::RCP<STR::TIMINT::NoxInterface> noxinterface_ptr =
      Teuchos::rcp(new STR::TIMINT::NoxInterface);
  noxinterface_ptr->Init(
      data_global_state_ptr(), explint_ptr_, DBCPtr(), Teuchos::rcp(this, false));
  noxinterface_ptr->Setup();
  // ---------------------------------------------------------------------------
  // build non-linear solver
  // ---------------------------------------------------------------------------
  enum INPAR::STR::NonlinSolTech nlnSolverType = DataSDyn().GetNlnSolverType();
  if (nlnSolverType != INPAR::STR::soltech_singlestep)
  {
    std::cout << "WARNING!!!Nonlinear solver for explicit dynamics is given (in the dat file) as "
              << INPAR::STR::NonlinSolTechString(nlnSolverType)
              << ". This is not compatible. singlestep solver will be selected." << std::endl;
    nlnSolverType = INPAR::STR::soltech_singlestep;
  }
  nlnsolver_ptr_ = STR::NLN::SOLVER::BuildNlnSolver(nlnSolverType);
  nlnsolver_ptr_->Init(data_global_state_ptr(), data_s_dyn_ptr(), noxinterface_ptr, explint_ptr_,
      Teuchos::rcp(this, false));
  nlnsolver_ptr_->Setup();
  // set setup flag
  issetup_ = true;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::prepare_time_step()
{
  check_init_setup();
  // things that need to be done before Predict
  PrePredict();

  // ToDo prepare contact for new time step
  // PrepareStepContact();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::update_state_incrementally(Teuchos::RCP<const Epetra_Vector> disiterinc)
{
  check_init_setup();
  FOUR_C_THROW(
      "All monolithically coupled problems work with implicit time "
      "integration schemes. Thus, calling Evaluate() in an explicit scheme "
      "is not possible.");
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::determine_stress_strain() { expl_int().determine_stress_strain(); }

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::Evaluate(Teuchos::RCP<const Epetra_Vector> disiterinc)
{
  check_init_setup();
  FOUR_C_THROW(
      "All monolithically coupled problems work with implicit time "
      "integration schemes. Thus, calling Evaluate() in an explicit scheme "
      "is not possible.");
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::Evaluate()
{
  check_init_setup();
  throw_if_state_not_in_sync_with_nox_group();
  ::NOX::Abstract::Group& grp = nln_solver().SolutionGroup();

  auto* grp_ptr = dynamic_cast<NOX::NLN::Group*>(&grp);
  if (grp_ptr == nullptr) FOUR_C_THROW("Dynamic cast failed!");

  // you definitely have to evaluate here. You might be called from a coupled
  // problem and the group might not be aware, that a different state than
  // the internally stored displacements may have changed.
  // This is a hack to get NOX to set IsValid to false.
  grp_ptr->setX(grp_ptr->getX());

  // compute the rhs vector and the stiffness matrix
  grp_ptr->computeFandJacobian();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::set_state(const Teuchos::RCP<Epetra_Vector>& x)
{
  FOUR_C_THROW(
      "All coupled problems work with implicit time "
      "integration schemes. Thus, calling set_state() in an explicit scheme "
      "is not considered, yet.");
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::reset_step()
{
  // calling the base reset
  STR::TIMINT::Base::reset_step();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
INPAR::STR::ConvergenceStatus STR::TIMINT::Explicit::Solve()
{
  check_init_setup();
  IntegrateStep();
  return INPAR::STR::conv_success;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::prepare_partition_step()
{
  // do nothing for explicit time integrators
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::Update(double endtime)
{
  check_init_setup();
  FOUR_C_THROW("Not implemented. No time adaptivity available for explicit time integration.");
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::PrintStep()
{
  check_init_setup();

  if (data_global_state().GetMyRank() != 0 or GroupId() != 0) return;

  const int stepmax = DataSDyn().GetStepMax();
  const int stepn = data_global_state().GetStepN();
  const double timen = data_global_state().GetTimeN();
  const double dt = (*data_global_state().GetDeltaTime())[0];
  const double wct = data_global_state().GetTimer()->totalElapsedTime(true);

  // open outstd::stringstream
  std::ostringstream oss;

  /* Output of the following quantities
   * time   : total simulated time
   * dt     : used time step
   * wct    : wall clock time */
  oss << "Finalised step " << std::setw(1) << stepn;
  oss << " / " << std::setw(1) << stepmax;
  oss << " | time " << std::setw(9) << std::setprecision(3) << std::scientific << timen;
  oss << " | dt " << std::setw(9) << std::setprecision(3) << std::scientific << dt;
  oss << " | wct " << std::setw(8) << std::setprecision(2) << std::scientific << wct;
  oss << "\n--------------------------------------------------------------------------------\n";

  // print to ofile (could be done differently...)
  fprintf(stdout, "%s\n", oss.str().c_str());

  // print it, now
  fflush(stdout);
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
INPAR::STR::StcScale STR::TIMINT::Explicit::GetSTCAlgo()
{
  check_init_setup();
  FOUR_C_THROW("GetSTCAlgo() has not been tested for explicit time integration.");
  return INPAR::STR::stc_none;
};


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<CORE::LINALG::SparseMatrix> STR::TIMINT::Explicit::GetSTCMat()
{
  check_init_setup();
  FOUR_C_THROW("GetSTCMat() has not been tested for explicit time integration.");
  return Teuchos::null;
};


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
int STR::TIMINT::Explicit::Integrate()
{
  FOUR_C_THROW(
      "The function is unused since the ADAPTER::StructureTimeLoop "
      "wrapper gives you all the flexibility you need.");
  return 0;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
int STR::TIMINT::Explicit::IntegrateStep()
{
  check_init_setup();
  throw_if_state_not_in_sync_with_nox_group();
  // reset the non-linear solver
  nln_solver().Reset();
  // solve the non-linear problem
  nln_solver().Solve();
  return 0;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Vector> STR::TIMINT::Explicit::initial_guess()
{
  FOUR_C_THROW("initial_guess() is not available for explicit time integration");
  return Teuchos::null;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<const Epetra_Vector> STR::TIMINT::Explicit::GetF() const
{
  FOUR_C_THROW("RHS() is not available for explicit time integration");
  return Teuchos::null;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<Epetra_Vector> STR::TIMINT::Explicit::Freact()
{
  check_init_setup();
  FOUR_C_THROW("Not implemented!");
  return Teuchos::null;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<CORE::LINALG::SparseMatrix> STR::TIMINT::Explicit::SystemMatrix()
{
  FOUR_C_THROW("SystemMatrix() is not available for explicit time integration");
  return Teuchos::null;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<CORE::LINALG::BlockSparseMatrixBase> STR::TIMINT::Explicit::BlockSystemMatrix()
{
  FOUR_C_THROW("BlockSystemMatrix() is not available for explicit time integration");
  return Teuchos::null;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void STR::TIMINT::Explicit::use_block_matrix(
    Teuchos::RCP<const CORE::LINALG::MultiMapExtractor> domainmaps,
    Teuchos::RCP<const CORE::LINALG::MultiMapExtractor> rangemaps)
{
  FOUR_C_THROW("use_block_matrix() is not available for explicit time integration");
}
///@}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
enum INPAR::STR::DynamicType STR::TIMINT::Explicit::MethodName() const
{
  return explint_ptr_->MethodName();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
int STR::TIMINT::Explicit::MethodSteps() const { return explint_ptr_->MethodSteps(); }

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
int STR::TIMINT::Explicit::method_order_of_accuracy_dis() const
{
  return explint_ptr_->method_order_of_accuracy_dis();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
int STR::TIMINT::Explicit::method_order_of_accuracy_vel() const
{
  return explint_ptr_->method_order_of_accuracy_vel();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
double STR::TIMINT::Explicit::method_lin_err_coeff_dis() const
{
  return explint_ptr_->method_lin_err_coeff_dis();
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
double STR::TIMINT::Explicit::method_lin_err_coeff_vel() const
{
  return explint_ptr_->method_lin_err_coeff_vel();
}

FOUR_C_NAMESPACE_CLOSE