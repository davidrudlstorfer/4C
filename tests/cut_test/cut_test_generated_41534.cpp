// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_cut_combintersection.hpp"
#include "4C_cut_levelsetintersection.hpp"
#include "4C_cut_meshintersection.hpp"
#include "4C_cut_options.hpp"
#include "4C_cut_side.hpp"
#include "4C_cut_tetmeshintersection.hpp"
#include "4C_cut_volumecell.hpp"
#include "4C_fem_general_utils_local_connectivity_matrices.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cut_test_utils.hpp"

void test_generated_41534()
{
  Cut::MeshIntersection intersection;
  intersection.get_options().init_for_cuttests();  // use full cln
  std::vector<int> nids;

  int sidecount = 0;
  std::vector<double> lsvs(8);
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(32344);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5378);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(33094);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5378);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(32346);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5379);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5379);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(32344);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5379);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(32348);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5380);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5380);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(32346);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5380);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(32350);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5381);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5381);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(32348);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5381);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(32352);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 2.64976e-14;
    nids.push_back(-5382);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 2.64976e-14;
    nids.push_back(-5382);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(32350);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 2.64976e-14;
    nids.push_back(-5382);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(32354);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5383);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5383);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(32352);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5383);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(32356);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5384);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5384);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(32354);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5384);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(32358);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5385);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5385);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(32356);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5385);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(32358);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5386);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5386);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5386);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(32358);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5386);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5387);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5387);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5387);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5387);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5388);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5388);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5388);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5388);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5389);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5389);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5389);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5390);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5390);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5390);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5390);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32383);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5391);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32383);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5391);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5391);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5391);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5513);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5513);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5513);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5515);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5515);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5515);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5515);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33167);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5517);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33919);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5517);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5517);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(33094);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5500);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5500);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(33846);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5500);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(33846);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(33094);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5500);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5501);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5501);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5501);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33096);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5501);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5502);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5502);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5502);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33098);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5502);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5503);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5503);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5503);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33100);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5503);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 1.87905e-14;
    nids.push_back(-5504);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 1.87905e-14;
    nids.push_back(-5504);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 1.87905e-14;
    nids.push_back(-5504);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33102);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 1.87905e-14;
    nids.push_back(-5504);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5505);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5505);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5505);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33104);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5505);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5506);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5506);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5506);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33106);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5506);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5507);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5507);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5507);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33108);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5507);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5508);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5508);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5508);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33110);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5508);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5509);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5509);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5509);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33112);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5509);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5510);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5510);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5510);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33114);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5510);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5511);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5511);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5511);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33116);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5511);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5512);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5512);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5512);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33118);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5512);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33119);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 2) = 0.983696;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5513);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(31606);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5264);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5264);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(32358);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5264);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5265);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5265);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5265);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(32360);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5265);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5266);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5266);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5266);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(32362);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5266);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5267);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5267);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5267);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5267);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31631);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32383);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32367);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 2) = 0.961957;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.945652;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(30856);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5142);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(31606);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5142);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.945652;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(30858);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5143);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5143);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(31608);
    tri3_xyze(0, 1) = 0.945652;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(30856);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5143);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.945652;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(30860);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5144);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5144);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(31610);
    tri3_xyze(0, 1) = 0.945652;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(30858);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5144);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.945652;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(30862);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5145);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5145);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(31612);
    tri3_xyze(0, 1) = 0.945652;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(30860);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5145);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.945652;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(30863);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5146);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5146);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31614);
    tri3_xyze(0, 1) = 0.945652;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(30862);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5146);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31631);
    tri3_xyze(0, 1) = 0.956522;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5147);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.956522;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(31615);
    tri3_xyze(0, 1) = 0.945652;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(30863);
    tri3_xyze(0, 2) = 0.951087;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5147);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5767);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5767);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34651);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5795);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34668);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5795);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5796);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34669);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5796);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34668);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5796);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5797);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34670);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5797);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34669);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5797);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5798);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34671);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5798);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34670);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5798);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5634);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5634);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5635);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5635);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5635);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5635);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5637);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5637);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5637);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33887);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5637);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.205522;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33919);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5639);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34671);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5639);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33903);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5639);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(32364);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32366);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5389);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(34598);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5745);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5745);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(34599);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5745);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(34599);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(34598);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5745);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(34599);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5746);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34628);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5746);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(34627);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(34599);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5746);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5747);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5747);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5747);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5747);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5748);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34629);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5748);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34628);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34601);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5748);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5749);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5749);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5749);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5749);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5750);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34630);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5750);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34629);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34603);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5750);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5751);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5751);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5751);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5751);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5752);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34631);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5752);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34630);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34605);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5752);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 1.00333e-14;
    nids.push_back(-5753);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 1.00333e-14;
    nids.push_back(-5753);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 1.00333e-14;
    nids.push_back(-5753);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = 1.00333e-14;
    nids.push_back(-5753);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 2.14011e-14;
    nids.push_back(-5754);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34632);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 2.14011e-14;
    nids.push_back(-5754);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34631);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34607);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = 2.14011e-14;
    nids.push_back(-5754);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5755);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5755);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5755);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5755);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5756);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34633);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5756);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34632);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34609);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5756);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5757);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5757);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5757);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5757);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5758);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34634);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5758);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34633);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34611);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5758);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5759);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(33846);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5622);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5622);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(34598);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5622);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.012;
    nids.push_back(34598);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.012;
    nids.push_back(33846);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.0106667;
    nids.push_back(-5622);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5623);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5623);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5623);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00933333;
    nids.push_back(34600);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00933333;
    nids.push_back(33848);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.008;
    nids.push_back(-5623);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5624);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5624);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5624);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00666667;
    nids.push_back(34602);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00666667;
    nids.push_back(33850);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00533333;
    nids.push_back(-5624);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5625);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5625);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5625);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.004;
    nids.push_back(34604);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.004;
    nids.push_back(33852);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 0.00266667;
    nids.push_back(-5625);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 8.43325e-15;
    nids.push_back(-5626);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 8.43325e-15;
    nids.push_back(-5626);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 8.43325e-15;
    nids.push_back(-5626);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = 0.00133333;
    nids.push_back(34606);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = 0.00133333;
    nids.push_back(33854);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = 8.43325e-15;
    nids.push_back(-5626);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5627);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5627);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5627);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00133333;
    nids.push_back(34608);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00133333;
    nids.push_back(33856);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00266667;
    nids.push_back(-5627);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5628);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5628);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5628);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.004;
    nids.push_back(34610);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.004;
    nids.push_back(33858);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.00533333;
    nids.push_back(-5628);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5629);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5629);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5629);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(33860);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5629);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5630);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5630);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5630);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(33862);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5630);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5631);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5631);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5631);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(33864);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5631);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5632);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5632);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5632);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(33866);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5632);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5633);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5633);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34622);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5633);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(33868);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.249;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5633);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33870);
    tri3_xyze(0, 1) = 0.98913;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5634);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.98913;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33871);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 2) = 0.994565;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5634);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5759);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5759);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34612);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5759);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5760);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34635);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5760);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.00666667;
    nids.push_back(34634);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00666667;
    nids.push_back(34613);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.008;
    nids.push_back(-5760);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5761);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5761);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5761);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34614);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5761);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5762);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5762);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34635);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5762);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34635);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34615);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5762);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5763);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5763);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5763);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34616);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5763);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5764);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5764);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5764);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34617);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5764);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5765);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.249;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5765);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5765);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34618);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5765);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5766);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5766);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5766);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34619);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5766);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5767);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.249;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34620);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.243565;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5767);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5768);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.23813;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34623);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5768);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5768);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.23813;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34621);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.232696;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5768);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.00933333;
    nids.push_back(34635);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5780);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5780);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.00933333;
    nids.push_back(34651);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0106667;
    nids.push_back(-5780);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5781);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5781);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5781);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.012;
    nids.push_back(34652);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.012;
    nids.push_back(34636);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0133333;
    nids.push_back(-5781);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5782);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5782);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5782);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0146667;
    nids.push_back(34653);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0146667;
    nids.push_back(34637);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.016;
    nids.push_back(-5782);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5783);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34639);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5783);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(34655);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5783);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 1;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.0173333;
    nids.push_back(34654);
    tri3_xyze(0, 1) = 1;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.0173333;
    nids.push_back(34638);
    tri3_xyze(0, 2) = 1;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.0186667;
    nids.push_back(-5783);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.967391;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(32399);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5393);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5393);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.227261;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33135);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.227261;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32383);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.221826;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5393);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.205522;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33167);
    tri3_xyze(0, 1) = 0.978261;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5395);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = 0.978261;
    tri3_xyze(1, 0) = -0.216391;
    tri3_xyze(2, 0) = -0.02;
    nids.push_back(33151);
    tri3_xyze(0, 1) = 0.967391;
    tri3_xyze(1, 1) = -0.216391;
    tri3_xyze(2, 1) = -0.02;
    nids.push_back(32399);
    tri3_xyze(0, 2) = 0.972826;
    tri3_xyze(1, 2) = -0.210957;
    tri3_xyze(2, 2) = -0.02;
    nids.push_back(-5395);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.977778;
    hex8_xyze(1, 0) = -0.255556;
    hex8_xyze(2, 0) = -0.0108696;
    nids.push_back(435620);
    hex8_xyze(0, 1) = 1;
    hex8_xyze(1, 1) = -0.255556;
    hex8_xyze(2, 1) = -0.0108696;
    nids.push_back(435622);
    hex8_xyze(0, 2) = 1;
    hex8_xyze(1, 2) = -0.233333;
    hex8_xyze(2, 2) = -0.0108696;
    nids.push_back(435984);
    hex8_xyze(0, 3) = 0.977778;
    hex8_xyze(1, 3) = -0.233333;
    hex8_xyze(2, 3) = -0.0108696;
    nids.push_back(435982);
    hex8_xyze(0, 4) = 0.977778;
    hex8_xyze(1, 4) = -0.255556;
    hex8_xyze(2, 4) = 0.0108696;
    nids.push_back(468562);
    hex8_xyze(0, 5) = 1;
    hex8_xyze(1, 5) = -0.255556;
    hex8_xyze(2, 5) = 0.0108696;
    nids.push_back(468564);
    hex8_xyze(0, 6) = 1;
    hex8_xyze(1, 6) = -0.233333;
    hex8_xyze(2, 6) = 0.0108696;
    nids.push_back(468926);
    hex8_xyze(0, 7) = 0.977778;
    hex8_xyze(1, 7) = -0.233333;
    hex8_xyze(2, 7) = 0.0108696;
    nids.push_back(468924);

    intersection.add_element(45584, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.977778;
    hex8_xyze(1, 0) = -0.255556;
    hex8_xyze(2, 0) = -0.0543478;
    nids.push_back(369736);
    hex8_xyze(0, 1) = 1;
    hex8_xyze(1, 1) = -0.255556;
    hex8_xyze(2, 1) = -0.0543478;
    nids.push_back(369738);
    hex8_xyze(0, 2) = 1;
    hex8_xyze(1, 2) = -0.233333;
    hex8_xyze(2, 2) = -0.0543478;
    nids.push_back(370100);
    hex8_xyze(0, 3) = 0.977778;
    hex8_xyze(1, 3) = -0.233333;
    hex8_xyze(2, 3) = -0.0543478;
    nids.push_back(370098);
    hex8_xyze(0, 4) = 0.977778;
    hex8_xyze(1, 4) = -0.255556;
    hex8_xyze(2, 4) = -0.0326087;
    nids.push_back(402678);
    hex8_xyze(0, 5) = 1;
    hex8_xyze(1, 5) = -0.255556;
    hex8_xyze(2, 5) = -0.0326087;
    nids.push_back(402680);
    hex8_xyze(0, 6) = 1;
    hex8_xyze(1, 6) = -0.233333;
    hex8_xyze(2, 6) = -0.0326087;
    nids.push_back(403042);
    hex8_xyze(0, 7) = 0.977778;
    hex8_xyze(1, 7) = -0.233333;
    hex8_xyze(2, 7) = -0.0326087;
    nids.push_back(403040);

    intersection.add_element(37484, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.977778;
    hex8_xyze(1, 0) = -0.277778;
    hex8_xyze(2, 0) = -0.0326087;
    nids.push_back(402316);
    hex8_xyze(0, 1) = 1;
    hex8_xyze(1, 1) = -0.277778;
    hex8_xyze(2, 1) = -0.0326087;
    nids.push_back(402318);
    hex8_xyze(0, 2) = 1;
    hex8_xyze(1, 2) = -0.255556;
    hex8_xyze(2, 2) = -0.0326087;
    nids.push_back(402680);
    hex8_xyze(0, 3) = 0.977778;
    hex8_xyze(1, 3) = -0.255556;
    hex8_xyze(2, 3) = -0.0326087;
    nids.push_back(402678);
    hex8_xyze(0, 4) = 0.977778;
    hex8_xyze(1, 4) = -0.277778;
    hex8_xyze(2, 4) = -0.0108696;
    nids.push_back(435258);
    hex8_xyze(0, 5) = 1;
    hex8_xyze(1, 5) = -0.277778;
    hex8_xyze(2, 5) = -0.0108696;
    nids.push_back(435260);
    hex8_xyze(0, 6) = 1;
    hex8_xyze(1, 6) = -0.255556;
    hex8_xyze(2, 6) = -0.0108696;
    nids.push_back(435622);
    hex8_xyze(0, 7) = 0.977778;
    hex8_xyze(1, 7) = -0.255556;
    hex8_xyze(2, 7) = -0.0108696;
    nids.push_back(435620);

    intersection.add_element(41444, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.955556;
    hex8_xyze(1, 0) = -0.255556;
    hex8_xyze(2, 0) = -0.0326087;
    nids.push_back(402676);
    hex8_xyze(0, 1) = 0.977778;
    hex8_xyze(1, 1) = -0.255556;
    hex8_xyze(2, 1) = -0.0326087;
    nids.push_back(402678);
    hex8_xyze(0, 2) = 0.977778;
    hex8_xyze(1, 2) = -0.233333;
    hex8_xyze(2, 2) = -0.0326087;
    nids.push_back(403040);
    hex8_xyze(0, 3) = 0.955556;
    hex8_xyze(1, 3) = -0.233333;
    hex8_xyze(2, 3) = -0.0326087;
    nids.push_back(403038);
    hex8_xyze(0, 4) = 0.955556;
    hex8_xyze(1, 4) = -0.255556;
    hex8_xyze(2, 4) = -0.0108696;
    nids.push_back(435618);
    hex8_xyze(0, 5) = 0.977778;
    hex8_xyze(1, 5) = -0.255556;
    hex8_xyze(2, 5) = -0.0108696;
    nids.push_back(435620);
    hex8_xyze(0, 6) = 0.977778;
    hex8_xyze(1, 6) = -0.233333;
    hex8_xyze(2, 6) = -0.0108696;
    nids.push_back(435982);
    hex8_xyze(0, 7) = 0.955556;
    hex8_xyze(1, 7) = -0.233333;
    hex8_xyze(2, 7) = -0.0108696;
    nids.push_back(435980);

    intersection.add_element(41533, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.977778;
    hex8_xyze(1, 0) = -0.255556;
    hex8_xyze(2, 0) = -0.0326087;
    nids.push_back(402678);
    hex8_xyze(0, 1) = 1;
    hex8_xyze(1, 1) = -0.255556;
    hex8_xyze(2, 1) = -0.0326087;
    nids.push_back(402680);
    hex8_xyze(0, 2) = 1;
    hex8_xyze(1, 2) = -0.233333;
    hex8_xyze(2, 2) = -0.0326087;
    nids.push_back(403042);
    hex8_xyze(0, 3) = 0.977778;
    hex8_xyze(1, 3) = -0.233333;
    hex8_xyze(2, 3) = -0.0326087;
    nids.push_back(403040);
    hex8_xyze(0, 4) = 0.977778;
    hex8_xyze(1, 4) = -0.255556;
    hex8_xyze(2, 4) = -0.0108696;
    nids.push_back(435620);
    hex8_xyze(0, 5) = 1;
    hex8_xyze(1, 5) = -0.255556;
    hex8_xyze(2, 5) = -0.0108696;
    nids.push_back(435622);
    hex8_xyze(0, 6) = 1;
    hex8_xyze(1, 6) = -0.233333;
    hex8_xyze(2, 6) = -0.0108696;
    nids.push_back(435984);
    hex8_xyze(0, 7) = 0.977778;
    hex8_xyze(1, 7) = -0.233333;
    hex8_xyze(2, 7) = -0.0108696;
    nids.push_back(435982);

    intersection.add_element(41534, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 1;
    hex8_xyze(1, 0) = -0.255556;
    hex8_xyze(2, 0) = -0.0326087;
    nids.push_back(402680);
    hex8_xyze(0, 1) = 1.02222;
    hex8_xyze(1, 1) = -0.255556;
    hex8_xyze(2, 1) = -0.0326087;
    nids.push_back(402682);
    hex8_xyze(0, 2) = 1.02222;
    hex8_xyze(1, 2) = -0.233333;
    hex8_xyze(2, 2) = -0.0326087;
    nids.push_back(403044);
    hex8_xyze(0, 3) = 1;
    hex8_xyze(1, 3) = -0.233333;
    hex8_xyze(2, 3) = -0.0326087;
    nids.push_back(403042);
    hex8_xyze(0, 4) = 1;
    hex8_xyze(1, 4) = -0.255556;
    hex8_xyze(2, 4) = -0.0108696;
    nids.push_back(435622);
    hex8_xyze(0, 5) = 1.02222;
    hex8_xyze(1, 5) = -0.255556;
    hex8_xyze(2, 5) = -0.0108696;
    nids.push_back(435624);
    hex8_xyze(0, 6) = 1.02222;
    hex8_xyze(1, 6) = -0.233333;
    hex8_xyze(2, 6) = -0.0108696;
    nids.push_back(435986);
    hex8_xyze(0, 7) = 1;
    hex8_xyze(1, 7) = -0.233333;
    hex8_xyze(2, 7) = -0.0108696;
    nids.push_back(435984);

    intersection.add_element(41535, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = 0.977778;
    hex8_xyze(1, 0) = -0.233333;
    hex8_xyze(2, 0) = -0.0326087;
    nids.push_back(403040);
    hex8_xyze(0, 1) = 1;
    hex8_xyze(1, 1) = -0.233333;
    hex8_xyze(2, 1) = -0.0326087;
    nids.push_back(403042);
    hex8_xyze(0, 2) = 1;
    hex8_xyze(1, 2) = -0.211111;
    hex8_xyze(2, 2) = -0.0326087;
    nids.push_back(403404);
    hex8_xyze(0, 3) = 0.977778;
    hex8_xyze(1, 3) = -0.211111;
    hex8_xyze(2, 3) = -0.0326087;
    nids.push_back(403402);
    hex8_xyze(0, 4) = 0.977778;
    hex8_xyze(1, 4) = -0.233333;
    hex8_xyze(2, 4) = -0.0108696;
    nids.push_back(435982);
    hex8_xyze(0, 5) = 1;
    hex8_xyze(1, 5) = -0.233333;
    hex8_xyze(2, 5) = -0.0108696;
    nids.push_back(435984);
    hex8_xyze(0, 6) = 1;
    hex8_xyze(1, 6) = -0.211111;
    hex8_xyze(2, 6) = -0.0108696;
    nids.push_back(436346);
    hex8_xyze(0, 7) = 0.977778;
    hex8_xyze(1, 7) = -0.211111;
    hex8_xyze(2, 7) = -0.0108696;
    nids.push_back(436344);

    intersection.add_element(41624, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  intersection.cut_test_cut(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation);
  intersection.cut_finalize(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation, false, true);
}
