// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_ssi_monolithic_dbc_handler.hpp"

#include "4C_adapter_str_ssiwrapper.hpp"
#include "4C_coupling_adapter_converter.hpp"
#include "4C_fem_condition_locsys.hpp"
#include "4C_linalg_blocksparsematrix.hpp"
#include "4C_linalg_utils_sparse_algebra_assemble.hpp"
#include "4C_linalg_utils_sparse_algebra_math.hpp"
#include "4C_scatra_timint_implicit.hpp"
#include "4C_ssi_monolithic.hpp"
#include "4C_ssi_utils.hpp"

FOUR_C_NAMESPACE_OPEN

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
SSI::DBCHandlerBase::DBCHandlerBase(const bool is_scatra_manifold,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_manifold,
    std::shared_ptr<SSI::Utils::SSIMaps> ssi_maps,
    std::shared_ptr<Adapter::SSIStructureWrapper> structure)
    : is_scatra_manifold_(is_scatra_manifold),
      scatra_(std::move(scatra)),
      scatra_manifold_(std::move(scatra_manifold)),
      ssi_maps_(std::move(ssi_maps)),
      structure_(std::move(structure))
{
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
SSI::DBCHandlerSparse::DBCHandlerSparse(const bool is_scatra_manifold,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_manifold,
    std::shared_ptr<SSI::Utils::SSIMaps> ssi_maps,
    std::shared_ptr<Adapter::SSIStructureWrapper> structure)
    : DBCHandlerBase(is_scatra_manifold, scatra, scatra_manifold, ssi_maps, structure)
{
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
SSI::DBCHandlerBlock::DBCHandlerBlock(const bool is_scatra_manifold,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_manifold,
    std::shared_ptr<SSI::Utils::SSIMaps> ssi_maps,
    std::shared_ptr<Adapter::SSIStructureWrapper> structure)
    : DBCHandlerBase(is_scatra_manifold, scatra, scatra_manifold, ssi_maps, structure),
      position_structure_(ssi_maps->get_block_positions(SSI::Subproblem::structure).at(0))
{
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
void SSI::DBCHandlerBase::apply_dbc_to_rhs(std::shared_ptr<Core::LinAlg::Vector<double>> rhs)
{
  // apply Dirichlet boundary conditions to the structure part of the right hand side
  const auto& locsysmanager_structure = structure_field()->locsys_manager();
  auto rhs_struct = ssi_maps()->maps_sub_problems()->extract_vector(
      *rhs, Utils::SSIMaps::get_problem_position(SSI::Subproblem::structure));
  const auto zeros_struct = std::make_shared<Core::LinAlg::Vector<double>>(
      *structure_field()->get_dbc_map_extractor()->cond_map());

  if (locsysmanager_structure != nullptr)
    locsysmanager_structure->rotate_global_to_local(*rhs_struct);
  Core::LinAlg::apply_dirichlet_to_system(
      *rhs_struct, *zeros_struct, *structure_field()->get_dbc_map_extractor()->cond_map());
  if (locsysmanager_structure != nullptr)
    locsysmanager_structure->rotate_local_to_global(*rhs_struct);

  ssi_maps()->maps_sub_problems()->insert_vector(
      *rhs_struct, Utils::SSIMaps::get_problem_position(SSI::Subproblem::structure), *rhs);

  // apply Dirichlet boundary conditions to the scatra part of the right hand side
  const auto zeros_scatra =
      std::make_shared<Core::LinAlg::Vector<double>>(*scatra_field()->dirich_maps()->cond_map());
  Core::LinAlg::apply_dirichlet_to_system(
      *rhs, *zeros_scatra, *scatra_field()->dirich_maps()->cond_map());

  // apply Dirichlet boundary conditions to the scatra manifold part of the right hand side
  if (is_scatra_manifold())
  {
    Core::LinAlg::Vector<double> zeros_scatramanifold(
        *scatra_manifold_field()->dirich_maps()->cond_map());
    Core::LinAlg::apply_dirichlet_to_system(
        *rhs, zeros_scatramanifold, *scatra_manifold_field()->dirich_maps()->cond_map());
  }
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
void SSI::DBCHandlerBase::apply_dbc_to_system_matrix(
    std::shared_ptr<Core::LinAlg::SparseOperator> system_matrix)
{
  // apply the scalar transport Dirichlet boundary conditions to the global system matrix
  system_matrix->apply_dirichlet(*scatra_field()->dirich_maps()->cond_map(), true);

  // apply the scalar transport on manifolds Dirichlet boundary conditions to the global system
  // matrix
  if (is_scatra_manifold())
    system_matrix->apply_dirichlet(*scatra_manifold_field()->dirich_maps()->cond_map(), true);

  // apply the structure Dirichlet boundary conditions to the global system matrix
  apply_structure_dbc_to_system_matrix(system_matrix);
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
void SSI::DBCHandlerBase::apply_structure_dbc_to_system_matrix(
    std::shared_ptr<Core::LinAlg::SparseOperator> system_matrix)
{
  // locsys manager of structure
  const auto& locsysmanager_structure = structure_field()->locsys_manager();

  // map of structure Dirichlet BCs
  const auto& dbcmap_structure = structure_field()->get_dbc_map_extractor()->cond_map();

  if (locsysmanager_structure == nullptr)
    system_matrix->apply_dirichlet(*dbcmap_structure);
  else
    apply_structure_dbc_with_loc_sys_rotation_to_system_matrix(
        system_matrix, dbcmap_structure, locsysmanager_structure);
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
void SSI::DBCHandlerSparse::apply_structure_dbc_with_loc_sys_rotation_to_system_matrix(
    std::shared_ptr<Core::LinAlg::SparseOperator> system_matrix,
    const std::shared_ptr<const Core::LinAlg::Map>& dbcmap_structure,
    std::shared_ptr<const Core::Conditions::LocsysManager> locsysmanager_structure)

{
  auto systemmatrix_sparse = Core::LinAlg::cast_to_sparse_matrix_and_check_success(system_matrix);

  // structure dof row map
  const auto& dofrowmap_structure = structure_field()->dof_row_map();

  // extract structure rows of global system matrix
  const auto systemmatrix_structure =
      std::make_shared<Core::LinAlg::SparseMatrix>(*dofrowmap_structure, 27, false, true);
  Coupling::Adapter::MatrixLogicalSplitAndTransform()(*systemmatrix_sparse, *dofrowmap_structure,
      Core::LinAlg::Map(system_matrix->domain_map()), 1.0, nullptr, nullptr,
      *systemmatrix_structure);
  systemmatrix_structure->complete(system_matrix->domain_map(), *dofrowmap_structure);

  // apply structure Dirichlet conditions
  locsysmanager_structure->rotate_global_to_local(systemmatrix_structure);
  systemmatrix_structure->apply_dirichlet_with_trafo(
      *locsysmanager_structure->trafo(), *dbcmap_structure);
  locsysmanager_structure->rotate_local_to_global(*systemmatrix_structure);

  // assemble structure rows of global system matrix back into global system matrix
  Core::LinAlg::matrix_put(*systemmatrix_structure, 1.0, dofrowmap_structure, *systemmatrix_sparse);
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
void SSI::DBCHandlerBlock::apply_structure_dbc_with_loc_sys_rotation_to_system_matrix(
    std::shared_ptr<Core::LinAlg::SparseOperator> system_matrix,
    const std::shared_ptr<const Core::LinAlg::Map>& dbcmap_structure,
    std::shared_ptr<const Core::Conditions::LocsysManager> locsysmanager_structure)
{
  auto systemmatrix_block =
      Core::LinAlg::cast_to_block_sparse_matrix_base_and_check_success(system_matrix);

  // apply structure Dirichlet conditions
  for (int iblock = 0; iblock < systemmatrix_block->cols(); ++iblock)
  {
    locsysmanager_structure->rotate_global_to_local(
        Core::Utils::shared_ptr_from_ref(systemmatrix_block->matrix(position_structure(), iblock)));
    systemmatrix_block->matrix(position_structure(), iblock)
        .apply_dirichlet_with_trafo(
            *locsysmanager_structure->trafo(), *dbcmap_structure, (iblock == position_structure()));
    locsysmanager_structure->rotate_local_to_global(
        systemmatrix_block->matrix(position_structure(), iblock));
  }
}

/*-------------------------------------------------------------------------*
 *-------------------------------------------------------------------------*/
std::shared_ptr<SSI::DBCHandlerBase> SSI::build_dbc_handler(const bool is_scatra_manifold,
    Core::LinAlg::MatrixType matrixtype_ssi, std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra,
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_manifold,
    std::shared_ptr<SSI::Utils::SSIMaps> ssi_maps,
    std::shared_ptr<Adapter::SSIStructureWrapper> structure)
{
  std::shared_ptr<SSI::DBCHandlerBase> dbc_handler = nullptr;

  switch (matrixtype_ssi)
  {
    case Core::LinAlg::MatrixType::block_field:
    {
      dbc_handler = std::make_shared<SSI::DBCHandlerBlock>(
          is_scatra_manifold, scatra, scatra_manifold, ssi_maps, structure);
      break;
    }
    case Core::LinAlg::MatrixType::sparse:
    {
      dbc_handler = std::make_shared<SSI::DBCHandlerSparse>(
          is_scatra_manifold, scatra, scatra_manifold, ssi_maps, structure);
      break;
    }
    default:
    {
      FOUR_C_THROW("unknown matrix type of SSI problem");
      break;
    }
  }

  return dbc_handler;
}
FOUR_C_NAMESPACE_CLOSE
