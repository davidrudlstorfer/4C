/*----------------------------------------------------------------------*/
/*! \file

\brief evaluate boundary conditions requiring parent-element evaluations

\level 2


*/
/*----------------------------------------------------------------------*/

#ifndef FOUR_C_FLUID_ELE_BOUNDARY_PARENT_CALC_HPP
#define FOUR_C_FLUID_ELE_BOUNDARY_PARENT_CALC_HPP


#include "4C_config.hpp"

#include "4C_discretization_fem_general_utils_integration.hpp"
#include "4C_discretization_fem_general_utils_local_connectivity_matrices.hpp"
#include "4C_fluid_ele_parameter_std.hpp"
#include "4C_fluid_ele_parameter_timint.hpp"
#include "4C_utils_singleton_owner.hpp"

FOUR_C_NAMESPACE_OPEN

namespace DRT
{
  class Discretization;

  namespace ELEMENTS
  {
    class FluidBoundary;

    /// Interface base class for FluidBoundaryParent
    /*!
      This class exists to provide a common interface for all template
      versions of FluidBoundaryParent. The only function
      this class actually defines is Impl, which returns a pointer to
      the appropriate version of FluidBoundaryImpl.
    */
    class FluidBoundaryParentInterface
    {
     public:
      //! empty constructor
      FluidBoundaryParentInterface() {}

      //! empty destructor
      virtual ~FluidBoundaryParentInterface() = default;
      virtual void FlowDepPressureBC(DRT::ELEMENTS::FluidBoundary* ele1,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseVector::Base& elevec1) = 0;

      virtual void SlipSuppBC(DRT::ELEMENTS::FluidBoundary* ele1, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseVector::Base& elevec1) = 0;

      virtual void NavierSlipBC(DRT::ELEMENTS::FluidBoundary* ele1, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseVector::Base& elevec1) = 0;

      virtual void EvaluateWeakDBC(DRT::ELEMENTS::FluidBoundary* ele1,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseVector::Base& elevec1) = 0;

      virtual void estimate_nitsche_trace_max_eigenvalue(DRT::FaceElement* ele1,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseMatrix::Base& elemat2) = 0;

      virtual void MixHybDirichlet(DRT::ELEMENTS::FluidBoundary* ele1,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseVector::Base& elevec1) = 0;

      /// Internal implementation class for FluidBoundaryParent elements
      static FluidBoundaryParentInterface* Impl(DRT::FaceElement* ele);
    };


    /// Internal FluidBoundaryParent element implementation
    /*!
      This internal class keeps all the working arrays needed to
      calculate the FluidBoundaryParent element. Additionally the method Sysmat()
      provides a clean and fast element implementation.

    */
    template <CORE::FE::CellType distype>
    class FluidBoundaryParent : public FluidBoundaryParentInterface
    {
      friend class FluidEleParameter;

     public:
      //! Singleton access method
      static FluidBoundaryParentInterface* Instance(
          CORE::UTILS::SingletonAction action = CORE::UTILS::SingletonAction::create);

      /// Constructor with number of nodes
      FluidBoundaryParent();

      void FlowDepPressureBC(DRT::ELEMENTS::FluidBoundary* surfele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec) override;

      void SlipSuppBC(DRT::ELEMENTS::FluidBoundary* surfele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec) override;

      void NavierSlipBC(DRT::ELEMENTS::FluidBoundary* surfele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec) override;

      void EvaluateWeakDBC(DRT::ELEMENTS::FluidBoundary* surfele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec) override;

      void estimate_nitsche_trace_max_eigenvalue(DRT::FaceElement* surfele,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat1,
          CORE::LINALG::SerialDenseMatrix::Base& elemat2) override;

      void MixHybDirichlet(DRT::ELEMENTS::FluidBoundary* surfele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec) override;


     private:
      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void FlowDepPressureBC(DRT::ELEMENTS::FluidBoundary* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec);

      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void SlipSuppBC(DRT::ELEMENTS::FluidBoundary* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec);

      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void NavierSlipBC(DRT::ELEMENTS::FluidBoundary* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec);

      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void EvaluateWeakDBC(DRT::ELEMENTS::FluidBoundary* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec);

      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void estimate_nitsche_trace_max_eigenvalue(DRT::FaceElement* ele,
          Teuchos::ParameterList& params, DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat_epetra1,
          CORE::LINALG::SerialDenseMatrix::Base& elemat_epetra2);

      template <CORE::FE::CellType bdistype, CORE::FE::CellType pdistype>
      void MixHybDirichlet(DRT::ELEMENTS::FluidBoundary* ele, Teuchos::ParameterList& params,
          DRT::Discretization& discretization, std::vector<int>& lm,
          CORE::LINALG::SerialDenseMatrix::Base& elemat,
          CORE::LINALG::SerialDenseVector::Base& elevec);

      void get_density_and_viscosity(Teuchos::RCP<const CORE::MAT::Material> material,
          const double pscaaf, const double thermpressaf, const double rateofstrain);


      //! pointer to parameter list
      DRT::ELEMENTS::FluidEleParameter* fldpara_;
      DRT::ELEMENTS::FluidEleParameterTimInt* fldparatimint_;


      //! infinitesimal area element drs
      double drs_;
      //! integration factor
      double fac_;
      //! physical viscosity
      double visc_;
      //! density at t_(n+alpha_F) or t_(n+1)
      double densaf_;

