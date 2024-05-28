/*-----------------------------------------------------------*/
/*! \file

\brief Factory to build the desired linear solver std::map corresponding to the active model types


\level 3

*/
/*-----------------------------------------------------------*/


#ifndef FOUR_C_STRUCTURE_NEW_SOLVER_FACTORY_HPP
#define FOUR_C_STRUCTURE_NEW_SOLVER_FACTORY_HPP

#include "4C_config.hpp"

#include <Teuchos_RCP.hpp>

#include <set>

namespace Teuchos
{
  class ParameterList;
}

FOUR_C_NAMESPACE_OPEN

// forward declarations...
namespace INPAR
{
  namespace STR
  {
    enum ModelType : int;
  }
  namespace CONTACT
  {
    enum SolvingStrategy : int;
    enum SystemType : int;
  }  // namespace CONTACT
}  // namespace INPAR
namespace CORE::LINALG
{
  class Solver;
}
namespace DRT
{
  class Discretization;
}
namespace STR
{
  namespace SOLVER
  {
    /*! \brief Factory to build the desired linear solver std::map corresponding
     *  to the active model types.
     *
     *  \author Michael Hiermeier */
    class Factory
    {
     private:
      typedef std::map<enum INPAR::STR::ModelType, Teuchos::RCP<CORE::LINALG::Solver>> LinSolMap;

     public:
      //! constructor
      Factory();

      //! destructor
      virtual ~Factory() = default;

      //! build the desired linear solvers
      Teuchos::RCP<LinSolMap> BuildLinSolvers(
          const std::set<enum INPAR::STR::ModelType>& modeltypes,
          const Teuchos::ParameterList& sdyn, DRT::Discretization& actdis) const;

      //! create the meshtying/contact linear solver
      static Teuchos::RCP<CORE::LINALG::Solver> build_meshtying_contact_lin_solver(
          DRT::Discretization& actdis, enum INPAR::CONTACT::SolvingStrategy sol_type,
          enum INPAR::CONTACT::SystemType sys_type, const int lin_solver_id);

     private:
      //! create the structural linear solver (should be called by default)
      Teuchos::RCP<CORE::LINALG::Solver> build_structure_lin_solver(
          const Teuchos::ParameterList& sdyn, DRT::Discretization& actdis) const;

      //! create the meshtying/contact linear solver
      Teuchos::RCP<CORE::LINALG::Solver> build_meshtying_contact_lin_solver(
          DRT::Discretization& actdis) const;

      //! create the Lagrange/penalty enforced constraint linear solver
      Teuchos::RCP<CORE::LINALG::Solver> build_lag_pen_constraint_lin_solver(
          const Teuchos::ParameterList& sdyn, DRT::Discretization& actdis) const;

      //! create the Windkessel linear solver
      Teuchos::RCP<CORE::LINALG::Solver> build_cardiovascular0_d_lin_solver(
          const Teuchos::ParameterList& sdyn, DRT::Discretization& actdis) const;

    };  // class Factory

    /*! Non-member function, which relates to the STR::SOLVER::Factory class
     *  Please call this method from outside! */
    Teuchos::RCP<std::map<enum INPAR::STR::ModelType, Teuchos::RCP<CORE::LINALG::Solver>>>
    BuildLinSolvers(const std::set<enum INPAR::STR::ModelType>& modeltypes,
        const Teuchos::ParameterList& sdyn, DRT::Discretization& actdis);
  }  // namespace SOLVER
}  // namespace STR

FOUR_C_NAMESPACE_CLOSE

#endif