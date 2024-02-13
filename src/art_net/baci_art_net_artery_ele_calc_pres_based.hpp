/*----------------------------------------------------------------------*/
/*! \file

\brief Internal implementation of PressureBased artery element


\level 3

*/
/*----------------------------------------------------------------------*/
#ifndef BACI_ART_NET_ARTERY_ELE_CALC_PRES_BASED_HPP
#define BACI_ART_NET_ARTERY_ELE_CALC_PRES_BASED_HPP

#include "baci_config.hpp"

#include "baci_art_net_artery.hpp"
#include "baci_art_net_artery_ele_action.hpp"
#include "baci_art_net_artery_ele_calc.hpp"
#include "baci_discretization_fem_general_utils_local_connectivity_matrices.hpp"
#include "baci_linalg_fixedsizematrix.hpp"

BACI_NAMESPACE_OPEN


namespace DRT
{
  namespace ELEMENTS
  {
    /// Internal artery implementation
    /*!
      This internal class keeps all the working arrays needed to
      calculate the artery element. Additionally the method Sysmat()
      provides a clean and fast element implementation.

      <h3>Purpose</h3>

      \author kremheller
      \date 03/18
    */

    template <CORE::FE::CellType distype>
    class ArteryEleCalcPresBased : public ArteryEleCalc<distype>
    {
     private:
      typedef ArteryEleCalc<distype> my;

      /// private constructor, since we are a Singleton.
      ArteryEleCalcPresBased(const int numdofpernode, const std::string& disname);

     public:
      //! Singleton access method
      static ArteryEleCalcPresBased<distype>* Instance(
          const int numdofpernode, const std::string& disname);

      int Evaluate(Artery* ele, Teuchos::ParameterList& params, DRT::Discretization& discretization,
          DRT::Element::LocationArray& la, CORE::LINALG::SerialDenseMatrix& elemat1_epetra,
          CORE::LINALG::SerialDenseMatrix& elemat2_epetra,
          CORE::LINALG::SerialDenseVector& elevec1_epetra,
          CORE::LINALG::SerialDenseVector& elevec2_epetra,
          CORE::LINALG::SerialDenseVector& elevec3_epetra,
          Teuchos::RCP<MAT::Material> mat) override;

      int ScatraEvaluate(Artery* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix& elemat1_epetra,
          CORE::LINALG::SerialDenseMatrix& elemat2_epetra,
          CORE::LINALG::SerialDenseVector& elevec1_epetra,
          CORE::LINALG::SerialDenseVector& elevec2_epetra,
          CORE::LINALG::SerialDenseVector& elevec3_epetra,
          Teuchos::RCP<MAT::Material> mat) override;

      int EvaluateService(Artery* ele, const ARTERY::Action action, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, DRT::Element::LocationArray& la,
          CORE::LINALG::SerialDenseMatrix& elemat1_epetra,
          CORE::LINALG::SerialDenseMatrix& elemat2_epetra,
          CORE::LINALG::SerialDenseVector& elevec1_epetra,
          CORE::LINALG::SerialDenseVector& elevec2_epetra,
          CORE::LINALG::SerialDenseVector& elevec3_epetra,
          Teuchos::RCP<MAT::Material> mat) override;


     protected:
      /*!
        \brief calculate element matrix and rhs

        \param ele[in]            the element which is evaluated
        \param discretization[in] discretization to which element belongs
        \param la[in]             element location array
        \param sysmat[in,out]     element matrix to calculate
        \param rhs[in,out]        element rhs to calculate
        \param material[in]       artery material
        */
      void Sysmat(Artery* ele, DRT::Discretization& discretization, DRT::Element::LocationArray& la,
          CORE::LINALG::Matrix<my::iel_, my::iel_>& sysmat, CORE::LINALG::Matrix<my::iel_, 1>& rhs,
          Teuchos::RCP<const MAT::Material> material);

      /*!
        \brief Evaluate volumetric flow inside the element (for post-processing)

        \param ele[in]            the element which is evaluated
        \param discretization[in] discretization to which element belongs
        \param la[in]             element location array
        \param flowVec[in,out]    element flow to calculate
        \param material[in]       artery material/dimesion

        \note  only checked for line2 elements
        */
      void EvaluateFlow(Artery* ele, DRT::Discretization& discretization,
          DRT::Element::LocationArray& la, CORE::LINALG::SerialDenseVector& flowVec,
          Teuchos::RCP<const MAT::Material> material);

      /*!
        \brief Calculate element length (either in current or deformed configuration)
        \param ele[in]            the element which is evaluated
        \param discretization[in] discretization to which element belongs
        \param la[in]             element location array
        \return                   element length (either in current or deformed configuration)
       */
      double CalculateEleLength(
          Artery* ele, DRT::Discretization& discretization, DRT::Element::LocationArray& la);
    };

  }  // namespace ELEMENTS

}  // namespace DRT



BACI_NAMESPACE_CLOSE

#endif  // ART_NET_ARTERY_ELE_CALC_PRES_BASED_H