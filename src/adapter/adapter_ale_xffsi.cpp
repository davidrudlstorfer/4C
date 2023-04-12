/*----------------------------------------------------------------------------*/
/*! \file

\brief Wrapper for the ALE time integration

\level 2


*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* header inclusions */
#include "adapter_ale_xffsi.H"

#include "lib_dserror.H"
#include "linalg_utils_sparse_algebra_math.H"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
ADAPTER::AleXFFsiWrapper::AleXFFsiWrapper(Teuchos::RCP<Ale> ale) : AleFsiWrapper(ale)
{
  // create the FSI interface
  xff_interface_ = Teuchos::rcp(new ALE::UTILS::XFluidFluidMapExtractor);
  xff_interface_->Setup(*Discretization());
  SetupDBCMapEx(ALE::UTILS::MapExtractor::dbc_set_x_ff, Interface(), xff_interface_);
  SetupDBCMapEx(ALE::UTILS::MapExtractor::dbc_set_x_fsi, Interface());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
const Teuchos::RCP<const LINALG::MapExtractor> ADAPTER::AleXFFsiWrapper::GetDBCMapExtractor()
{
  return AleWrapper::GetDBCMapExtractor(ALE::UTILS::MapExtractor::dbc_set_x_ff);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ADAPTER::AleXFFsiWrapper::Evaluate(Teuchos::RCP<const Epetra_Vector> stepinc)
{
  AleFsiWrapper::Evaluate(stepinc, ALE::UTILS::MapExtractor::dbc_set_x_ff);

  // set dispnp_ of xfem dofs to dispn_
  xff_interface_->InsertXFluidFluidCondVector(
      xff_interface_->ExtractXFluidFluidCondVector(Dispn()), WriteAccessDispnp());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int ADAPTER::AleXFFsiWrapper::Solve()
{
  AleFsiWrapper::Evaluate(Teuchos::null, ALE::UTILS::MapExtractor::dbc_set_x_fsi);

  int err = AleFsiWrapper::Solve();

  UpdateIter();

  return err;
}