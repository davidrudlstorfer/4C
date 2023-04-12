/*---------------------------------------------------------------------*/
/*! \file

\brief Incomplete! - Purpose: Internal implementation of RedAirBloodScatraLine3 element


\level 3

*/
/*---------------------------------------------------------------------*/



#include "red_airways_air_blood_scatraLine3_impl.H"

#include "mat_newtonianfluid.H"
#include "mat_par_bundle.H"
#include "mat_list.H"
#include "mat_air_0d_O2_saturation.H"
#include "mat_hemoglobin_0d_O2_saturation.H"
#include "lib_discret.H"
#include "lib_function.H"
#include "lib_utils.H"
#include "lib_globalproblem.H"
#include "fem_general_utils_fem_shapefunctions.H"
#include "fem_general_utils_gder2.H"

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
DRT::ELEMENTS::RedAirBloodScatraLine3ImplInterface*
DRT::ELEMENTS::RedAirBloodScatraLine3ImplInterface::Impl(
    DRT::ELEMENTS::RedAirBloodScatraLine3* red_acinus)
{
  switch (red_acinus->Shape())
  {
    case DRT::Element::line3:
    {
      static RedAirBloodScatraLine3Impl<DRT::Element::line3>* acinus;
      if (acinus == NULL)
      {
        acinus = new RedAirBloodScatraLine3Impl<DRT::Element::line3>;
      }
      return acinus;
    }
    default:
      dserror("shape %d (%d nodes) not supported", red_acinus->Shape(), red_acinus->NumNode());
  }
  return NULL;
}



/*----------------------------------------------------------------------*
  | constructor (public)                                    ismail 01/10 |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::RedAirBloodScatraLine3Impl()
{
}

/*----------------------------------------------------------------------*
 | evaluate (public)                                       ismail 01/10 |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
int DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::Evaluate(RedAirBloodScatraLine3* ele,
    Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
    Epetra_SerialDenseMatrix& elemat1_epetra, Epetra_SerialDenseMatrix& elemat2_epetra,
    Epetra_SerialDenseVector& elevec1_epetra, Epetra_SerialDenseVector& elevec2_epetra,
    Epetra_SerialDenseVector& elevec3_epetra, Teuchos::RCP<MAT::Material> mat)
{
  return 0;
}


/*----------------------------------------------------------------------*
 |  calculate element matrix and right hand side (private)  ismail 01/10|
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::Initial(RedAirBloodScatraLine3* ele,
    Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
    Teuchos::RCP<const MAT::Material> material)
{
  Teuchos::RCP<Epetra_Vector> generations = params.get<Teuchos::RCP<Epetra_Vector>>("generations");

  //--------------------------------------------------------------------
  // get the generation numbers
  //--------------------------------------------------------------------
  //  if(myrank == ele->Owner())
  {
    int gid = ele->Id();
    double val = -2.0;
    generations->ReplaceGlobalValues(1, &val, &gid);
  }

}  // RedAirBloodScatraLine3Impl::Initial

/*----------------------------------------------------------------------*
 |  calculate element matrix and right hand side (private)  ismail 01/10|
 |                                                                      |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::Sysmat(RedAirBloodScatraLine3* ele,
    Epetra_SerialDenseVector& epnp, Epetra_SerialDenseVector& epn, Epetra_SerialDenseVector& epnm,
    Epetra_SerialDenseMatrix& sysmat, Epetra_SerialDenseVector& rhs,
    Teuchos::RCP<const MAT::Material> material, Teuchos::ParameterList& params, double time,
    double dt)
{
}

/*----------------------------------------------------------------------*
 |  Evaluate the values of the degrees of freedom           ismail 01/10|
 |  at terminal nodes.                                                  |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::EvaluateTerminalBC(
    RedAirBloodScatraLine3* ele, Teuchos::ParameterList& params,
    DRT::Discretization& discretization, std::vector<int>& lm, Epetra_SerialDenseVector& rhs,
    Teuchos::RCP<MAT::Material> material)
{
}


/*----------------------------------------------------------------------*
 |  Evaluate the values of the degrees of freedom           ismail 01/10|
 |  at terminal nodes.                                                  |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::CalcFlowRates(RedAirBloodScatraLine3* ele,
    Teuchos::ParameterList& params, DRT::Discretization& discretization,
    Epetra_SerialDenseVector& elevec1,  // a_volumenp,
    Epetra_SerialDenseVector& elevec2,  // a_volume_strain_np,
    std::vector<int>& lm, Teuchos::RCP<MAT::Material> material)

{
}

/*----------------------------------------------------------------------*
 |  Get the coupled the values on the coupling interface    ismail 07/10|
 |  of the 3D/reduced-D problem                                         |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::GetCoupledValues(
    RedAirBloodScatraLine3* ele, Teuchos::ParameterList& params,
    DRT::Discretization& discretization, std::vector<int>& lm, Teuchos::RCP<MAT::Material> material)
{
}

/*----------------------------------------------------------------------*
 |  solve the transport of O2 from air to blood             ismail 02/13|
 |                                                                      |
 | Example of use (1):                                                  |
 |--------------------                                                  |
 |        [RED_AIRWAY element]          [RED_ACINUS element]            |
 |             |                             |                          |
 |             |                             |                          |
 |  (node1)    V     (node2)        (node2)  V   (node1)                |
 |     o======>>>=======o              o============o                   |
 |     |       ^        |              |                                |
 |     |       |        |              |                                |
 |     |(flow direction)|              |                                |
 |     |                |              |                                |
 |     V                V              |                                |
 |     o=====           o===           |                                |
 |  (node1)  =====   (node3)==         |                                |
 |    or 3      ^ ==== or 1   ==       |                                |
 |              |     ======    ===    |                                |
 |              |           =====  ==  V                                |
 |              |                ======o                                |
 |              |                   (node2)                             |
 |              |                                                       |
 |              |                                                       |
 |    [RED_AIR_BLOOD_SCATRA_LINE3 element]                              |
 |                                                                      |
 | Example of use (2):                                                  |
 |--------------------                                                  |
 |        [RED_AIRWAY element]          [RED_ACINUS element]            |
 |             |                             |                          |
 |             |                             |                          |
 |  (node1)    V     (node2)        (node2)  V   (node1)                |
 |     o======>>>=======o              o============o                   |
 |     |       ^        |              |                                |
 |     |       |        |              |                                |
 |     |(flow direction)|              |                                |
 |     |                |              |                                |
 |     V                V              |                                |
 |     o================o==============o                                |
 |  (node1)     ^    (node2)        (node3)                             |
 |    or 3      |      or 1           or 1                              |
 |              |                                                       |
 |              |                                                       |
 |    [RED_AIR_BLOOD_SCATRA_LINE3 element]                              |
 *----------------------------------------------------------------------*/
template <DRT::Element::DiscretizationType distype>
void DRT::ELEMENTS::RedAirBloodScatraLine3Impl<distype>::SolveBloodAirTransport(
    RedAirBloodScatraLine3* ele, Epetra_SerialDenseVector& dscatra, Epetra_SerialDenseVector& dvo2,
    Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
    Teuchos::RCP<MAT::Material> material)
{
}