/*--------------------------------------------------------------------------*/
/*! \file

\brief evaluation of scatra elements for conservation of mass concentration and electronic charge
within electrodes

\level 2

*/
/*--------------------------------------------------------------------------*/
#include "4C_discretization_fem_general_extract_values.hpp"
#include "4C_lib_discret.hpp"
#include "4C_mat_material_factory.hpp"
#include "4C_material_base.hpp"
#include "4C_scatra_ele_calc_elch_electrode.hpp"
#include "4C_scatra_ele_parameter_std.hpp"
#include "4C_scatra_ele_parameter_timint.hpp"

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*
 | evaluate action                                           fang 02/15 |
 *----------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
int DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype, probdim>::evaluate_action(DRT::Element* ele,
    Teuchos::ParameterList& params, DRT::Discretization& discretization,
    const SCATRA::Action& action, DRT::Element::LocationArray& la,
    CORE::LINALG::SerialDenseMatrix& elemat1_epetra,
    CORE::LINALG::SerialDenseMatrix& elemat2_epetra,
    CORE::LINALG::SerialDenseVector& elevec1_epetra,
    CORE::LINALG::SerialDenseVector& elevec2_epetra,
    CORE::LINALG::SerialDenseVector& elevec3_epetra)
{
  // determine and evaluate action
  switch (action)
  {
    case SCATRA::Action::calc_elch_electrode_soc_and_c_rate:
    {
      calculate_electrode_soc_and_c_rate(ele, discretization, la, elevec1_epetra);

      break;
    }
    case SCATRA::Action::calc_elch_elctrode_mean_concentration:
    {
      calculate_mean_electrode_concentration(ele, discretization, la, elevec1_epetra);

      break;
    }

    default:
    {
      myelch::evaluate_action(ele, params, discretization, action, la, elemat1_epetra,
          elemat2_epetra, elevec1_epetra, elevec2_epetra, elevec3_epetra);

      break;
    }
  }  // switch(action)

  return 0;
}


/*----------------------------------------------------------------------------------------------------------*
 | validity check with respect to input parameters, degrees of freedom, number of scalars etc. fang
 02/15 |
 *----------------------------------------------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype, probdim>::check_elch_element_parameter(
    DRT::Element* ele  //!< current element
)
{
  // safety checks
  if (ele->Material()->MaterialType() != CORE::Materials::m_electrode)
    FOUR_C_THROW("Invalid material type!");

  if (my::numscal_ != 1) FOUR_C_THROW("Invalid number of transported scalars!");
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::check_elch_element_parameter


/*----------------------------------------------------------------------*
 | get conductivity                                          fang 02/15 |
 *----------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype, probdim>::get_conductivity(
    const enum INPAR::ELCH::EquPot equpot,  //!< type of closing equation for electric potential
    double& sigma_all,                      //!< conductivity of electrolyte solution
    std::vector<double>& sigma,  //!< conductivity or a single ion + overall electrolyte solution
    bool effCond)
{
  // use precomputed conductivity
  sigma_all = diff_manager()->GetCond();
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::get_conductivity


/*----------------------------------------------------------------------*
 | calculate weighted current density                        fang 07/16 |
 *----------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype, probdim>::calculate_current(
    CORE::LINALG::Matrix<nsd_, 1>& q,        //!< flux of species k
    const INPAR::SCATRA::FluxType fluxtype,  //!< type fo flux
    const double fac                         //!< integration factor
)
{
  /*
  Actually, we compute here a weighted (and integrated) form of the current density!
  On time integration level, these contributions are then used to calculate
  an L2-projected representation of the current density.
  Thus, this method here DOES NOT YET provide current density values that are ready to use!

  /                           \
  |                    /   \  |
  | w, -sigma * nabla | phi | |
  |                    \   /  |
  \                           /
  */

  switch (fluxtype)
  {
    case INPAR::SCATRA::flux_diffusive:
    case INPAR::SCATRA::flux_total:
    {
      // ohmic contribution to current density
      q.Update(-diff_manager()->GetCond(), var_manager()->GradPot());
      break;
    }

    default:
    {
      FOUR_C_THROW("Invalid flux type!");
      break;
    }
  }
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::calculate_current


