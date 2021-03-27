/*----------------------------------------------------------------------*/
/*! \file
 \brief Evaluates flux between ScaTra and ScaTra on manifolds incl. coupling matrices

 \level 2


 *------------------------------------------------------------------------------------------------*/

#include "ssi_manifold_flux_evaluator.H"

#include "ssi_monolithic.H"
#include "ssi_utils.H"

#include "../drt_adapter/adapter_coupling.H"
#include "../drt_adapter/adapter_scatra_base_algorithm.H"

#include "../drt_inpar/inpar_s2i.H"
#include "../drt_inpar/inpar_ssi.H"

#include "../drt_lib/drt_assemblestrategy.H"
#include "../drt_lib/drt_condition_utils.H"
#include "../drt_lib/drt_utils_gid_vector.H"

#include "../drt_scatra/scatra_timint_implicit.H"

#include "../drt_scatra_ele/scatra_ele_action.H"

#include "../linalg/linalg_matrixtransform.H"
#include "../linalg/linalg_utils_sparse_algebra_create.H"

/*---------------------------------------------------------------------------------*
 *---------------------------------------------------------------------------------*/
SSI::ManifoldScaTraCoupling::ManifoldScaTraCoupling(Teuchos::RCP<DRT::Discretization> manifolddis,
    Teuchos::RCP<DRT::Discretization> scatradis, DRT::Condition* condition_manifold,
    DRT::Condition* condition_kinetics)
    : condition_kinetics_(condition_kinetics),
      condition_manifold_(condition_manifold),
      coupling_adapter_(Teuchos::rcp(new ADAPTER::Coupling())),
      manifold_conditionID_(condition_manifold->GetInt("ConditionID")),
      manifold_map_extractor_(Teuchos::null),
      scatra_map_extractor_(Teuchos::null),
      slave_converter_(Teuchos::null)
{
  std::vector<int> inodegidvec_manifold;
  DRT::UTILS::AddOwnedNodeGIDVector(
      manifolddis, *condition_manifold->Nodes(), inodegidvec_manifold);

  std::vector<int> inodegidvec_scatra;
  DRT::UTILS::AddOwnedNodeGIDVector(scatradis, *condition_kinetics->Nodes(), inodegidvec_scatra);

  coupling_adapter_->SetupCoupling(
      *scatradis, *manifolddis, inodegidvec_scatra, inodegidvec_manifold, 2, true, 1.0e-8);
  slave_converter_ = Teuchos::rcp(new ADAPTER::CouplingSlaveConverter(*coupling_adapter_));

  scatra_map_extractor_ = Teuchos::rcp(
      new LINALG::MapExtractor(*scatradis->DofRowMap(), coupling_adapter_->MasterDofMap(), true));

  manifold_map_extractor_ = Teuchos::rcp(
      new LINALG::MapExtractor(*manifolddis->DofRowMap(), coupling_adapter_->SlaveDofMap(), true));
}

/*---------------------------------------------------------------------------------*
 *---------------------------------------------------------------------------------*/
