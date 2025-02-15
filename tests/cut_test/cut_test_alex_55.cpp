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

void test_alex55()
{
  Cut::MeshIntersection intersection;
  intersection.get_options().init_for_cuttests();  // use full cln
  std::vector<int> nids;

  int sidecount = 0;
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = -0.0001;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.01573333333;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, -350469331, -1088801054, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 545174512, 1066409062, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(456);
    nids.push_back(551);
    nids.push_back(552);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.1283;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.01573333333;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263277, 1067460993, 1745474708, 1069575202};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 545174512, 1066409062, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(551);
    nids.push_back(547);
    nids.push_back(552);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.1283;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, -1846263277, 1067460993, 1745474708, 1069575202};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853487, 1067992272, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(547);
    nids.push_back(551);
    nids.push_back(554);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.06323333333;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 1252985125, 1068511247, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853487, 1067992272, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(551);
    nids.push_back(553);
    nids.push_back(554);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.06323333333;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.06323333333;
    tri3_xyze(2, 1) = 0.1283;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, 1252985125, 1068511247, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 1252985126, 1068511247, 1745474708, 1069575202};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853487, 1067992272, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(553);
    nids.push_back(549);
    nids.push_back(554);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.06323333333;
    tri3_xyze(2, 0) = 0.1283;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.1283;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.11225;

    {
      int data[] = {1999736773, 1072505133, 1252985126, 1068511247, 1745474708, 1069575202};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263277, 1067460993, 1745474708, 1069575202};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853487, 1067992272, 2130303778, 1069333610};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(549);
    nids.push_back(547);
    nids.push_back(554);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.0641;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.01573333333;
    tri3_xyze(2, 2) = 0.08015;

    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, -1951633140, 1068525787};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 545174512, 1066409062, -590987500, 1068795061};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(555);
    nids.push_back(551);
    nids.push_back(556);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = -0.0001;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.01573333333;
    tri3_xyze(2, 2) = 0.08015;

    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -350469331, -1088801054, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 545174512, 1066409062, -590987500, 1068795061};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(551);
    nids.push_back(456);
    nids.push_back(556);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.03156666667;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.0641;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.08015;

    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, -1951633140, 1068525787};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853486, 1067992272, -590987500, 1068795061};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(551);
    nids.push_back(555);
    nids.push_back(558);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.06323333333;
    tri3_xyze(2, 0) = 0.0641;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.06323333333;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.08015;

    {
      int data[] = {1999736773, 1072505133, 1252985125, 1068511247, -1951633140, 1068525787};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 1252985125, 1068511247, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853486, 1067992272, -590987500, 1068795061};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(557);
    nids.push_back(553);
    nids.push_back(558);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    tri3_xyze(0, 0) = 0.9103;
    tri3_xyze(1, 0) = 0.06323333333;
    tri3_xyze(2, 0) = 0.0962;
    tri3_xyze(0, 1) = 0.9103;
    tri3_xyze(1, 1) = 0.03156666667;
    tri3_xyze(2, 1) = 0.0962;
    tri3_xyze(0, 2) = 0.9103;
    tri3_xyze(1, 2) = 0.0474;
    tri3_xyze(2, 2) = 0.08015;

    {
      int data[] = {1999736773, 1072505133, 1252985125, 1068511247, 769658140, 1069064336};
      std::memcpy(&tri3_xyze(0, 0), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, -1846263278, 1067460993, 769658141, 1069064336};
      std::memcpy(&tri3_xyze(0, 1), data, 3 * sizeof(double));
    }
    {
      int data[] = {1999736773, 1072505133, 329853486, 1067992272, -590987500, 1068795061};
      std::memcpy(&tri3_xyze(0, 2), data, 3 * sizeof(double));
    }

    nids.clear();
    nids.push_back(553);
    nids.push_back(551);
    nids.push_back(558);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

  hex8_xyze(0, 0) = 0.9253731343;
  hex8_xyze(1, 0) = 0.02962962963;
  hex8_xyze(2, 0) = 0.08823529412;
  hex8_xyze(0, 1) = 0.9253731343;
  hex8_xyze(1, 1) = 0.05925925926;
  hex8_xyze(2, 1) = 0.08823529412;
  hex8_xyze(0, 2) = 0.8955223881;
  hex8_xyze(1, 2) = 0.05925925926;
  hex8_xyze(2, 2) = 0.08823529412;
  hex8_xyze(0, 3) = 0.8955223881;
  hex8_xyze(1, 3) = 0.02962962963;
  hex8_xyze(2, 3) = 0.08823529412;
  hex8_xyze(0, 4) = 0.9253731343;
  hex8_xyze(1, 4) = 0.02962962963;
  hex8_xyze(2, 4) = 0.1176470588;
  hex8_xyze(0, 5) = 0.9253731343;
  hex8_xyze(1, 5) = 0.05925925926;
  hex8_xyze(2, 5) = 0.1176470588;
  hex8_xyze(0, 6) = 0.8955223881;
  hex8_xyze(1, 6) = 0.05925925926;
  hex8_xyze(2, 6) = 0.1176470588;
  hex8_xyze(0, 7) = 0.8955223881;
  hex8_xyze(1, 7) = 0.02962962963;
  hex8_xyze(2, 7) = 0.1176470588;

  {
    int data[] = {512831913, 1072536744, -922622551, 1067341626, -1768515956, 1068930710};
    std::memcpy(&hex8_xyze(0, 0), data, 3 * sizeof(double));
  }
  {
    int data[] = {512831913, 1072536744, -922622587, 1068390202, -1768515956, 1068930710};
    std::memcpy(&hex8_xyze(0, 1), data, 3 * sizeof(double));
  }
  {
    int data[] = {-1859015699, 1072474142, -922622587, 1068390202, -1768515956, 1068930710};
    std::memcpy(&hex8_xyze(0, 2), data, 3 * sizeof(double));
  }
  {
    int data[] = {-1859015697, 1072474142, -922622551, 1067341626, -1768515956, 1068930710};
    std::memcpy(&hex8_xyze(0, 3), data, 3 * sizeof(double));
  }
  {
    int data[] = {512831914, 1072536744, -922622526, 1067341626, 505290247, 1069424158};
    std::memcpy(&hex8_xyze(0, 4), data, 3 * sizeof(double));
  }
  {
    int data[] = {512831914, 1072536744, -922622575, 1068390202, 505290247, 1069424158};
    std::memcpy(&hex8_xyze(0, 5), data, 3 * sizeof(double));
  }
  {
    int data[] = {-1859015698, 1072474142, -922622575, 1068390202, 505290247, 1069424158};
    std::memcpy(&hex8_xyze(0, 6), data, 3 * sizeof(double));
  }
  {
    int data[] = {-1859015696, 1072474142, -922622526, 1067341626, 505290247, 1069424158};
    std::memcpy(&hex8_xyze(0, 7), data, 3 * sizeof(double));
  }

  nids.clear();
  for (int i = 0; i < 8; ++i) nids.push_back(i);

  intersection.add_element(1, nids, hex8_xyze, Core::FE::CellType::hex8);
  intersection.cut_test_cut(true, Cut::VCellGaussPts_DirectDivergence);
}
