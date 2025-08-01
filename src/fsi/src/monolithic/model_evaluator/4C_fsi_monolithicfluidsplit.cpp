// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_fsi_monolithicfluidsplit.hpp"

#include "4C_adapter_ale_fsi.hpp"
#include "4C_adapter_fld_fluid_fsi.hpp"
#include "4C_adapter_str_fsiwrapper.hpp"
#include "4C_adapter_str_structure.hpp"
#include "4C_ale_utils_mapextractor.hpp"
#include "4C_constraint_manager.hpp"
#include "4C_coupling_adapter.hpp"
#include "4C_coupling_adapter_converter.hpp"
#include "4C_fluid_utils_mapextractor.hpp"
#include "4C_fsi_nox_group.hpp"
#include "4C_fsi_statustest.hpp"
#include "4C_global_data.hpp"
#include "4C_io.hpp"
#include "4C_io_control.hpp"
#include "4C_linalg_utils_sparse_algebra_create.hpp"
#include "4C_linalg_utils_sparse_algebra_manipulation.hpp"
#include "4C_linalg_utils_sparse_algebra_math.hpp"
#include "4C_linear_solver_method_linalg.hpp"
#include "4C_structure_aux.hpp"

#include <Teuchos_StandardParameterEntryValidators.hpp>
#include <Teuchos_TimeMonitor.hpp>

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
FSI::MonolithicFluidSplit::MonolithicFluidSplit(
    MPI_Comm comm, const Teuchos::ParameterList& timeparams)
    : BlockMonolithic(comm, timeparams), lambda_(nullptr), lambdaold_(nullptr), energysum_(0.0)
{
  // ---------------------------------------------------------------------------
  // FSI specific check of Dirichlet boundary conditions
  // ---------------------------------------------------------------------------
  // Create intersection of slave DOFs that hold a Dirichlet boundary condition
  // and are located at the FSI interface
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> intersectionmaps;
  intersectionmaps.push_back(fluid_field()->get_dbc_map_extractor()->cond_map());
  intersectionmaps.push_back(fluid_field()->interface()->fsi_cond_map());
  std::shared_ptr<Core::LinAlg::Map> intersectionmap =
      Core::LinAlg::MultiMapExtractor::intersect_maps(intersectionmaps);

  // Check whether the intersection is empty
  if (intersectionmap->num_global_elements() != 0)
  {
    //    std::cout << "Slave interface nodes with Dirichlet boundary condition (input file
    //    numbering):" << std::endl; for (int i=0; i <
    //    (int)fluid_field()->discretization()->NumMyRowNodes(); i++)
    //    {
    //      // get all nodes and add them
    //      int gid = fluid_field()->discretization()->NodeRowMap()->GID(i);
    //
    //      // do only nodes that I have in my discretization
    //      if (!fluid_field()->discretization()->NodeColMap()->MyGID(gid)) continue;
    //      Core::Nodes::Node* node = fluid_field()->discretization()->gNode(gid);
    //      if (!node) FOUR_C_THROW("Cannot find node with gid %",gid);
    //
    //      std::vector<int> nodedofs = fluid_field()->discretization()->Dof(node);
    //
    //      for (int j=0; j < (int)nodedofs.size(); j++)
    //      {
    //        for (int k=0; k < (int)intersectionmap->NumGlobalElements(); k++)
    //        {
    //          if (nodedofs[j] == intersectionmap->GID(k))
    //          {
    //            std::cout << gid+1 << std::endl;
    //            k = (int)intersectionmap->GID(k);
    //            j = (int)nodedofs.size();
    //          }
    //        }
    //      }
    //    }


    // It is not allowed, that slave DOFs at the interface hold a Dirichlet
    // boundary condition. Thus --> Error message

    // We do not have to care whether ALE interface DOFs carry DBCs in the
    // input file since they do not occur in the monolithic system and, hence,
    // do not cause a conflict.

    std::stringstream errormsg;
    errormsg << "  "
                "+---------------------------------------------------------------------------------"
                "------------+"
             << std::endl
             << "  |                DIRICHLET BOUNDARY CONDITIONS ON SLAVE SIDE OF FSI INTERFACE   "
                "              |"
             << std::endl
             << "  "
                "+---------------------------------------------------------------------------------"
                "------------+"
             << std::endl
             << "  | NOTE: The slave side of the interface is not allowed to carry Dirichlet "
                "boundary conditions.|"
             << std::endl
             << "  |                                                                               "
                "              |"
             << std::endl
             << "  | This is a fluid split scheme. Hence, master and slave field are chosen as "
                "follows:          |"
             << std::endl
             << "  |     MASTER  = STRUCTURE                                                       "
                "              |"
             << std::endl
             << "  |     SLAVE   = FLUID                                                           "
                "              |"
             << std::endl
             << "  |                                                                               "
                "              |"
             << std::endl
             << "  | Dirichlet boundary conditions were detected on slave interface degrees of "
                "freedom. Please   |"
             << std::endl
             << "  | remove Dirichlet boundary conditions from the slave side of the FSI "
                "interface.              |"
             << std::endl
             << "  | Only the master side of the FSI interface is allowed to carry Dirichlet "
                "boundary conditions.|"
             << std::endl
             << "  "
                "+---------------------------------------------------------------------------------"
                "------------+"
             << std::endl;

    FOUR_C_THROW("{}", errormsg.str());
  }
  // ---------------------------------------------------------------------------

  fggtransform_ = std::make_shared<Coupling::Adapter::MatrixRowColTransform>();
  fgitransform_ = std::make_shared<Coupling::Adapter::MatrixRowTransform>();
  figtransform_ = std::make_shared<Coupling::Adapter::MatrixColTransform>();
  aigtransform_ = std::make_shared<Coupling::Adapter::MatrixColTransform>();
  fmiitransform_ = std::make_shared<Coupling::Adapter::MatrixColTransform>();
  fmgitransform_ = std::make_shared<Coupling::Adapter::MatrixRowColTransform>();
  fmggtransform_ = std::make_shared<Coupling::Adapter::MatrixRowColTransform>();

  // Recovery of Lagrange multiplier happens on fluid field
  lambda_ = std::make_shared<Core::LinAlg::Vector<double>>(
      *fluid_field()->interface()->fsi_cond_map(), true);
  lambdaold_ = std::make_shared<Core::LinAlg::Vector<double>>(
      *fluid_field()->interface()->fsi_cond_map(), true);
  fmgiprev_ = nullptr;
  fmgicur_ = nullptr;
  fmggprev_ = nullptr;
  fmggcur_ = nullptr;
  fgiprev_ = nullptr;
  fgicur_ = nullptr;
  fggprev_ = nullptr;
  fggcur_ = nullptr;

#ifdef FOUR_C_ENABLE_ASSERTIONS
  // check whether allocation was successful
  if (fggtransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'fggtransform_' failed.");
  }
  if (fgitransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'fgitransform_' failed.");
  }
  if (figtransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'figtransform_' failed.");
  }
  if (aigtransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'aigtransform_' failed.");
  }
  if (fmiitransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'fmiitransform_' failed.");
  }
  if (fmgitransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'fmgitransform_' failed.");
  }
  if (fmggtransform_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'fmggtransform_' failed.");
  }
  if (lambda_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'lambda_' failed.");
  }
  if (lambdaold_ == nullptr)
  {
    FOUR_C_THROW("Allocation of 'lambdaold_' failed.");
  }
#endif

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_system()
{
  const Teuchos::ParameterList& fsidyn = Global::Problem::instance()->fsi_dynamic_params();
  const Teuchos::ParameterList& fsimono = fsidyn.sublist("MONOLITHIC SOLVER");
  linearsolverstrategy_ =
      Teuchos::getIntegralValue<Inpar::FSI::LinearBlockSolver>(fsimono, "LINEARBLOCKSOLVER");

  set_default_parameters(fsidyn, nox_parameter_list());

  // call SetupSystem in base class
  FSI::Monolithic::setup_system();

  // create combined map
  create_combined_dof_row_map();

  /*----------------------------------------------------------------------*/
  // Switch fluid to interface split block matrix
  fluid_field()->use_block_matrix(true);

  // build ale system matrix in split system
  ale_field()->create_system_matrix(ale_field()->interface());

  aleresidual_ =
      std::make_shared<Core::LinAlg::Vector<double>>(*ale_field()->interface()->other_map());

  // ---------------------------------------------------------------------------
  // Build the global Dirichlet map extractor
  setup_dbc_map_extractor();
  // ---------------------------------------------------------------------------

  // create the system matrix
  create_system_matrix();
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::create_system_matrix()
{
  FSI::BlockMonolithic::create_system_matrix(systemmatrix_, false);
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::create_combined_dof_row_map()
{
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> vecSpaces;
  vecSpaces.push_back(structure_field()->dof_row_map());
  vecSpaces.push_back(fluid_field()->dof_row_map());
  vecSpaces.push_back(ale_field()->interface()->other_map());

  if (vecSpaces[1]->num_global_elements() == 0)
    FOUR_C_THROW("No inner fluid equations. Splitting not possible. Panic.");

  set_dof_row_maps(vecSpaces);

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_dbc_map_extractor()
{
  // Dirichlet maps for structure and fluid do not intersect with interface map.
  // ALE Dirichlet map might intersect with interface map, but ALE interface DOFs
  // are not part of the final system of equations. Hence, we just need the
  // intersection of inner ALE DOFs with Dirichlet ALE DOFs.
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> aleintersectionmaps;
  aleintersectionmaps.push_back(ale_field()->get_dbc_map_extractor()->cond_map());
  aleintersectionmaps.push_back(ale_field()->interface()->other_map());
  std::shared_ptr<Core::LinAlg::Map> aleintersectionmap =
      Core::LinAlg::MultiMapExtractor::intersect_maps(aleintersectionmaps);

  // Merge Dirichlet maps of structure, fluid and ALE to global FSI Dirichlet map
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> dbcmaps;
  dbcmaps.push_back(structure_field()->get_dbc_map_extractor()->cond_map());
  dbcmaps.push_back(fluid_field()->get_dbc_map_extractor()->cond_map());
  dbcmaps.push_back(aleintersectionmap);
  std::shared_ptr<const Core::LinAlg::Map> dbcmap =
      Core::LinAlg::MultiMapExtractor::merge_maps(dbcmaps);

  // Finally, create the global FSI Dirichlet map extractor
  dbcmaps_ = std::make_shared<Core::LinAlg::MapExtractor>(*dof_row_map(), dbcmap, true);
  if (dbcmaps_ == nullptr)
  {
    FOUR_C_THROW("Creation of FSI Dirichlet map extractor failed.");
  }
  // ---------------------------------------------------------------------------

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> FSI::MonolithicFluidSplit::system_matrix()
    const
{
  return systemmatrix_;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_rhs_residual(Core::LinAlg::Vector<double>& f)
{
  // get time integration parameters of structure and fluid time integrators
  // to enable consistent time integration among the fields
  const double stiparam = structure_field()->tim_int_param();
  const double ftiparam = fluid_field()->tim_int_param();

  // some scaling factors for fluid
  const double fluidscale = fluid_field()->residual_scaling();

  // get single field residuals
  const Core::LinAlg::Vector<double> sv(*structure_field()->rhs());
  const Core::LinAlg::Vector<double> fv(*fluid_field()->rhs());
  const Core::LinAlg::Vector<double> av(*ale_field()->rhs());

  // extract only inner DOFs from fluid (=slave) and ALE field
  std::shared_ptr<Core::LinAlg::Vector<double>> fov =
      fluid_field()->interface()->extract_other_vector(fv);
  fov = fluid_field()->interface()->insert_other_vector(*fov);
  std::shared_ptr<const Core::LinAlg::Vector<double>> aov =
      ale_field()->interface()->extract_other_vector(av);

  // add fluid interface values to structure vector
  std::shared_ptr<Core::LinAlg::Vector<double>> fcv =
      fluid_field()->interface()->extract_fsi_cond_vector(fv);
  std::shared_ptr<Core::LinAlg::Vector<double>> modsv =
      structure_field()->interface()->insert_fsi_cond_vector(*fluid_to_struct(fcv));
  modsv->update(1.0, sv, (1.0 - stiparam) / (1.0 - ftiparam) * fluidscale);

  if (structure_field()->get_stc_algo() == Inpar::Solid::stc_currsym)
  {
    std::shared_ptr<Core::LinAlg::SparseMatrix> stcmat = structure_field()->get_stc_mat();
    stcmat->multiply(true, *modsv, *modsv);
  }

  // put the single field residuals together
  FSI::Monolithic::combine_field_vectors(f, *modsv, *fov, *aov);

  // add additional ale residual
  extractor().add_vector(*aleresidual_, 2, f);

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_rhs_lambda(Core::LinAlg::Vector<double>& f)
{
  if (lambdaold_ != nullptr)
  {
    // get time integration parameters of structure and fluid time integrators
    // to enable consistent time integration among the fields
    const double stiparam = structure_field()->tim_int_param();
    const double ftiparam = fluid_field()->tim_int_param();

    // project Lagrange multiplier field onto the master interface DOFs and consider temporal
    // scaling
    std::shared_ptr<Core::LinAlg::Vector<double>> lambdafull =
        structure_field()->interface()->insert_fsi_cond_vector(*fluid_to_struct(lambdaold_));
    lambdafull->scale(stiparam - (ftiparam * (1.0 - stiparam)) / (1.0 - ftiparam));

    // add Lagrange multiplier
    extractor().add_vector(*lambdafull, 0, f);
  }

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_rhs_firstiter(Core::LinAlg::Vector<double>& f)
{
  // get time integration parameters of structure and fluid time integrators
  // to enable consistent time integration among the fields
  const double stiparam = structure_field()->tim_int_param();
  const double ftiparam = fluid_field()->tim_int_param();

  // some scaling factors for fluid
  const double timescale = fluid_field()->time_scaling();
  const double scale = fluid_field()->residual_scaling();

  // old interface velocity of fluid field
  const std::shared_ptr<const Core::LinAlg::Vector<double>> fveln =
      fluid_field()->extract_interface_veln();

  // get fluid matrix
  const std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> blockf =
      fluid_field()->block_system_matrix();

  // get fluid shape derivatives matrix
  const std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> mmm =
      fluid_field()->shape_derivatives();

  // get ale matrix
  std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> blocka = ale_field()->block_system_matrix();

#ifdef FOUR_C_ENABLE_ASSERTIONS
  if (blockf == nullptr)
  {
    FOUR_C_THROW("Expected Teuchos::rcp to fluid block matrix.");
  }
  if (blocka == nullptr)
  {
    FOUR_C_THROW("Expected Teuchos::rcp to ale block matrix.");
  }
#endif

  // extract fluid and ale submatrices
  const Core::LinAlg::SparseMatrix& fig = blockf->matrix(0, 1);  // F_{I\Gamma}
  const Core::LinAlg::SparseMatrix& fgg = blockf->matrix(1, 1);  // F_{\Gamma\Gamma}
  const Core::LinAlg::SparseMatrix& aig = blocka->matrix(0, 1);  // A_{I\Gamma}

  // some often re-used vectors
  std::shared_ptr<Core::LinAlg::Vector<double>> rhs = nullptr;

  // Different contributions/terms to the rhs are separated by the following comment line
  // ---------- structural interface DOFs
  /* The following terms are added to the structural interface DOFs of right hand side:
   *
   * rhs_firstnewtonstep =
   *
   * (1)  + (1-stiparam)/(1-ftiparam) * dt / tau * F_{\Gamma\Gamma} * u^{n}_{\Gamma}
   *
   * (2)  - (1-stiparam)/(1-ftiparam) / tau * F_{\Gamma\Gamma} * \Delta d_{\Gamma,p}
   *
   * (3)  - (1-stiparam)/(1-ftiparam) * F^{G}_{\Gamma\Gamma} * \Delta d_{\Gamma,p}
   *
   * Remarks on all terms:
   * +  tau: time scaling factor for interface time integration (tau =
   * 1/fluid_field()->TimeScaling())
   *
   */
  // ----------addressing term 1
  rhs = std::make_shared<Core::LinAlg::Vector<double>>(fgg.range_map(), true);

  fgg.Apply(*fveln, *rhs);

  rhs->scale(scale * (1. - stiparam) / (1. - ftiparam) * dt() * timescale);

  rhs = fluid_to_struct(rhs);
  rhs = structure_field()->interface()->insert_fsi_cond_vector(*rhs);

  if (structure_field()->get_stc_algo() == Inpar::Solid::stc_currsym)
  {
    std::shared_ptr<Core::LinAlg::SparseMatrix> stcmat = structure_field()->get_stc_mat();
    stcmat->multiply(true, *rhs, *rhs);
  }

  extractor().add_vector(*rhs, 0, f);
  // ----------end of term 1

  // ----------addressing term 2:
  rhs = std::make_shared<Core::LinAlg::Vector<double>>(fgg.range_map(), true);

  fgg.Apply(*struct_to_fluid(ddgpred_), *rhs);

  rhs->scale(-scale * (1. - stiparam) / (1. - ftiparam) * timescale);
  rhs = structure_field()->interface()->insert_fsi_cond_vector(*fluid_to_struct(rhs));

  extractor().add_vector(*rhs, 0, f);
  // ----------end of term 2

  // ----------addressing term 3:
  if (mmm != nullptr)
  {
    // extract F^{G}_{\Gamma\Gamma}
    const Core::LinAlg::SparseMatrix& fmgg = mmm->matrix(1, 1);

    rhs = std::make_shared<Core::LinAlg::Vector<double>>(fmgg.range_map(), true);

    fmgg.Apply(*struct_to_fluid(ddgpred_), *rhs);

    rhs->scale(-(1. - stiparam) / (1. - ftiparam));
    rhs = structure_field()->interface()->insert_fsi_cond_vector(*fluid_to_struct(rhs));

    extractor().add_vector(*rhs, 0, f);
  }
  // ----------end of term 3
  // ----------end of structural interface DOFs

  // ---------- inner fluid DOFs
  /* The following terms are added to the inner fluid DOFs of right hand side:
   *
   * rhs_firstnewtonstep =
   *
   * (1)  + dt / tau * F_{I \Gamma} * u^{n}_{\Gamma}
   *
   * (2)  - 1 / tau F_{I \Gamma} * \Delta d_{\Gamma,p}
   *
   * (3)  - F^{G}_{I \Gamma} * \Delta d_{\Gamma,p}
   *
   * Remarks on all terms:
   * +  tau: time scaling factor for interface time integration (tau =
   * 1/fluid_field()->TimeScaling())
   *
   */
  // ----------addressing term 1
  rhs = std::make_shared<Core::LinAlg::Vector<double>>(fig.range_map(), true);

  fig.Apply(*fveln, *rhs);

  rhs->scale(dt() * timescale);

  rhs = fluid_field()->interface()->insert_other_vector(*rhs);

  extractor().add_vector(*rhs, 1, f);
  // ----------end of term 1

  // ----------addressing term 2
  rhs = std::make_shared<Core::LinAlg::Vector<double>>(fig.range_map(), true);

  fig.Apply(*struct_to_fluid(ddgpred_), *rhs);

  rhs->scale(-timescale);

  rhs = fluid_field()->interface()->insert_other_vector(*rhs);

  extractor().add_vector(*rhs, 1, f);
  // ----------end of term 2

  // ----------addressing term 3
  if (mmm != nullptr)
  {
    // extract F^{G}_{I \Gamma}
    const Core::LinAlg::SparseMatrix& fmig = mmm->matrix(0, 1);

    rhs = std::make_shared<Core::LinAlg::Vector<double>>(fmig.range_map(), true);

    fmig.Apply(*struct_to_fluid(ddgpred_), *rhs);

    rhs->scale(-1.);

    rhs = fluid_field()->interface()->insert_other_vector(*rhs);

    extractor().add_vector(*rhs, 1, f);
  }
  // ----------end of term 3
  // ----------end of inner fluid DOFs

  // ---------- inner ale DOFs
  /* The following terms are added to the inner ale DOFs of right hand side:
   *
   * rhs_firstnewtonstep =
   *
   * (1)  - A_{I \Gamma} * \Delta d_{\Gamma,p}
   *
   */
  // ----------addressing term 1
  rhs = std::make_shared<Core::LinAlg::Vector<double>>(aig.range_map(), true);

  aig.Apply(*struct_to_ale(ddgpred_), *rhs);
  rhs->scale(-1.0);

  extractor().add_vector(*rhs, 2, f);
  // ----------end of term 1
  // ---------- end of inner ale DOFs

  // Reset quantities of previous iteration step since they still store values from the last time
  // step
  ddginc_ = Core::LinAlg::create_vector(*structure_field()->interface()->fsi_cond_map(), true);
  duiinc_ = Core::LinAlg::create_vector(*fluid_field()->interface()->other_map(), true);
  ddialeinc_ = Core::LinAlg::create_vector(*ale_field()->interface()->other_map(), true);
  soliprev_ = nullptr;
  solgprev_ = nullptr;
  fgicur_ = nullptr;
  fggcur_ = nullptr;

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::setup_system_matrix(Core::LinAlg::BlockSparseMatrixBase& mat)
{
  TEUCHOS_FUNC_TIME_MONITOR("FSI::MonolithicFluidSplit::setup_system_matrix");

  const Coupling::Adapter::Coupling& coupsf = structure_fluid_coupling();
  const Coupling::Adapter::Coupling& coupsa = structure_ale_coupling();
  const Coupling::Adapter::Coupling& coupfa = fluid_ale_coupling();

  // get info about STC feature
  Inpar::Solid::StcScale stcalgo = structure_field()->get_stc_algo();
  std::shared_ptr<Core::LinAlg::SparseMatrix> stcmat = nullptr;
  // if STC is to be used, get STC matrix from structure field
  if (stcalgo != Inpar::Solid::stc_inactive) stcmat = structure_field()->get_stc_mat();

  // get single field block matrices
  std::shared_ptr<Core::LinAlg::SparseMatrix> s =
      structure_field()->system_matrix();  // can't be 'const' --> is modified by STC
  const std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> f =
      fluid_field()->block_system_matrix();
  const std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> a = ale_field()->block_system_matrix();

#ifdef FOUR_C_ENABLE_ASSERTIONS
  // check whether allocation was successful
  if (s == nullptr)
  {
    FOUR_C_THROW("expect structure block matrix");
  }
  if (f == nullptr)
  {
    FOUR_C_THROW("expect fluid block matrix");
  }
  if (a == nullptr)
  {
    FOUR_C_THROW("expect ale block matrix");
  }
#endif

  // extract submatrices
  Core::LinAlg::SparseMatrix& fii = f->matrix(0, 0);
  Core::LinAlg::SparseMatrix& fig = f->matrix(0, 1);
  Core::LinAlg::SparseMatrix& fgi = f->matrix(1, 0);
  Core::LinAlg::SparseMatrix& fgg = f->matrix(1, 1);
  Core::LinAlg::SparseMatrix& aii = a->matrix(0, 0);
  Core::LinAlg::SparseMatrix& aig = a->matrix(0, 1);

  // scaling factors for fluid
  const double scale = fluid_field()->residual_scaling();
  const double timescale = fluid_field()->time_scaling();

  // get time integration parameters of structure and fluid time integrators
  // to enable consistent time integration among the fields
  const double stiparam = structure_field()->tim_int_param();
  const double ftiparam = fluid_field()->tim_int_param();

  /*----------------------------------------------------------------------*/

  // build block matrix
  // The maps of the block matrix have to match the maps of the blocks we
  // insert here.

  // uncomplete because the fluid interface can have more connections than the
  // structural one. (Tet elements in fluid can cause this.) We should do
  // this just once...
  s->un_complete();

  (*fggtransform_)(fgg, (1.0 - stiparam) / (1.0 - ftiparam) * scale * timescale,
      Coupling::Adapter::CouplingSlaveConverter(coupsf),
      Coupling::Adapter::CouplingSlaveConverter(coupsf), *s, true, true);

  std::shared_ptr<Core::LinAlg::SparseMatrix> lfgi =
      std::make_shared<Core::LinAlg::SparseMatrix>(s->row_map(), 81, false);
  (*fgitransform_)(fgi, (1.0 - stiparam) / (1.0 - ftiparam) * scale,
      Coupling::Adapter::CouplingSlaveConverter(coupsf), *lfgi);

  lfgi->complete(fgi.domain_map(), s->range_map());

  if (stcalgo == Inpar::Solid::stc_currsym)
    lfgi = Core::LinAlg::matrix_multiply(*stcmat, true, *lfgi, false, true, true, true);

  mat.matrix(0, 1).un_complete();
  mat.matrix(0, 1).add(*lfgi, false, 1., 0.0);

  if (stcalgo == Inpar::Solid::stc_inactive)
  {
    Core::LinAlg::SparseMatrix lfig(fig.row_map(), 81, false);
    (*figtransform_)(f->full_row_map(), f->full_col_map(), fig, timescale,
        Coupling::Adapter::CouplingSlaveConverter(coupsf), mat.matrix(1, 0));
  }
  else
  {
    std::shared_ptr<Core::LinAlg::SparseMatrix> lfig =
        std::make_shared<Core::LinAlg::SparseMatrix>(fig.row_map(), 81, false);
    (*figtransform_)(f->full_row_map(), f->full_col_map(), fig, timescale,
        Coupling::Adapter::CouplingSlaveConverter(coupsf), *lfig);

    lfig->complete(s->domain_map(), fig.range_map());

    lfig = Core::LinAlg::matrix_multiply(*lfig, false, *stcmat, false, false, false, true);

    mat.matrix(1, 0).un_complete();
    mat.matrix(1, 0).add(*lfig, false, 1., 0.0);
  }

  mat.matrix(1, 1).add(fii, false, 1., 0.0);
  std::shared_ptr<Core::LinAlg::SparseMatrix> eye =
      Core::LinAlg::create_identity_matrix(*fluid_field()->interface()->fsi_cond_map());
  mat.matrix(1, 1).add(*eye, false, 1., 1.0);

  if (stcalgo == Inpar::Solid::stc_inactive)
  {
    (*aigtransform_)(a->full_row_map(), a->full_col_map(), aig, 1.,
        Coupling::Adapter::CouplingSlaveConverter(coupsa), mat.matrix(2, 0));
  }
  else
  {
    std::shared_ptr<Core::LinAlg::SparseMatrix> laig =
        std::make_shared<Core::LinAlg::SparseMatrix>(aii.row_map(), 81, false);
    (*aigtransform_)(a->full_row_map(), a->full_col_map(), aig, 1.,
        Coupling::Adapter::CouplingSlaveConverter(coupsa), *laig);

    laig->complete(s->domain_map(), laig->range_map());

    if (stcalgo != Inpar::Solid::stc_inactive)
    {
      laig = Core::LinAlg::matrix_multiply(*laig, false, *stcmat, false, false, false, true);
    }

    mat.assign(2, 0, Core::LinAlg::DataAccess::View, *laig);
  }

  mat.assign(2, 2, Core::LinAlg::DataAccess::View, aii);

  /*----------------------------------------------------------------------*/
  // add optional fluid linearization with respect to mesh motion block

  std::shared_ptr<Core::LinAlg::BlockSparseMatrixBase> mmm = fluid_field()->shape_derivatives();
  if (mmm != nullptr)
  {
    Core::LinAlg::SparseMatrix& fmii = mmm->matrix(0, 0);
    Core::LinAlg::SparseMatrix& fmgi = mmm->matrix(1, 0);

    Core::LinAlg::SparseMatrix& fmig = mmm->matrix(0, 1);
    Core::LinAlg::SparseMatrix& fmgg = mmm->matrix(1, 1);

    // reuse transform objects to add shape derivative matrices to structural blocks

    if (stcalgo == Inpar::Solid::stc_inactive)
    {
      (*figtransform_)(f->full_row_map(), f->full_col_map(), fmig, 1.,
          Coupling::Adapter::CouplingSlaveConverter(coupsf), mat.matrix(1, 0), false, true);
    }
    else
    {
      std::shared_ptr<Core::LinAlg::SparseMatrix> lfmig =
          std::make_shared<Core::LinAlg::SparseMatrix>(fmig.row_map(), 81, false);
      (*figtransform_)(f->full_row_map(), f->full_col_map(), fmig, 1.,
          Coupling::Adapter::CouplingSlaveConverter(coupsf), *lfmig, false, true);


      lfmig->complete(s->domain_map(), fmig.range_map());

      if (stcalgo != Inpar::Solid::stc_inactive)
      {
        lfmig = Core::LinAlg::matrix_multiply(*lfmig, false, *stcmat, false, false, false, true);
      }

      mat.matrix(1, 0).add(*lfmig, false, 1.0, 1.0);
    }

    (*fmggtransform_)(fmgg, (1.0 - stiparam) / (1.0 - ftiparam) * scale,
        Coupling::Adapter::CouplingSlaveConverter(coupsf),
        Coupling::Adapter::CouplingSlaveConverter(coupsf), *s, false, true);

    // We cannot copy the pressure value. It is not used anyway. So no exact
    // match here.
    (*fmiitransform_)(mmm->full_row_map(), mmm->full_col_map(), fmii, 1.,
        Coupling::Adapter::CouplingMasterConverter(coupfa), mat.matrix(1, 2), false);

    {
      std::shared_ptr<Core::LinAlg::SparseMatrix> lfmgi =
          std::make_shared<Core::LinAlg::SparseMatrix>(s->row_map(), 81, false);
      (*fmgitransform_)(fmgi, (1.0 - stiparam) / (1.0 - ftiparam) * scale,
          Coupling::Adapter::CouplingSlaveConverter(coupsf),
          Coupling::Adapter::CouplingMasterConverter(coupfa), *lfmgi, false, false);

      lfmgi->complete(aii.domain_map(), s->range_map());

      if (stcalgo == Inpar::Solid::stc_currsym)
        lfmgi = Core::LinAlg::matrix_multiply(*stcmat, true, *lfmgi, false, true, true, true);

      mat.matrix(0, 2).un_complete();
      mat.matrix(0, 2).add(*lfmgi, false, 1., 0.0);
    }
  }

  s->complete();

  if (stcalgo == Inpar::Solid::stc_inactive)
  {
    s->un_complete();
  }
  else  // apply STC matrix on block (0,0) if STC is used
  {
    s = Core::LinAlg::matrix_multiply(*s, false, *stcmat, false, true, true, true);
    if (stcalgo == Inpar::Solid::stc_currsym)
      s = Core::LinAlg::matrix_multiply(*stcmat, true, *s, false, true, true, false);
  }

  // finally assign structure block
  mat.matrix(0, 0).assign(Core::LinAlg::DataAccess::View, *s);

  // done. make sure all blocks are filled.
  mat.complete();

  // Finally, take care of Dirichlet boundary conditions
  mat.apply_dirichlet(*(dbcmaps_->cond_map()), true);

  // store parts of fluid matrix to know them in the next iteration as previous iteration matrices
  fgiprev_ = fgicur_;
  fggprev_ = fggcur_;
  fgicur_ = std::make_shared<Core::LinAlg::SparseMatrix>(f->matrix(1, 0));
  fggcur_ = std::make_shared<Core::LinAlg::SparseMatrix>(f->matrix(1, 1));

  // store parts of fluid shape derivative matrix to know them in the next iteration as previous
  // iteration matrices
  fmgiprev_ = fmgicur_;
  fmggprev_ = fmggcur_;
  if (mmm != nullptr)
  {
    fmgicur_ = std::make_shared<Core::LinAlg::SparseMatrix>(mmm->matrix(1, 0));
    fmggcur_ = std::make_shared<Core::LinAlg::SparseMatrix>(mmm->matrix(1, 1));
  }
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::scale_system(
    Core::LinAlg::BlockSparseMatrixBase& mat, Core::LinAlg::Vector<double>& b)
{
  const Teuchos::ParameterList& fsidyn = Global::Problem::instance()->fsi_dynamic_params();
  const Teuchos::ParameterList& fsimono = fsidyn.sublist("MONOLITHIC SOLVER");
  const bool scaling_infnorm = fsimono.get<bool>("INFNORMSCALING");

  if (scaling_infnorm)
  {
    // TODO: The matrices are modified here. Do we have to change them back later on?

    // do scaling of structure rows
    Core::LinAlg::SparseMatrix& A_00 = mat.matrix(0, 0);
    srowsum_ = std::make_shared<Core::LinAlg::Vector<double>>(A_00.row_map(), false);
    scolsum_ = std::make_shared<Core::LinAlg::Vector<double>>(A_00.row_map(), false);
    A_00.inv_row_sums(*srowsum_);
    A_00.inv_col_sums(*scolsum_);
    if (A_00.left_scale(*srowsum_) or A_00.right_scale(*scolsum_) or
        mat.matrix(0, 1).left_scale(*srowsum_) or mat.matrix(0, 2).left_scale(*srowsum_) or
        mat.matrix(1, 0).right_scale(*scolsum_) or mat.matrix(2, 0).right_scale(*scolsum_))
      FOUR_C_THROW("structure scaling failed");

    // do scaling of ale rows
    Core::LinAlg::SparseMatrix& A_22 = mat.matrix(2, 2);
    arowsum_ = std::make_shared<Core::LinAlg::Vector<double>>(A_22.row_map(), false);
    acolsum_ = std::make_shared<Core::LinAlg::Vector<double>>(A_22.row_map(), false);
    A_22.inv_row_sums(*arowsum_);
    A_22.inv_col_sums(*acolsum_);
    if (A_22.left_scale(*arowsum_) or A_22.right_scale(*acolsum_) or
        mat.matrix(2, 0).left_scale(*arowsum_) or mat.matrix(2, 1).left_scale(*arowsum_) or
        mat.matrix(0, 2).right_scale(*acolsum_) or mat.matrix(1, 2).right_scale(*acolsum_))
      FOUR_C_THROW("ale scaling failed");

    // do scaling of structure and ale rhs vectors
    std::shared_ptr<Core::LinAlg::Vector<double>> sx = extractor().extract_vector(b, 0);
    std::shared_ptr<Core::LinAlg::Vector<double>> ax = extractor().extract_vector(b, 2);

    if (sx->multiply(1.0, *srowsum_, *sx, 0.0)) FOUR_C_THROW("structure scaling failed");
    if (ax->multiply(1.0, *arowsum_, *ax, 0.0)) FOUR_C_THROW("ale scaling failed");

    extractor().insert_vector(*sx, 0, b);
    extractor().insert_vector(*ax, 2, b);
  }
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::unscale_solution(Core::LinAlg::BlockSparseMatrixBase& mat,
    Core::LinAlg::Vector<double>& x, Core::LinAlg::Vector<double>& b)
{
  const Teuchos::ParameterList& fsidyn = Global::Problem::instance()->fsi_dynamic_params();
  const Teuchos::ParameterList& fsimono = fsidyn.sublist("MONOLITHIC SOLVER");
  const bool scaling_infnorm = fsimono.get<bool>("INFNORMSCALING");

  if (scaling_infnorm)
  {
    std::shared_ptr<Core::LinAlg::Vector<double>> sy = extractor().extract_vector(x, 0);
    std::shared_ptr<Core::LinAlg::Vector<double>> ay = extractor().extract_vector(x, 2);

    if (sy->multiply(1.0, *scolsum_, *sy, 0.0)) FOUR_C_THROW("structure scaling failed");
    if (ay->multiply(1.0, *acolsum_, *ay, 0.0)) FOUR_C_THROW("ale scaling failed");

    // get info about STC feature and unscale solution if necessary
    Inpar::Solid::StcScale stcalgo = structure_field()->get_stc_algo();
    if (stcalgo != Inpar::Solid::stc_inactive)
    {
      structure_field()->get_stc_mat()->multiply(false, *sy, *sy);
    }

    extractor().insert_vector(*sy, 0, x);
    extractor().insert_vector(*ay, 2, x);

    std::shared_ptr<Core::LinAlg::Vector<double>> sx = extractor().extract_vector(b, 0);
    std::shared_ptr<Core::LinAlg::Vector<double>> ax = extractor().extract_vector(b, 2);

    if (sx->reciprocal_multiply(1.0, *srowsum_, *sx, 0.0)) FOUR_C_THROW("structure scaling failed");
    if (ax->reciprocal_multiply(1.0, *arowsum_, *ax, 0.0)) FOUR_C_THROW("ale scaling failed");

    if (stcalgo != Inpar::Solid::stc_inactive)
    {
      structure_field()->get_stc_mat()->multiply(false, *sx, *sx);
    }

    extractor().insert_vector(*sx, 0, b);
    extractor().insert_vector(*ax, 2, b);

    Core::LinAlg::SparseMatrix& A_00 = mat.matrix(0, 0);
    srowsum_->reciprocal(*srowsum_);
    scolsum_->reciprocal(*scolsum_);
    if (A_00.left_scale(*srowsum_) or A_00.right_scale(*scolsum_) or
        mat.matrix(0, 1).left_scale(*srowsum_) or mat.matrix(0, 2).left_scale(*srowsum_) or
        mat.matrix(1, 0).right_scale(*scolsum_) or mat.matrix(2, 0).right_scale(*scolsum_))
      FOUR_C_THROW("structure scaling failed");

    Core::LinAlg::SparseMatrix& A_22 = mat.matrix(2, 2);
    arowsum_->reciprocal(*arowsum_);
    acolsum_->reciprocal(*acolsum_);
    if (A_22.left_scale(*arowsum_) or A_22.right_scale(*acolsum_) or
        mat.matrix(2, 0).left_scale(*arowsum_) or mat.matrix(2, 1).left_scale(*arowsum_) or
        mat.matrix(0, 2).right_scale(*acolsum_) or mat.matrix(1, 2).right_scale(*acolsum_))
      FOUR_C_THROW("ale scaling failed");
  }

  // very simple hack just to see the linear solution

  Core::LinAlg::Vector<double> r(b.get_map());
  mat.Apply(x, r);
  r.update(1., b, 1.);

  std::shared_ptr<Core::LinAlg::Vector<double>> sr = extractor().extract_vector(r, 0);
  std::shared_ptr<Core::LinAlg::Vector<double>> fr = extractor().extract_vector(r, 1);
  std::shared_ptr<Core::LinAlg::Vector<double>> ar = extractor().extract_vector(r, 2);

  // increment additional ale residual
  aleresidual_->update(-1., *ar, 0.);

  std::ios_base::fmtflags flags = utils()->out().flags();

  double n, ns, nf, na;
  r.norm_2(&n);
  sr->norm_2(&ns);
  fr->norm_2(&nf);
  ar->norm_2(&na);
  if (verbosity_ == Inpar::FSI::verbosity_full)
  {
    utils()->out() << std::scientific << "\nlinear solver quality:\n"
                   << "L_2-norms:\n"
                   << "   |r|=" << n << "   |rs|=" << ns << "   |rf|=" << nf << "   |ra|=" << na
                   << "\n";
  }
  r.norm_inf(&n);
  sr->norm_inf(&ns);
  fr->norm_inf(&nf);
  ar->norm_inf(&na);
  if (verbosity_ == Inpar::FSI::verbosity_full)
  {
    utils()->out() << "L_inf-norms:\n"
                   << "   |r|=" << n << "   |rs|=" << ns << "   |rf|=" << nf << "   |ra|=" << na
                   << "\n";
  }
  utils()->out().flags(flags);

  if (structure_field()->get_stc_algo() != Inpar::Solid::stc_inactive)
    structure_field()->system_matrix()->reset();
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
Teuchos::RCP<::NOX::StatusTest::Combo> FSI::MonolithicFluidSplit::create_status_test(
    Teuchos::ParameterList& nlParams, Teuchos::RCP<::NOX::Abstract::Group> grp)
{
  // --------------------------------------------------------------------
  // Setup the test framework
  // --------------------------------------------------------------------
  // Create the top-level test combo
  Teuchos::RCP<::NOX::StatusTest::Combo> combo =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::OR);

  // Create test combo for convergence of residuals and iterative increments
  Teuchos::RCP<::NOX::StatusTest::Combo> converged =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::AND);

  // Create some other plausibility tests
  Teuchos::RCP<::NOX::StatusTest::MaxIters> maxiters =
      Teuchos::make_rcp<::NOX::StatusTest::MaxIters>(nlParams.get<int>("Max Iterations"));
  Teuchos::RCP<::NOX::StatusTest::FiniteValue> fv =
      Teuchos::make_rcp<::NOX::StatusTest::FiniteValue>();

  // Add single tests to the top-level test combo
  combo->addStatusTest(fv);
  combo->addStatusTest(converged);
  combo->addStatusTest(maxiters);

  // Start filling the 'converged' combo here
  // require one solve
  converged->addStatusTest(Teuchos::make_rcp<NOX::FSI::MinIters>(1));


  // --------------------------------------------------------------------
  // setup tests for structural displacement field
  // --------------------------------------------------------------------
  // create ::NOX::StatusTest::Combo for structural displacement field
  Teuchos::RCP<::NOX::StatusTest::Combo> structcombo =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::AND);

  // create Norm-objects for each norm that has to be tested
  Teuchos::RCP<NOX::FSI::PartialNormF> structureDisp_L2 = Teuchos::make_rcp<NOX::FSI::PartialNormF>(
      "DISPL residual", extractor(), 0, nlParams.get<double>("Tol dis res L2"),
      ::NOX::Abstract::Vector::TwoNorm, NOX::FSI::PartialNormF::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormF> structureDisp_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormF>("DISPL residual", extractor(), 0,
          nlParams.get<double>("Tol dis res Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormF::Unscaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> structureDispUpdate_L2 =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("DISPL update", extractor(), 0,
          nlParams.get<double>("Tol dis inc L2"), ::NOX::Abstract::Vector::TwoNorm,
          NOX::FSI::PartialNormUpdate::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> structureDispUpdate_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("DISPL update", extractor(), 0,
          nlParams.get<double>("Tol dis inc Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormUpdate::Unscaled);

  // tests needed to adapt relative tolerance of the linear solver
  add_status_test(structureDisp_L2);

  // add norm-tests to structural displacement ::NOX::StatusTest::Combo
  structcombo->addStatusTest(structureDisp_L2);
  structcombo->addStatusTest(structureDisp_inf);
  structcombo->addStatusTest(structureDispUpdate_L2);
  structcombo->addStatusTest(structureDispUpdate_inf);

  // add structural displacement test combo to top-level test combo
  converged->addStatusTest(structcombo);
  // ---------- end of structural displacement field tests

  // --------------------------------------------------------------------
  // setup tests for interface
  // --------------------------------------------------------------------
  // build mapextractor
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> interface;
  interface.push_back(structure_field()->interface()->fsi_cond_map());
  interface.push_back(nullptr);
  Core::LinAlg::MultiMapExtractor interfaceextract(*dof_row_map(), interface);

  // create ::NOX::StatusTest::Combo for interface
  Teuchos::RCP<::NOX::StatusTest::Combo> interfacecombo =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::AND);

  // create Norm-objects for each norm that has to be tested
  Teuchos::RCP<NOX::FSI::PartialNormF> interfaceTest_L2 = Teuchos::make_rcp<NOX::FSI::PartialNormF>(
      "GAMMA residual", interfaceextract, 0, nlParams.get<double>("Tol fsi res L2"),
      ::NOX::Abstract::Vector::TwoNorm, NOX::FSI::PartialNormF::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormF> interfaceTest_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormF>("GAMMA residual", interfaceextract, 0,
          nlParams.get<double>("Tol fsi res Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormF::Unscaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> interfaceTestUpdate_L2 =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("GAMMA update", interfaceextract, 0,
          nlParams.get<double>("Tol fsi inc L2"), ::NOX::Abstract::Vector::TwoNorm,
          NOX::FSI::PartialNormUpdate::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> interfaceTestUpdate_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("GAMMA update", interfaceextract, 0,
          nlParams.get<double>("Tol fsi inc Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormUpdate::Unscaled);

  // tests needed to adapt relative tolerance of the linear solver
  add_status_test(interfaceTest_L2);

  // add norm-tests to interface ::NOX::StatusTest::Combo
  interfacecombo->addStatusTest(interfaceTest_L2);
  interfacecombo->addStatusTest(interfaceTest_inf);
  interfacecombo->addStatusTest(interfaceTestUpdate_L2);
  interfacecombo->addStatusTest(interfaceTestUpdate_inf);

  // add interface test combo to top-level test combo
  converged->addStatusTest(interfacecombo);
  // ---------- end of interface tests

  // --------------------------------------------------------------------
  // setup tests for fluid velocity field
  // --------------------------------------------------------------------
  // build mapextractor
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> fluidvel;
  fluidvel.push_back(fluid_field()->inner_velocity_row_map());
  fluidvel.push_back(nullptr);
  Core::LinAlg::MultiMapExtractor fluidvelextract(*dof_row_map(), fluidvel);

  // create ::NOX::StatusTest::Combo for fluid velocity field
  Teuchos::RCP<::NOX::StatusTest::Combo> fluidvelcombo =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::AND);

  // create Norm-objects for each norm that has to be tested
  Teuchos::RCP<NOX::FSI::PartialNormF> innerFluidVel_L2 = Teuchos::make_rcp<NOX::FSI::PartialNormF>(
      "VELOC residual", fluidvelextract, 0, nlParams.get<double>("Tol vel res L2"),
      ::NOX::Abstract::Vector::TwoNorm, NOX::FSI::PartialNormF::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormF> innerFluidVel_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormF>("VELOC residual", fluidvelextract, 0,
          nlParams.get<double>("Tol vel res Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormF::Unscaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> innerFluidVelUpdate_L2 =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("VELOC update", fluidvelextract, 0,
          nlParams.get<double>("Tol vel inc L2"), ::NOX::Abstract::Vector::TwoNorm,
          NOX::FSI::PartialNormUpdate::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> innerFluidVelUpdate_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("VELOC update", fluidvelextract, 0,
          nlParams.get<double>("Tol vel inc Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormUpdate::Unscaled);

  // tests needed to adapt relative tolerance of the linear solver
  add_status_test(innerFluidVel_L2);

  // add norm-tests to fluid velocity ::NOX::StatusTest::Combo
  fluidvelcombo->addStatusTest(innerFluidVel_L2);
  fluidvelcombo->addStatusTest(innerFluidVel_inf);
  fluidvelcombo->addStatusTest(innerFluidVelUpdate_L2);
  fluidvelcombo->addStatusTest(innerFluidVelUpdate_inf);

  // add fluid velocity test combo to top-level test combo
  converged->addStatusTest(fluidvelcombo);
  // ---------- end of fluid velocity field tests

  // --------------------------------------------------------------------
  // setup tests for fluid pressure field
  // --------------------------------------------------------------------
  // build mapextractor
  std::vector<std::shared_ptr<const Core::LinAlg::Map>> fluidpress;
  fluidpress.push_back(fluid_field()->pressure_row_map());
  fluidpress.push_back(nullptr);
  Core::LinAlg::MultiMapExtractor fluidpressextract(*dof_row_map(), fluidpress);

  // create ::NOX::StatusTest::Combo for fluid pressure field
  Teuchos::RCP<::NOX::StatusTest::Combo> fluidpresscombo =
      Teuchos::make_rcp<::NOX::StatusTest::Combo>(::NOX::StatusTest::Combo::AND);

  // create Norm-objects for each norm that has to be tested
  Teuchos::RCP<NOX::FSI::PartialNormF> fluidPress_L2 = Teuchos::make_rcp<NOX::FSI::PartialNormF>(
      "PRESS residual", fluidpressextract, 0, nlParams.get<double>("Tol pre res L2"),
      ::NOX::Abstract::Vector::TwoNorm, NOX::FSI::PartialNormF::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormF> fluidPress_inf = Teuchos::make_rcp<NOX::FSI::PartialNormF>(
      "PRESS residual", fluidpressextract, 0, nlParams.get<double>("Tol pre res Inf"),
      ::NOX::Abstract::Vector::MaxNorm, NOX::FSI::PartialNormF::Unscaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> fluidPressUpdate_L2 =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("PRESS update", fluidpressextract, 0,
          nlParams.get<double>("Tol pre inc L2"), ::NOX::Abstract::Vector::TwoNorm,
          NOX::FSI::PartialNormUpdate::Scaled);
  Teuchos::RCP<NOX::FSI::PartialNormUpdate> fluidPressUpdate_inf =
      Teuchos::make_rcp<NOX::FSI::PartialNormUpdate>("PRESS update", fluidpressextract, 0,
          nlParams.get<double>("Tol pre inc Inf"), ::NOX::Abstract::Vector::MaxNorm,
          NOX::FSI::PartialNormUpdate::Unscaled);

  // tests needed to adapt relative tolerance of the linear solver
  add_status_test(fluidPress_L2);

  // add norm-tests to fluid pressure ::NOX::StatusTest::Combo
  fluidpresscombo->addStatusTest(fluidPress_L2);
  fluidpresscombo->addStatusTest(fluidPress_inf);
  fluidpresscombo->addStatusTest(fluidPressUpdate_L2);
  fluidpresscombo->addStatusTest(fluidPressUpdate_inf);

  // add fluid pressure test combo to top-level test combo
  converged->addStatusTest(fluidpresscombo);
  // ---------- end of fluid pressure field tests

  // Finally, return the test combo
  return combo;
}


/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::extract_field_vectors(
    std::shared_ptr<const Core::LinAlg::Vector<double>> x,
    std::shared_ptr<const Core::LinAlg::Vector<double>>& sx,
    std::shared_ptr<const Core::LinAlg::Vector<double>>& fx,
    std::shared_ptr<const Core::LinAlg::Vector<double>>& ax)
{
  TEUCHOS_FUNC_TIME_MONITOR("FSI::MonolithicFluidSplit::extract_field_vectors");

#ifdef FOUR_C_ENABLE_ASSERTIONS
  if (ddgpred_ == nullptr)
  {
    FOUR_C_THROW("Vector 'ddgpred_' has not been initialized properly.");
  }
#endif

  // ---------------------------------------------------------------------------
  // process structure unknowns
  // ---------------------------------------------------------------------------
  // extract structure solution increment from NOX increment
  sx = extractor().extract_vector(*x, 0);

  // ---------------------------------------------------------------------------
  // process ale unknowns
  // ---------------------------------------------------------------------------
  // extract inner ALE solution increment from NOX increment
  std::shared_ptr<const Core::LinAlg::Vector<double>> aox = extractor().extract_vector(*x, 2);

  // convert structure solution increment to ALE solution increment at the interface
  std::shared_ptr<Core::LinAlg::Vector<double>> scx =
      structure_field()->interface()->extract_fsi_cond_vector(*sx);
  scx->update(1.0, *ddgpred_, 1.0);
  std::shared_ptr<const Core::LinAlg::Vector<double>> acx = struct_to_ale(scx);

  // put inner and interface ALE solution increments together
  std::shared_ptr<Core::LinAlg::Vector<double>> a =
      ale_field()->interface()->insert_other_vector(*aox);
  ale_field()->interface()->insert_fsi_cond_vector(*acx, *a);
  ax = a;

  // ---------------------------------------------------------------------------
  // process fluid unknowns
  // ---------------------------------------------------------------------------
  // extract inner fluid solution increment from NOX increment
  std::shared_ptr<const Core::LinAlg::Vector<double>> fox = extractor().extract_vector(*x, 1);
  fox = fluid_field()->interface()->extract_other_vector(*fox);

  // convert ALE solution increment to fluid solution increment at the interface
  std::shared_ptr<Core::LinAlg::Vector<double>> fcx = ale_to_fluid_interface(acx);
  fluid_field()->displacement_to_velocity(fcx);

  // put inner and interface fluid solution increments together
  std::shared_ptr<Core::LinAlg::Vector<double>> f =
      fluid_field()->interface()->insert_other_vector(*fox);
  fluid_field()->interface()->insert_fsi_cond_vector(*fcx, *f);
  fx = f;

  // ---------------------------------------------------------------------------

  // Store field vectors to know them later on as previous quantities
  // inner ale displacement increment
  // interface structure displacement increment
  if (disgprev_ != nullptr)
    ddginc_->update(1.0, *scx, -1.0, *disgprev_, 0.0);  // compute current iteration increment
  else
    ddginc_ = std::make_shared<Core::LinAlg::Vector<double>>(*scx);  // first iteration increment

  disgprev_ = scx;  // store current step increment
  // ------------------------------------

  // inner ale displacement increment
  if (solialeprev_ != nullptr)
    ddialeinc_->update(1.0, *aox, -1.0, *solialeprev_, 0.0);  // compute current iteration increment
  else
    ddialeinc_ = std::make_shared<Core::LinAlg::Vector<double>>(*aox);  // first iteration increment

  solialeprev_ = aox;  // store current step increment
  // ------------------------------------

  // fluid solution increment
  if (soliprev_ != nullptr)  // compute current iteration increment
    duiinc_->update(1.0, *fox, -1.0, *soliprev_, 0.0);
  else  // first iteration increment
    duiinc_ = std::make_shared<Core::LinAlg::Vector<double>>(*fox);
  // store current step increment
  soliprev_ = fox;
  // ------------------------------------
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::output()
{
  structure_field()->output();
  fluid_field()->output();

  // output Lagrange multiplier
  output_lambda();

  ale_field()->output();

  if (structure_field()->get_constraint_manager()->have_monitor())
  {
    structure_field()->get_constraint_manager()->compute_monitor_values(
        structure_field()->dispnp());
    if (Core::Communication::my_mpi_rank(get_comm()) == 0)
      structure_field()->get_constraint_manager()->print_monitor_values();
  }
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::output_lambda()
{
  /* 'lambda_' is only defined on the interface. So, insert 'lambda_' into
   * 'lambdafull' that is defined on the entire fluid field. Then, write
   * output or restart data.
   */
  std::shared_ptr<Core::LinAlg::Vector<double>> lambdafull =
      fluid_field()->interface()->insert_fsi_cond_vector(*lambda_);
  const int uprestart = timeparams_.get<int>("RESTARTEVERY");
  const int upres = timeparams_.get<int>("RESULTSEVERY");
  if ((uprestart != 0 && fluid_field()->step() % uprestart == 0) or
      (upres != 0 and fluid_field()->step() % upres == 0))
    fluid_field()->disc_writer()->write_vector("fsilambda", lambdafull);
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::update()
{
  lambdaold_->update(1.0, *lambda_, 0.0);

  FSI::BlockMonolithic::update();
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::read_restart(int step)
{
  structure_field()->read_restart(step);
  fluid_field()->read_restart(step);

  // read Lagrange multiplier
  {
    std::shared_ptr<Core::LinAlg::Vector<double>> lambdafull =
        std::make_shared<Core::LinAlg::Vector<double>>(*fluid_field()->dof_row_map(), true);
    Core::IO::DiscretizationReader reader = Core::IO::DiscretizationReader(
        fluid_field()->discretization(), Global::Problem::instance()->input_control_file(), step);
    reader.read_vector(lambdafull, "fsilambda");
    lambdaold_ = fluid_field()->interface()->extract_fsi_cond_vector(*lambdafull);
    // Note: the above is normally enough. However, we can use the restart in order to periodically
    // repeat the fsi simulation (see AC-FS3I)
    lambda_ = fluid_field()->interface()->extract_fsi_cond_vector(*lambdafull);
  }

  ale_field()->read_restart(step);

  set_time_step(fluid_field()->time(), fluid_field()->step());
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::prepare_time_step()
{
  increment_time_and_step();
  if (verbosity_ >= Inpar::FSI::verbosity_low) print_header();

  prepare_time_step_preconditioner();

  if (structure_field()->get_stc_algo() != Inpar::Solid::stc_inactive)
    structure_field()->system_matrix()->reset();

  prepare_time_step_fields();

  // Note: it's important to first prepare the single fields and than the fsi problem
  prepare_time_step_fsi();

  return;
}

/*----------------------------------------------------------------------*/
/* Recover the Lagrange multiplier at the interface   mayr.mt (03/2012) */
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::recover_lagrange_multiplier()
{
  // get time integration parameter of fluid time integrator
  // to enable consistent time integration among the fields
  const double ftiparam = fluid_field()->tim_int_param();

  // some scaling factors for fluid
  const double timescale = fluid_field()->time_scaling();
  const double scale = fluid_field()->residual_scaling();

  // some often re-used vectors
  std::shared_ptr<Core::LinAlg::Vector<double>> tmpvec =
      nullptr;  // stores intermediate result of terms (3)-(8)
  std::shared_ptr<Core::LinAlg::Vector<double>> auxvec = nullptr;     // just for convenience
  std::shared_ptr<Core::LinAlg::Vector<double>> auxauxvec = nullptr;  // just for convenience

  /* Recovery of Lagrange multiplier \lambda_^{n+1} is done by the following
   * condensation expression:
   *
   * lambda_^{n+1} =
   *
   * (1)  - ftiparam / (1.-ftiparam) * lambda^{n}
   *
   * (2)  - 1. / (1.-ftiparam) * tmpvec
   *
   * with tmpvec =
   *
   * (3)    r_{\Gamma}^{F,n+1}
   *
   * (4)  + 1 / tau * F_{\Gamma\Gamma} * \Delta d_{\Gamma}^{S,n+1}
   *
   * (5)  + F_{\Gamma\Gamma}^{G} * \Delta d_{\Gamma}^{S,n+1}
   *
   * (6)  + F_{\Gamma I} * \Delta u_{I}^{F,n+1}
   *
   * (7)  + F_{\Gamma I}^{G} * \Delta d_{I}^{G,n+1}
   *
   * (8)  + dt / tau * F_{\Gamma\Gamma} * u_{\Gamma}^n]
   *
   * Remark on term (8):
   * Term (8) has to be considered only in the first Newton iteration.
   * Hence, it will usually not be computed since in general we need more
   * than one nonlinear iteration until convergence.
   *
   * Remarks on all terms:
   * +  Division by -(1.0 - ftiparam) will be done in the end
   *    since this is common to all terms
   * +  tau: time scaling factor for interface time integration (tau =
   * 1/fluid_field()->TimeScaling())
   * +  neglecting terms (4)-(8) should not alter the results significantly
   *    since at the end of the time step the solution increments tend to zero.
   *
   *                                                 Matthias Mayr (10/2012)
   */

  // ---------Addressing term (1)
  lambda_->update(ftiparam, *lambdaold_, 0.0);
  // ---------End of term (1)

  // ---------Addressing term (3)
  std::shared_ptr<Core::LinAlg::Vector<double>> fluidresidual =
      fluid_field()->interface()->extract_fsi_cond_vector(*fluid_field()->rhs());
  fluidresidual->scale(-1.0);  // invert sign to obtain residual, not rhs
  tmpvec = std::make_shared<Core::LinAlg::Vector<double>>(*fluidresidual);
  // ---------End of term (3)

  // ---------Addressing term (4)
  auxvec = std::make_shared<Core::LinAlg::Vector<double>>(fggprev_->range_map(), true);
  fggprev_->Apply(*struct_to_fluid(ddginc_), *auxvec);
  tmpvec->update(timescale, *auxvec, 1.0);
  // ---------End of term (4)

  // ---------Addressing term (5)
  if (fmggprev_ != nullptr)
  {
    auxvec = std::make_shared<Core::LinAlg::Vector<double>>(fmggprev_->range_map(), true);
    fmggprev_->Apply(*struct_to_fluid(ddginc_), *auxvec);
    tmpvec->update(1.0, *auxvec, 1.0);
  }
  // ---------End of term (5)

  // ---------Addressing term (6)
  auxvec = std::make_shared<Core::LinAlg::Vector<double>>(fgiprev_->range_map(), true);
  fgiprev_->Apply(*duiinc_, *auxvec);
  tmpvec->update(1.0, *auxvec, 1.0);
  // ---------End of term (6)

  // ---------Addressing term (7)
  if (fmgiprev_ != nullptr)
  {
    /* For matrix-vector-product, the DomainMap() of the matrix and the Map() of the vector
     * have to match. DomainMap() contains inner velocity DOFs and all pressure DOFs.
     * The inner ale displacement increment is converted to the fluid map using AleToFluid().
     * This results in a map that contains all velocity but no pressure DOFs.
     *
     * We have to circumvent some trouble with Core::LinAlg::Maps since we cannot split
     * an Core::LinAlg::Map into inner and interface DOFs.
     *
     * We create a map extractor 'velothermap' in order to extract the inner velocity
     * DOFs after calling AleToFluid(). Afterwards, a second map extractor 'velotherpressuremapext'
     * is used to append pressure DOFs filled with zeros.
     *
     * Finally, maps match and matrix-vector-multiplication can be done.
     */

    // extract inner velocity DOFs after calling AleToFluid()
    std::shared_ptr<Core::LinAlg::Map> velothermap = Core::LinAlg::split_map(
        *fluid_field()->velocity_row_map(), *interface_fluid_ale_coupling().master_dof_map());
    Core::LinAlg::MapExtractor velothermapext =
        Core::LinAlg::MapExtractor(*fluid_field()->velocity_row_map(), velothermap, false);
    auxvec = std::make_shared<Core::LinAlg::Vector<double>>(*velothermap, true);
    velothermapext.extract_other_vector(
        *ale_to_fluid(ale_field()->interface()->insert_other_vector(*ddialeinc_)), *auxvec);

    // add pressure DOFs
    Core::LinAlg::MapExtractor velotherpressuremapext =
        Core::LinAlg::MapExtractor(fmgiprev_->domain_map(), velothermap);
    auxauxvec = std::make_shared<Core::LinAlg::Vector<double>>(fmgiprev_->domain_map(), true);
    velotherpressuremapext.insert_cond_vector(*auxvec, *auxauxvec);

    // prepare vector to store result of matrix-vector-product
    auxvec = std::make_shared<Core::LinAlg::Vector<double>>(fmgiprev_->range_map(), true);

    // Now, do the actual matrix-vector-product
    fmgiprev_->Apply(*auxauxvec, *auxvec);
    tmpvec->update(1.0, *auxvec, 1.0);
  }
  // ---------End of term (7)

  // ---------Addressing term (8)
  if (firstcall_)
  {
    auxvec = std::make_shared<Core::LinAlg::Vector<double>>(fggprev_->range_map(), true);
    fggprev_->Apply(*fluid_field()->extract_interface_veln(), *auxvec);
    tmpvec->update(dt() * timescale, *auxvec, 1.0);
  }
  // ---------End of term (8)

  // ---------Addressing term (2)
  lambda_->update(scale, *tmpvec, 1.0);  // scale with residual_scaling() to get [N/m^2]
  // ---------End of term (2)

  // Finally, divide by (1.0-ftiparam) which is common to all terms
  lambda_->scale(-1.0 / (1.0 - ftiparam));

  // Finally, the Lagrange multiplier 'lambda_' is recovered here.
  // It represents nodal forces acting onto the structure.

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::calculate_interface_energy_increment()
{
  // get time integration parameters of structure and fluid time integrators
  // to enable consistent time integration among the fields
  const double stiparam = structure_field()->tim_int_param();
  const double ftiparam = fluid_field()->tim_int_param();

  // interface traction weighted by time integration factors
  std::shared_ptr<Core::LinAlg::Vector<double>> tractionfluid =
      std::make_shared<Core::LinAlg::Vector<double>>(lambda_->get_map(), true);
  tractionfluid->update(stiparam - ftiparam, *lambdaold_, ftiparam - stiparam, *lambda_, 0.0);
  std::shared_ptr<Core::LinAlg::Vector<double>> tractionstructure = fluid_to_struct(tractionfluid);

  // displacement increment of this time step
  std::shared_ptr<Core::LinAlg::Vector<double>> deltad =
      std::make_shared<Core::LinAlg::Vector<double>>(*structure_field()->dof_row_map(), true);
  deltad->update(1.0, *structure_field()->dispnp(), -1.0, *structure_field()->dispn(), 0.0);

  // calculate the energy increment
  double energy = 0.0;
  tractionstructure->dot(
      *structure_field()->interface()->extract_fsi_cond_vector(*deltad), &energy);

  energysum_ += energy;

  write_interface_energy_file(energy, energysum_);

  return;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
void FSI::MonolithicFluidSplit::combine_field_vectors(Core::LinAlg::Vector<double>& v,
    std::shared_ptr<const Core::LinAlg::Vector<double>> sv,
    std::shared_ptr<const Core::LinAlg::Vector<double>> fv,
    std::shared_ptr<const Core::LinAlg::Vector<double>> av,
    const bool slave_vectors_contain_interface_dofs)
{
  if (slave_vectors_contain_interface_dofs)
  {
    // extract inner DOFs from slave vectors
    std::shared_ptr<Core::LinAlg::Vector<double>> fov =
        fluid_field()->interface()->extract_other_vector(*fv);
    fov = fluid_field()->interface()->insert_other_vector(*fov);
    std::shared_ptr<Core::LinAlg::Vector<double>> aov =
        ale_field()->interface()->extract_other_vector(*av);

    // put them together
    FSI::Monolithic::combine_field_vectors(v, *sv, *fov, *aov);
  }
  else
    FSI::Monolithic::combine_field_vectors(v, *sv, *fv, *av);
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
double FSI::MonolithicFluidSplit::select_dt_error_based() const
{
  // get time step size suggestions based on some error norms
  const double dtstr = get_ada_str_dt();           // based on all structure DOFs
  const double dtstrfsi = get_ada_str_fsi_dt();    // based on structure FSI DOFs
  const double dtflinner = get_ada_fl_inner_dt();  // based on inner fluid DOFs

  double dt = MonolithicFluidSplit::dt();

  // select time step size based on error estimation
  if (is_ada_structure() and is_ada_fluid())
    dt = std::min(std::min(dtstr, dtstrfsi), dtflinner);
  else if (is_ada_structure() and (not is_ada_fluid()))
    dt = std::min(dtstr, dtstrfsi);
  else if ((not is_ada_structure()) and is_ada_fluid())
    dt = dtflinner;
  else
  {
    // no change in time step size based on structure or fluid field error estimation
  }

  return dt;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
bool FSI::MonolithicFluidSplit::set_accepted() const
{
  // get error norms
  const double strnorm = get_ada_strnorm();            // based on all structure DOFs
  const double strfsinorm = get_ada_str_fs_inorm();    // based on structure FSI DOFs
  const double flinnernorm = get_ada_fl_inner_norm();  // based on inner fluid DOFs

  bool accepted = std::max(strnorm, strfsinorm) < errtolstr_ and flinnernorm < errtolfl_;

  // in case error estimation in the fluid field is turned off:
  if (not is_ada_fluid()) accepted = std::max(strnorm, strfsinorm) < errtolstr_;

  // in case error estimation in the structure field is turned off:
  if (not is_ada_structure()) accepted = flinnernorm < errtolfl_;

  // no error based time adaptivity
  if ((not is_ada_structure()) and (not is_ada_fluid())) accepted = true;

  return accepted;
}

FOUR_C_NAMESPACE_CLOSE