SSI::ScaTraManifoldScaTraFluxEvaluator::ScaTraManifoldScaTraFluxEvaluator(
    const SSI::SSIMono& ssi_mono)
    : block_map_scatra_(ssi_mono.MapsScatra()),
      block_map_structure_(ssi_mono.MapStructure()),
      full_map_manifold_(
          ssi_mono.MapsSubProblems()->Map(ssi_mono.GetProblemPosition(Subproblem::manifold))),
      full_map_scatra_(ssi_mono.MapsSubProblems()->Map(
          ssi_mono.GetProblemPosition(Subproblem::scalar_transport))),
      full_map_structure_(
          ssi_mono.MapsSubProblems()->Map(ssi_mono.GetProblemPosition(Subproblem::structure))),
      icoup_structure_(ssi_mono.InterfaceCouplingAdapterStructure()),
      matrix_manifold_scatra_(Teuchos::null),
      matrix_manifold_structure_(Teuchos::null),
      matrix_scatra_manifold_(Teuchos::null),
      matrix_scatra_structure_(Teuchos::null),
      rhs_manifold_(Teuchos::null),
      rhs_scatra_(Teuchos::null),
      scatra_(ssi_mono.ScaTraBaseAlgorithm()),
      scatra_manifold_(ssi_mono.ScaTraManifoldBaseAlgorithm()),
      scatra_manifold_couplings_(Teuchos::null),
      systemmatrix_manifold_(Teuchos::null),
      systemmatrix_scatra_(Teuchos::null)
{
  std::vector<DRT::Condition*> conditions_manifold;
  scatra_manifold_->ScaTraField()->Discretization()->GetCondition(
      "SSISurfaceManifold", conditions_manifold);

  std::vector<DRT::Condition*> conditions_manifold_kinetics_scatra;
  scatra_->ScaTraField()->Discretization()->GetCondition(
      "SSISurfaceManifoldKinetics", conditions_manifold_kinetics_scatra);

  // create pair: manifold condition - kinetics condition
  for (const auto& condition_manifold : conditions_manifold)
  {
    for (const auto& condition_kinetics : conditions_manifold_kinetics_scatra)
    {
      if (condition_manifold->GetInt("ConditionID") ==
          condition_kinetics->GetInt("ManifoldConditionID"))
      {
        scatra_manifold_couplings_.emplace_back(Teuchos::rcp(
            new SSI::ManifoldScaTraCoupling(scatra_manifold_->ScaTraField()->Discretization(),
                scatra_->ScaTraField()->Discretization(), condition_manifold, condition_kinetics)));
      }
    }
  }

  rhs_manifold_ = LINALG::CreateVector(*full_map_manifold_, true);
  rhs_scatra_ = LINALG::CreateVector(*full_map_scatra_, true);

  switch (scatra_->ScaTraField()->MatrixType())
  {
    case LINALG::MatrixType::block_condition:
    case LINALG::MatrixType::block_condition_dof:
    {
      auto scatramanifoldblockmaps =
          Teuchos::rcpFromRef(scatra_manifold_->ScaTraField()->BlockMaps());

      systemmatrix_manifold_ = SSI::UTILS::SSIMatrices::SetupBlockMatrix(
          scatramanifoldblockmaps, scatramanifoldblockmaps);
      systemmatrix_scatra_ =
          SSI::UTILS::SSIMatrices::SetupBlockMatrix(block_map_scatra_, block_map_scatra_);
      matrix_manifold_structure_ =
          SSI::UTILS::SSIMatrices::SetupBlockMatrix(scatramanifoldblockmaps, block_map_structure_);
      matrix_manifold_scatra_ =
          SSI::UTILS::SSIMatrices::SetupBlockMatrix(scatramanifoldblockmaps, block_map_scatra_);
      matrix_scatra_manifold_ =
          SSI::UTILS::SSIMatrices::SetupBlockMatrix(block_map_scatra_, scatramanifoldblockmaps);
      matrix_scatra_structure_ =
          SSI::UTILS::SSIMatrices::SetupBlockMatrix(block_map_scatra_, block_map_structure_);

      break;
    }
    case LINALG::MatrixType::sparse:
    {
      systemmatrix_manifold_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
      systemmatrix_scatra_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);
      matrix_manifold_structure_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
      matrix_manifold_scatra_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
      matrix_scatra_manifold_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);
      matrix_scatra_structure_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);

      break;
    }
    default:
    {
      dserror("Invalid matrix type associated with scalar transport field!");
      break;
    }
  }

  rhs_manifold_cond_ = LINALG::CreateVector(*full_map_manifold_, true);
  rhs_scatra_cond_ = LINALG::CreateVector(*full_map_scatra_, true);

  systemmatrix_manifold_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
  systemmatrix_scatra_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);
  matrix_manifold_scatra_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
  matrix_manifold_structure_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_manifold_);
  matrix_scatra_manifold_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);
  matrix_scatra_structure_cond_ = SSI::UTILS::SSIMatrices::SetupSparseMatrix(full_map_scatra_);
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void SSI::ScaTraManifoldScaTraFluxEvaluator::Evaluate()
{
  // clear matrices and rhs from last evaluation
  systemmatrix_manifold_->Zero();
  systemmatrix_scatra_->Zero();
  matrix_manifold_scatra_->Zero();
  matrix_manifold_structure_->Zero();
  matrix_scatra_manifold_->Zero();
  matrix_scatra_structure_->Zero();

  rhs_manifold_->PutScalar(0.0);
  rhs_scatra_->PutScalar(0.0);

  // evaluate all scatra-manifold coupling conditions
  for (const auto& scatra_manifold_coupling : scatra_manifold_couplings_)
  {
    // clear matrices and rhs from last condition
    systemmatrix_manifold_cond_->Zero();
    systemmatrix_manifold_cond_->UnComplete();
    systemmatrix_scatra_cond_->Zero();
    systemmatrix_scatra_cond_->UnComplete();
    matrix_manifold_scatra_cond_->Zero();
    matrix_manifold_scatra_cond_->UnComplete();
    matrix_manifold_structure_cond_->Zero();
    matrix_manifold_structure_cond_->UnComplete();
    matrix_scatra_manifold_cond_->Zero();
    matrix_scatra_manifold_cond_->UnComplete();
    matrix_scatra_structure_cond_->Zero();
    matrix_scatra_structure_cond_->UnComplete();

    rhs_manifold_cond_->PutScalar(0.0);
    rhs_scatra_cond_->PutScalar(0.0);

    EvaluateManifoldSide(scatra_manifold_coupling);

    CopyScaTraManifoldScaTraMasterSide(scatra_manifold_coupling);

    AddConditionContribution();
  }

  systemmatrix_manifold_->Complete();
  systemmatrix_scatra_->Complete();

  switch (scatra_->ScaTraField()->MatrixType())
  {
    case LINALG::MatrixType::block_condition:
    case LINALG::MatrixType::block_condition_dof:
    {
      matrix_manifold_scatra_->Complete();
      matrix_manifold_structure_->Complete();
      matrix_scatra_manifold_->Complete();
      matrix_scatra_structure_->Complete();

      break;
    }
    case LINALG::MatrixType::sparse:
    {
      matrix_manifold_scatra_->Complete(*full_map_scatra_, *full_map_manifold_);
      matrix_manifold_structure_->Complete(*full_map_structure_, *full_map_manifold_);
      matrix_scatra_manifold_->Complete(*full_map_manifold_, *full_map_scatra_);
      matrix_scatra_structure_->Complete(*full_map_structure_, *full_map_scatra_);

      break;
    }
    default:
    {
      dserror("Invalid matrix type associated with scalar transport field!");
      break;
    }
  }
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void SSI::ScaTraManifoldScaTraFluxEvaluator::EvaluateManifoldSide(
    Teuchos::RCP<ManifoldScaTraCoupling> scatra_manifold_coupling)
{
  // First: Set parameters to elements
  {
    Teuchos::ParameterList eleparams;

    eleparams.set<int>("action", SCATRA::set_elch_scatra_manifold_parameter);

    eleparams.set<bool>("evaluate_master_side",
        !(DRT::UTILS::HaveSameNodes(scatra_manifold_coupling->ConditionManifold(),
            scatra_manifold_coupling->ConditionKinetics(), false)));

    eleparams.set<int>(
        "kinetic_model", scatra_manifold_coupling->ConditionKinetics()->GetInt("KineticModel"));

    if (scatra_manifold_coupling->ConditionKinetics()->GetInt("KineticModel") ==
        INPAR::SSI::kinetics_constantinterfaceresistance)
    {
      eleparams.set<int>(
          "num_electrons", scatra_manifold_coupling->ConditionKinetics()->GetInt("e-"));
      eleparams.set<double>(
          "resistance", scatra_manifold_coupling->ConditionKinetics()->GetDouble("resistance"));
    }

    scatra_manifold_->ScaTraField()->Discretization()->Evaluate(
        eleparams, Teuchos::null, Teuchos::null);
  }

  // Second: Evaluate condition
  {
    // manifold-scatra coupling matrix evaluated on manifold side
    auto matrix_manifold_scatra_manifold_side =
        Teuchos::rcp(new LINALG::SparseMatrix(*full_map_manifold_, 27, false, true));

    Teuchos::ParameterList condparams;

    condparams.set<int>("action", SCATRA::calc_scatra_manifold_flux);

    condparams.set<int>("ndsdisp", 1);

    scatra_manifold_->ScaTraField()->Discretization()->ClearState();

    scatra_manifold_->ScaTraField()->AddTimeIntegrationSpecificVectors();

    // Evaluation of RHS and scatra-manifold coupling matrices
    {
      condparams.set<int>(
          "differentiationtype", static_cast<int>(SCATRA::DifferentiationType::elch));

      DRT::AssembleStrategy strategymanifold(0, 0, systemmatrix_manifold_cond_,
          matrix_manifold_scatra_manifold_side, rhs_manifold_cond_, Teuchos::null, Teuchos::null);

      scatra_manifold_->ScaTraField()->Discretization()->EvaluateCondition(condparams,
          strategymanifold, "SSISurfaceManifold", scatra_manifold_coupling->ManifoldConditionID());

      systemmatrix_manifold_cond_->Complete();
      matrix_manifold_scatra_manifold_side->Complete();

      // column dofs are so far on manifold dis. They are transformed to scatra dis
      LINALG::MatrixLogicalSplitAndTransform()(*matrix_manifold_scatra_manifold_side,
          *full_map_manifold_, *full_map_scatra_, 1.0, nullptr,
          &*scatra_manifold_coupling->SlaveConverter(), *matrix_manifold_scatra_cond_, true, true);

      matrix_manifold_scatra_cond_->Complete(*full_map_scatra_, *full_map_manifold_);
    }

    // Evaluation of linearization w.r.t. displacement
    {
      condparams.set<int>(
          "differentiationtype", static_cast<int>(SCATRA::DifferentiationType::disp));

      auto flux_manifold_scatra_md_cond_slave_side_disp =
          Teuchos::rcp(new LINALG::SparseMatrix(*full_map_manifold_, 27, false, true));

      DRT::AssembleStrategy strategymanifold(0, 1, flux_manifold_scatra_md_cond_slave_side_disp,
          Teuchos::null, Teuchos::null, Teuchos::null, Teuchos::null);

      scatra_manifold_->ScaTraField()->Discretization()->EvaluateCondition(condparams,
          strategymanifold, "SSISurfaceManifold", scatra_manifold_coupling->ManifoldConditionID());

      flux_manifold_scatra_md_cond_slave_side_disp->Complete(
          *full_map_structure_, *full_map_manifold_);

      // Add slave side disp. contributions
      matrix_manifold_structure_cond_->Add(
          *flux_manifold_scatra_md_cond_slave_side_disp, false, 1.0, 1.0);

      // Add master side disp. contributions
      ADAPTER::CouplingSlaveConverter converter(*icoup_structure_);
      LINALG::MatrixLogicalSplitAndTransform()(*flux_manifold_scatra_md_cond_slave_side_disp,
          *full_map_manifold_, *full_map_structure_, 1.0, nullptr, &converter,
          *matrix_manifold_structure_cond_, true, true);

      matrix_manifold_structure_cond_->Complete(*full_map_structure_, *full_map_manifold_);
    }

    scatra_manifold_->ScaTraField()->Discretization()->ClearState();
  }
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void SSI::ScaTraManifoldScaTraFluxEvaluator::CopyScaTraManifoldScaTraMasterSide(
    Teuchos::RCP<ManifoldScaTraCoupling> scatra_manifold_coupling)
{
  {
    auto flux_manifold_scatra_m_cond_extract =
        scatra_manifold_coupling->ManifoldMapExtractor()->ExtractCondVector(rhs_manifold_cond_);

    auto flux_manifold_domain_RHS_m_cond_to_s =
        scatra_manifold_coupling->CouplingAdapter()->SlaveToMaster(
            flux_manifold_scatra_m_cond_extract);

    scatra_manifold_coupling->ScaTraMapExtractor()->AddCondVector(
        flux_manifold_domain_RHS_m_cond_to_s, rhs_scatra_cond_);
    rhs_scatra_cond_->Scale(-1.0);
  }

  // djscatra_dmanifold: manifold rows are transformed to scatra side (flux is scaled by -1.0)
  LINALG::MatrixLogicalSplitAndTransform()(*systemmatrix_manifold_cond_, *full_map_scatra_,
      *full_map_manifold_, -1.0, &*scatra_manifold_coupling->SlaveConverter(), nullptr,
      *matrix_scatra_manifold_cond_, true, true);

  // djscatra_dscatra: manifold rows are transformed to scatra side (flux is scaled by -1.0)
  LINALG::MatrixLogicalSplitAndTransform()(*matrix_manifold_scatra_cond_, *full_map_scatra_,
      *full_map_scatra_, -1.0, &*scatra_manifold_coupling->SlaveConverter(), nullptr,
      *systemmatrix_scatra_cond_, true, true);

  matrix_scatra_manifold_cond_->Complete(*full_map_manifold_, *full_map_scatra_);
  systemmatrix_scatra_cond_->Complete();

  // djscatra_dstructure: manifold rows are transformed to scatra side (flux is scaled by -1.0)
  LINALG::MatrixLogicalSplitAndTransform()(*matrix_manifold_structure_cond_, *full_map_scatra_,
      *full_map_structure_, -1.0, &*scatra_manifold_coupling->SlaveConverter(), nullptr,
      *matrix_scatra_structure_cond_, true, true);

  matrix_scatra_structure_cond_->Complete(*full_map_structure_, *full_map_scatra_);
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void SSI::ScaTraManifoldScaTraFluxEvaluator::AddConditionContribution()
{
  rhs_manifold_->Update(1.0, *rhs_manifold_cond_, 1.0);
  rhs_scatra_->Update(1.0, *rhs_scatra_cond_, 1.0);

  switch (scatra_->ScaTraField()->MatrixType())
  {
    case LINALG::MatrixType::block_condition:
    case LINALG::MatrixType::block_condition_dof:
    {
      auto blockmaps_manifold = scatra_manifold_->ScaTraField()->BlockMaps();

      auto flux_manifold_scatra_mm_block =
          systemmatrix_manifold_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              blockmaps_manifold, blockmaps_manifold);
      auto flux_manifold_scatra_md_block =
          matrix_manifold_structure_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              *block_map_structure_, blockmaps_manifold);
      auto flux_manifold_scatra_ms_block =
          matrix_manifold_scatra_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              *block_map_scatra_, blockmaps_manifold);
      auto flux_manifold_scatra_sm_block =
          matrix_scatra_manifold_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              blockmaps_manifold, *block_map_scatra_);
      auto flux_manifold_scatra_sd_block =
          matrix_scatra_structure_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              *block_map_structure_, *block_map_scatra_);
      auto flux_manifold_scatra_ss_block =
          systemmatrix_scatra_cond_->Split<LINALG::DefaultBlockMatrixStrategy>(
              *block_map_scatra_, *block_map_scatra_);

      flux_manifold_scatra_mm_block->Complete();
      flux_manifold_scatra_md_block->Complete();
      flux_manifold_scatra_ms_block->Complete();
      flux_manifold_scatra_sm_block->Complete();
      flux_manifold_scatra_sd_block->Complete();
      flux_manifold_scatra_ss_block->Complete();

      systemmatrix_manifold_->Add(*flux_manifold_scatra_mm_block, false, 1.0, 1.0);
      matrix_manifold_scatra_->Add(*flux_manifold_scatra_ms_block, false, 1.0, 1.0);
      matrix_manifold_structure_->Add(*flux_manifold_scatra_md_block, false, 1.0, 1.0);
      systemmatrix_scatra_->Add(*flux_manifold_scatra_ss_block, false, 1.0, 1.0);
      matrix_scatra_manifold_->Add(*flux_manifold_scatra_sm_block, false, 1.0, 1.0);
      matrix_scatra_structure_->Add(*flux_manifold_scatra_sd_block, false, 1.0, 1.0);

      break;
    }
    case LINALG::MatrixType::sparse:
    {
      systemmatrix_manifold_->Add(*systemmatrix_manifold_cond_, false, 1.0, 1.0);
      matrix_manifold_structure_->Add(*matrix_manifold_structure_cond_, false, 1.0, 1.0);
      matrix_manifold_scatra_->Add(*matrix_manifold_scatra_cond_, false, 1.0, 1.0);
      systemmatrix_scatra_->Add(*systemmatrix_scatra_cond_, false, 1.0, 1.0);
      matrix_scatra_structure_->Add(*matrix_scatra_structure_cond_, false, 1.0, 1.0);
      matrix_scatra_manifold_->Add(*matrix_scatra_manifold_cond_, false, 1.0, 1.0);

      break;
    }
    default:
    {
      dserror("Invalid matrix type associated with scalar transport field!");
      break;
    }
  }
}