/*----------------------------------------------------------------------*
 | calculate electrode state of charge and C rate            fang 01/15 |
 *----------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype,
    probdim>::calculate_electrode_soc_and_c_rate(const DRT::Element* const&
                                                     ele,  //!< the element we are dealing with
    const DRT::Discretization& discretization,             //!< discretization
    DRT::Element::LocationArray& la,                       //!< location array
    CORE::LINALG::SerialDenseVector& scalars  //!< result vector for scalar integrals to be computed
)
{
  // safety check
  if (my::numscal_ != 1)
    FOUR_C_THROW("Electrode state of charge can only be computed for one transported scalar!");

  // get global state vectors
  const Teuchos::RCP<const Epetra_Vector> phinp = discretization.GetState("phinp");
  if (phinp == Teuchos::null) FOUR_C_THROW("Cannot get state vector \"phinp\"!");
  const Teuchos::RCP<const Epetra_Vector> phidtnp = discretization.GetState("phidtnp");
  if (phidtnp == Teuchos::null) FOUR_C_THROW("Cannot get state vector \"phidtnp\"!");

  // extract local nodal values from global state vectors
  CORE::FE::ExtractMyValues(*phinp, my::ephinp_, la[0].lm_);
  static std::vector<CORE::LINALG::Matrix<nen_, 1>> ephidtnp(2);
  CORE::FE::ExtractMyValues(*phidtnp, ephidtnp, la[0].lm_);

  // initialize variables for integrals of concentration, its time derivative, and domain
  double intconcentration(0.);
  double intconcentrationtimederiv(0.);
  double intdomain(0.);

  // integration points and weights
  const CORE::FE::IntPointsAndWeights<nsd_ele_> intpoints(
      SCATRA::DisTypeToOptGaussRule<distype>::rule);

  // loop over integration points
  for (int iquad = 0; iquad < intpoints.IP().nquad; ++iquad)
  {
    // evaluate values of shape functions and domain integration factor at current integration point
    const double fac = my::eval_shape_func_and_derivs_at_int_point(intpoints, iquad);

    // calculate integrals of concentration and its time derivative
    for (unsigned vi = 0; vi < nen_; ++vi)
    {
      const double vi_fac = my::funct_(vi) * fac;

      // integral of concentration
      intconcentration += my::ephinp_[0](vi) * vi_fac;

      // integral of time derivative of concentration
      intconcentrationtimederiv += ephidtnp[0](vi) * vi_fac;
    }

    // domain integral
    intdomain += fac;
  }  // loop over integration points

  // safety check
  if (not my::scatrapara_->IsAle() and scalars.length() != 3)
    FOUR_C_THROW(
        "Result vector for electrode state of charge and C rate computation has invalid length!");

  // write results for integrals of concentration, its time derivative, and domain into result
  // vector
  scalars(0) = intconcentration;
  scalars(1) = intconcentrationtimederiv;
  scalars(2) = intdomain;

  // additional computations in case of ALE
  if (my::scatrapara_->IsAle())
  {
    const int ndsvel = my::scatrapara_->NdsVel();
    // extract velocities
    const Teuchos::RCP<const Epetra_Vector> vel = discretization.GetState(ndsvel, "velocity field");
    if (vel == Teuchos::null) FOUR_C_THROW("Cannot get state vector \"velocity field\"!");
    CORE::FE::ExtractMyValues(*vel, my::evelnp_, la[ndsvel].lm_);

    // initialize additional variables for integrals related to velocity divergence
    double intdivv(0.);
    double intcdivv(0.);
    double intvgradc(0.);

    // loop over integration points
    for (int iquad = 0; iquad < intpoints.IP().nquad; ++iquad)
    {
      // evaluate values of shape functions and domain integration factor at current integration
      // point
      const double fac = my::eval_shape_func_and_derivs_at_int_point(intpoints, iquad);

      // compute internal variables at current integration point
      var_manager()->set_internal_variables_elch_electrode_soc_and_c_rate(
          my::funct_, my::derxy_, my::ephinp_, my::ephin_, my::econvelnp_, my::ehist_);

      // compute velocity and its divergence
      static CORE::LINALG::Matrix<nsd_, 1> v;
      v.Multiply(my::evelnp_, my::funct_);
      double divv(0.);
      my::get_divergence(divv, my::evelnp_);

      // integral of velocity divergence
      const double divv_fac = divv * fac;
      intdivv += divv_fac;

      // integral of concentration times velocity divergence
      intcdivv += my::scatravarmanager_->Phinp(0) * divv_fac;

      // integral of velocity times concentration gradient
      intvgradc += v.Dot(my::scatravarmanager_->GradPhi(0)) * fac;
    }  // loop over integration points

    // safety check
    if (scalars.length() != 6)
      FOUR_C_THROW(
          "Result vector for electrode state of charge and C rate computation has invalid length!");

    // write results for integrals related to velocity divergence into result vector
    scalars(3) = intdivv;
    scalars(4) = intcdivv;
    scalars(5) = intvgradc;
  }
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::calculate_electrode_soc_and_c_rate


/*---------------------------------------------------------------------*
 | calculate weighted mass flux (no reactive flux so far)   fang 02/15 |
 *---------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype, probdim>::calculate_flux(
    CORE::LINALG::Matrix<nsd_, 1>& q,        //!< flux of species k
    const INPAR::SCATRA::FluxType fluxtype,  //!< type fo flux
    const int k                              //!< index of current scalar
)
{
  /*
    Actually, we compute here a weighted (and integrated) form of the fluxes!
    On time integration level, these contributions are then used to calculate
    an L2-projected representation of fluxes.
    Thus, this method here DOES NOT YET provide flux values that are ready to use!
  */

  // add convective flux contribution
  switch (fluxtype)
  {
    case INPAR::SCATRA::flux_diffusive:
    case INPAR::SCATRA::flux_total:
    {
      // diffusive flux contribution
      q.Update(-diff_manager()->GetIsotropicDiff(k), var_manager()->GradPhi(k));
      break;
    }

    default:
    {
      FOUR_C_THROW("received illegal flag inside flux evaluation for whole domain");
      break;
    }
  }
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::calculate_flux


