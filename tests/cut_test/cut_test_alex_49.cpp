// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

void test_alex49()
{
  Cut::MeshIntersection intersection;
  intersection.get_options().init_for_cuttests();  // use full cln
  std::vector<int> nids;

  int sidecount = 0;
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152615e-01;
    tri3_xyze(1, 0) = 5.824657e-02;
    tri3_xyze(2, 0) = 1.925487e-01;
    tri3_xyze(0, 1) = 9.152529e-01;
    tri3_xyze(1, 1) = 5.826475e-02;
    tri3_xyze(2, 1) = 2.247628e-01;
    tri3_xyze(0, 2) = 9.137422e-01;
    tri3_xyze(1, 2) = 4.361683e-02;
    tri3_xyze(2, 2) = 2.086372e-01;
    nids.clear();
    nids.push_back(541);
    nids.push_back(537);
    nids.push_back(542);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152615e-01;
    tri3_xyze(1, 0) = 5.824657e-02;
    tri3_xyze(2, 0) = 1.925487e-01;
    tri3_xyze(0, 1) = 9.199503e-01;
    tri3_xyze(1, 1) = 8.768309e-02;
    tri3_xyze(2, 1) = 1.925545e-01;
    tri3_xyze(0, 2) = 9.175974e-01;
    tri3_xyze(1, 2) = 7.297176e-02;
    tri3_xyze(2, 2) = 2.086629e-01;
    nids.clear();
    nids.push_back(541);
    nids.push_back(571);
    nids.push_back(572);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152529e-01;
    tri3_xyze(1, 0) = 5.826475e-02;
    tri3_xyze(2, 0) = 2.247628e-01;
    tri3_xyze(0, 1) = 9.152615e-01;
    tri3_xyze(1, 1) = 5.824657e-02;
    tri3_xyze(2, 1) = 1.925487e-01;
    tri3_xyze(0, 2) = 9.175974e-01;
    tri3_xyze(1, 2) = 7.297176e-02;
    tri3_xyze(2, 2) = 2.086629e-01;
    nids.clear();
    nids.push_back(537);
    nids.push_back(541);
    nids.push_back(572);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.199503e-01;
    tri3_xyze(1, 0) = 8.768309e-02;
    tri3_xyze(2, 0) = 1.925545e-01;
    tri3_xyze(0, 1) = 9.152615e-01;
    tri3_xyze(1, 1) = 5.824657e-02;
    tri3_xyze(2, 1) = 1.925487e-01;
    tri3_xyze(0, 2) = 9.175918e-01;
    tri3_xyze(1, 2) = 7.296768e-02;
    tri3_xyze(2, 2) = 1.764727e-01;
    nids.clear();
    nids.push_back(571);
    nids.push_back(541);
    nids.push_back(574);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.122270e-01;
    tri3_xyze(1, 0) = 2.896875e-02;
    tri3_xyze(2, 0) = 1.925313e-01;
    tri3_xyze(0, 1) = 9.122100e-01;
    tri3_xyze(1, 1) = 2.897082e-02;
    tri3_xyze(2, 1) = 1.603983e-01;
    tri3_xyze(0, 2) = 9.137331e-01;
    tri3_xyze(1, 2) = 4.360893e-02;
    tri3_xyze(2, 2) = 1.764686e-01;
    nids.clear();
    nids.push_back(539);
    nids.push_back(543);
    nids.push_back(546);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.122273e-01;
    tri3_xyze(1, 0) = 2.898726e-02;
    tri3_xyze(2, 0) = 2.247060e-01;
    tri3_xyze(0, 1) = 9.122270e-01;
    tri3_xyze(1, 1) = 2.896875e-02;
    tri3_xyze(2, 1) = 1.925313e-01;
    tri3_xyze(0, 2) = 9.137422e-01;
    tri3_xyze(1, 2) = 4.361683e-02;
    tri3_xyze(2, 2) = 2.086372e-01;
    nids.clear();
    nids.push_back(535);
    nids.push_back(539);
    nids.push_back(542);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152615e-01;
    tri3_xyze(1, 0) = 5.824657e-02;
    tri3_xyze(2, 0) = 1.925487e-01;
    tri3_xyze(0, 1) = 9.152340e-01;
    tri3_xyze(1, 1) = 5.824957e-02;
    tri3_xyze(2, 1) = 1.603964e-01;
    tri3_xyze(0, 2) = 9.175918e-01;
    tri3_xyze(1, 2) = 7.296768e-02;
    tri3_xyze(2, 2) = 1.764727e-01;
    nids.clear();
    nids.push_back(541);
    nids.push_back(545);
    nids.push_back(574);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.122270e-01;
    tri3_xyze(1, 0) = 2.896875e-02;
    tri3_xyze(2, 0) = 1.925313e-01;
    tri3_xyze(0, 1) = 9.152615e-01;
    tri3_xyze(1, 1) = 5.824657e-02;
    tri3_xyze(2, 1) = 1.925487e-01;
    tri3_xyze(0, 2) = 9.137422e-01;
    tri3_xyze(1, 2) = 4.361683e-02;
    tri3_xyze(2, 2) = 2.086372e-01;
    nids.clear();
    nids.push_back(539);
    nids.push_back(541);
    nids.push_back(542);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152340e-01;
    tri3_xyze(1, 0) = 5.824957e-02;
    tri3_xyze(2, 0) = 1.603964e-01;
    tri3_xyze(0, 1) = 9.152615e-01;
    tri3_xyze(1, 1) = 5.824657e-02;
    tri3_xyze(2, 1) = 1.925487e-01;
    tri3_xyze(0, 2) = 9.137331e-01;
    tri3_xyze(1, 2) = 4.360893e-02;
    tri3_xyze(2, 2) = 1.764686e-01;
    nids.clear();
    nids.push_back(545);
    nids.push_back(541);
    nids.push_back(546);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 9.152615e-01;
    tri3_xyze(1, 0) = 5.824657e-02;
    tri3_xyze(2, 0) = 1.925487e-01;
    tri3_xyze(0, 1) = 9.122270e-01;
    tri3_xyze(1, 1) = 2.896875e-02;
    tri3_xyze(2, 1) = 1.925313e-01;
    tri3_xyze(0, 2) = 9.137331e-01;
    tri3_xyze(1, 2) = 4.360893e-02;
    tri3_xyze(2, 2) = 1.764686e-01;
    nids.clear();
    nids.push_back(541);
    nids.push_back(539);
    nids.push_back(546);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

  hex8_xyze(0, 0) = 9.253731e-01;
  hex8_xyze(1, 0) = 2.962963e-02;
  hex8_xyze(2, 0) = 1.764706e-01;
  hex8_xyze(0, 1) = 9.253731e-01;
  hex8_xyze(1, 1) = 5.925926e-02;
  hex8_xyze(2, 1) = 1.764706e-01;
  hex8_xyze(0, 2) = 8.955224e-01;
  hex8_xyze(1, 2) = 5.925926e-02;
  hex8_xyze(2, 2) = 1.764706e-01;
  hex8_xyze(0, 3) = 8.955224e-01;
  hex8_xyze(1, 3) = 2.962963e-02;
  hex8_xyze(2, 3) = 1.764706e-01;
  hex8_xyze(0, 4) = 9.253731e-01;
  hex8_xyze(1, 4) = 2.962963e-02;
  hex8_xyze(2, 4) = 2.058824e-01;
  hex8_xyze(0, 5) = 9.253731e-01;
  hex8_xyze(1, 5) = 5.925926e-02;
  hex8_xyze(2, 5) = 2.058824e-01;
  hex8_xyze(0, 6) = 8.955224e-01;
  hex8_xyze(1, 6) = 5.925926e-02;
  hex8_xyze(2, 6) = 2.058824e-01;
  hex8_xyze(0, 7) = 8.955224e-01;
  hex8_xyze(1, 7) = 2.962963e-02;
  hex8_xyze(2, 7) = 2.058824e-01;

  nids.clear();
  for (int i = 0; i < 8; ++i) nids.push_back(i);

  intersection.add_element(1, nids, hex8_xyze, Core::FE::CellType::hex8);
  intersection.cut_test_cut(true, Cut::VCellGaussPts_DirectDivergence);
}