      //-----------------------------------------------------------------
      //-----------------------------------------------------------------
      //
      //                    SPALDINGS LAW OF THE WALL
      //
      //-----------------------------------------------------------------
      //-----------------------------------------------------------------
      //-----------------------------------------------------------------
      //  evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double spalding_residual(
          const double y, const double visc, const double tau_B, const double normu)
      {
        // get dimensionless velocity
        const double up = sqrt(normu / tau_B);

        // constants
        const double chi = 0.4;
        const double B = 5.5;

        //      +
        // get y , a dimensionless boundary layer thickness
        const double yp = y * sqrt(tau_B * normu) / visc;

        return (yp - (up + exp(-chi * B) *
                               (exp(chi * up) - 1.0 -
                                   chi * up * (1 + chi * up / 2.0 * (1.0 + chi * up / 3.0)))));
      }

      //-----------------------------------------------------------------
      //  evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double spalding_residual_utau(
          const double y, const double visc, const double utau, const double normu)
      {
        // get dimensionless velocity
        if (abs(utau) < 1.0E-14) FOUR_C_THROW("utau is zero!");
        const double up = normu / utau;

        // constants
        const double chi = 0.4;
        const double B = 5.5;

        //      +
        // get y , a dimensionless boundary layer thickness
        if (visc < 1.0E-14) FOUR_C_THROW("visc is zero or negative!");
        const double yp = y * utau / visc;

        return (yp - (up + exp(-chi * B) *
                               (exp(chi * up) - 1.0 -
                                   chi * up * (1 + chi * up / 2.0 * (1.0 + chi * up / 3.0)))));
      }

      //-----------------------------------------------------------------
      //     evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double jacobian_spalding_residual(
          const double y, const double visc, const double tau_B, const double normu)
      {
        // constants
        const double chi = 0.4;
        const double B = 5.5;

        // get dimensionless velocity
        const double up = sqrt(normu / tau_B);

        // compute the derivative of the Spalding residual w.r.t. tau_B
        double drdtauB = y / (2.0 * visc * sqrt(tau_B)) * sqrt(normu);

        drdtauB +=
            (1 + chi * exp(-chi * B) * (exp(chi * up) - 1.0 - chi * up * (1.0 + 0.5 * chi * up))) *
            0.5 * sqrt(normu) / (sqrt(tau_B) * sqrt(tau_B) * sqrt(tau_B));

        return (drdtauB);
      }

      //-----------------------------------------------------------------
      //     evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double jacobian_spalding_residual_utau(
          const double y, const double visc, const double utau, const double normu)
      {
        // get dimensionless velocity
        const double up = normu / utau;

        // constants
        const double chi = 0.4;
        const double B = 5.5;

        //                    +
        // get derivative of y , a dimensionless boundary layer thickness
        const double dyplus_dutau = y / visc;

        //                                   +
        // get derivative of function f wrt u
        const double df_duplus = 1 + exp(-chi * B) * (chi * exp(chi * up) - chi - chi * chi * up -
                                                         chi * chi * chi * up * up / 2.0);

        //                    +
        // get derivative of u  wrt u_tau
        const double duplus_dutau = -normu / (utau * utau);

        return (dyplus_dutau - df_duplus * duplus_dutau);
      }

      //-----------------------------------------------------------------
      //     evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double jacobian_spalding_residual_u(
          const double y, const double visc, const double utau, const double normu)
      {
        // get dimensionless velocity
        const double up = normu / utau;

        // constants
        const double chi = 0.4;
        const double B = 5.5;

        //                    +
        // get derivative of y , a dimensionless boundary layer thickness
        const double dyplus_du = 0.0;

        //                                   +
        // get derivative of function f wrt u
        const double df_duplus = 1 + exp(-chi * B) * (chi * exp(chi * up) - chi - chi * chi * up -
                                                         chi * chi * chi * up * up / 2.0);

        //                    +
        // get derivative of u  wrt u
        const double duplus_du = 1.0 / utau;

        return (dyplus_du - df_duplus * duplus_du);
      }

      //-----------------------------------------------------------------
      //     evaluate the residual of Spaldings law of the wall
      //                                           (private) gammi 11/09
      //-----------------------------------------------------------------
      double jacobian_spalding_residual_uplus(
          const double y, const double visc, const double utau, const double normu)
      {
        // get dimensionless velocity
        const double up = normu / utau;

        // constants
        const double chi = 0.4;
        const double B = 5.5;

        //                    +
        // get derivative of y , a dimensionless boundary layer thickness
        const double dyplus_duplus = 0.0;
        /*
        // Spaldings law of the wall
        //                     /                                                  \
        //                    |                           /     +\ 2    /     +\ 3 |
        //                    |       +                  | chi*u  |    | chi*u  |  |
        //  +   +    -chi*B   |  chi*u               +    \      /      \      /   |
        // y - u  - e       * | e       - 1.0 - chi*u  - ----------- - ----------- | = 0
        //                    |                              2.0           6.0     |
        //                    |                                                    |
        //                     \                                                  /
        */

        //                                   +
        // get derivative of function f wrt u
        const double df_duplus = 1 + exp(-chi * B) * (chi * exp(chi * up) - chi - chi * chi * up -
                                                         chi * chi * chi * up * up / 2.0);

        return (dyplus_duplus - df_duplus);
      }
    };  // end class fluid3boundaryImpl
  }     // namespace ELEMENTS
}  // namespace DRT

FOUR_C_NAMESPACE_CLOSE

#endif