/*----------------------------------------------------------------------------------------*
 | calculate error of numerical solution with respect to analytical solution   fang 10/16 |
 *----------------------------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype,
    probdim>::cal_error_compared_to_analyt_solution(const DRT::Element* ele,  //!< element
    Teuchos::ParameterList& params,                                           //!< parameter list
    CORE::LINALG::SerialDenseVector& errors  //!< vector containing L2 and H1 error norms
)
{
  // call base class routine
  myelch::cal_error_compared_to_analyt_solution(ele, params, errors);
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::cal_error_compared_to_analyt_solution


/*------------------------------------------------------------------------------*
 | set internal variables for electrodes                             fang 02/15 |
 *------------------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype,
    probdim>::set_internal_variables_for_mat_and_rhs()
{
  // set internal variables
  var_manager()->set_internal_variables_elch_electrode(
      my::funct_, my::derxy_, my::ephinp_, my::ephin_, my::econvelnp_, my::ehist_);
}  // DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype>::set_internal_variables_for_mat_and_rhs()

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
template <CORE::FE::CellType distype, int probdim>
void DRT::ELEMENTS::ScaTraEleCalcElchElectrode<distype,
    probdim>::calculate_mean_electrode_concentration(const DRT::Element* const& ele,
    const DRT::Discretization& discretization, DRT::Element::LocationArray& la,
    CORE::LINALG::SerialDenseVector& conc)
{
  // for complete 1D simulation of battery:
  // Micro state must exist for electrolyte -> set value to 00
  for (int node = 0; node < static_cast<int>(nen_); ++node) conc(node) = 0.0;
}

// template classes
// 1D elements
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::line2, 1>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::line2, 2>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::line2, 3>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::line3, 1>;

// 2D elements
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::tri3, 2>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::tri3, 3>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::tri6, 2>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::quad4, 2>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::quad4, 3>;
// template class
// DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::quad8>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::quad9, 2>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::nurbs9, 2>;

// 3D elements
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::hex8, 3>;
// template class
// DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::hex20>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::hex27, 3>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::tet4, 3>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::tet10, 3>;
// template class
// DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::wedge6>;
template class DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::pyramid5, 3>;
// template class
// DRT::ELEMENTS::ScaTraEleCalcElchElectrode<CORE::FE::CellType::nurbs27>;

FOUR_C_NAMESPACE_CLOSE