/*---------------------------------------------------------------------*/
/*! \file

\brief Evaluation and assembly of all 0D cardiovascular model terms


\level 3

*/
/*---------------------------------------------------------------------*/

#ifndef BACI_CARDIOVASCULAR0D_STRUCTURE_NEW_MODEL_EVALUATOR_HPP
#define BACI_CARDIOVASCULAR0D_STRUCTURE_NEW_MODEL_EVALUATOR_HPP

#include "baci_config.hpp"

#include "baci_cardiovascular0d_manager.hpp"
#include "baci_structure_new_model_evaluator_generic.hpp"

#include <Teuchos_RCP.hpp>

BACI_NAMESPACE_OPEN

// forward declarations
namespace CORE::LINALG
{
  class SparseMatrix;
}  // namespace CORE::LINALG

namespace STR
{
  namespace MODELEVALUATOR
  {
    class Cardiovascular0D : public Generic
    {
     public:
      //! constructor
      Cardiovascular0D();


      void Setup() override;

      //! derived
      INPAR::STR::ModelType Type() const override { return INPAR::STR::model_cardiovascular0d; }

      //! derived
      void Reset(const Epetra_Vector& x) override;

      //! derived
      bool EvaluateForce() override;

      //! derived
      bool EvaluateStiff() override;

      //! derived
      bool EvaluateForceStiff() override;

      //! derived
      void PreEvaluate() override { return; };

      //! derived
      void PostEvaluate() override { return; };

      //! derived
      bool AssembleForce(Epetra_Vector& f, const double& timefac_np) const override;

      //! derived
      bool AssembleJacobian(
          CORE::LINALG::SparseOperator& jac, const double& timefac_np) const override;

      //! derived
      void WriteRestart(
          IO::DiscretizationWriter& iowriter, const bool& forced_writerestart) const override;

      //! derived
      void ReadRestart(IO::DiscretizationReader& ioreader) override;

      //! [derived]
      void Predict(const INPAR::STR::PredEnum& pred_type) override { return; };

      //! derived
      void RunPreComputeX(const Epetra_Vector& xold, Epetra_Vector& dir_mutable,
          const NOX::NLN::Group& curr_grp) override
      {
        return;
      };

      //! derived
      void RunPostComputeX(
          const Epetra_Vector& xold, const Epetra_Vector& dir, const Epetra_Vector& xnew) override;

      //! derived
      void RunPostIterate(const ::NOX::Solver::Generic& solver) override { return; };

      //! derived
      void UpdateStepState(const double& timefac_n) override;

      //! derived
      void UpdateStepElement() override;

      //! derived
      void DetermineStressStrain() override;

      //! derived
      void DetermineEnergy() override;

      //! derived
      void DetermineOptionalQuantity() override;

      //! derived
      void OutputStepState(IO::DiscretizationWriter& iowriter) const override;

      //! derived
      void ResetStepState() override;

      //! [derived]
      void PostOutput() override;

      //! derived
      Teuchos::RCP<const Epetra_Map> GetBlockDofRowMapPtr() const override;

      //! derived
      Teuchos::RCP<const Epetra_Vector> GetCurrentSolutionPtr() const override;

      //! derived
      Teuchos::RCP<const Epetra_Vector> GetLastTimeStepSolutionPtr() const override;

     private:
      Teuchos::RCP<UTILS::Cardiovascular0DManager> cardvasc0dman_;  //!< Cardiovascular0D manager

      //! structural displacement at \f$t_{n+1}\f$
      Teuchos::RCP<const Epetra_Vector> disnp_ptr_;

      //! cardio contributions to the structural stiffness matrix
      Teuchos::RCP<CORE::LINALG::SparseMatrix> stiff_cardio_ptr_;

      //! structural rhs contributions of the cardio model at \f$t_{n+1}\f$
      Teuchos::RCP<Epetra_Vector> fstructcardio_np_ptr_;
    };

  }  // namespace MODELEVALUATOR
}  // namespace STR


BACI_NAMESPACE_CLOSE

#endif  // CARDIOVASCULAR0D_STRUCTURE_NEW_MODEL_EVALUATOR_H