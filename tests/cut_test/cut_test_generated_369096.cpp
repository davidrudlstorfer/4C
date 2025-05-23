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

void test_generated_369096()
{
  Cut::MeshIntersection intersection;
  intersection.get_options().init_for_cuttests();  // use full cln
  std::vector<int> nids;

  int sidecount = 0;
  std::vector<double> lsvs(8);
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.15578953642382692424;
    tri3_xyze(1, 0) = 0.33081333459436645938;
    tri3_xyze(2, 0) = -0.35617755554977048593;
    nids.push_back(30167);
    tri3_xyze(0, 1) = -0.18026475132540792368;
    tri3_xyze(1, 1) = 0.31850315005013474234;
    tri3_xyze(2, 1) = -0.34515857058472027008;
    nids.push_back(29806);
    tri3_xyze(0, 2) = -0.16045677374854358521;
    tri3_xyze(1, 2) = 0.32740788158476591407;
    tri3_xyze(2, 2) = -0.35046734213091546017;
    nids.push_back(-3268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.1880689510804707254;
    tri3_xyze(1, 0) = 0.31303613216999026658;
    tri3_xyze(2, 0) = -0.36891601415642483186;
    nids.push_back(30606);
    tri3_xyze(0, 1) = -0.17465078855573273886;
    tri3_xyze(1, 1) = 0.32196323541773003152;
    tri3_xyze(2, 1) = -0.36846538653851473866;
    nids.push_back(30534);
    tri3_xyze(0, 2) = -0.16915861119059610873;
    tri3_xyze(1, 2) = 0.32370840044281695702;
    tri3_xyze(2, 2) = -0.37419968037253864557;
    nids.push_back(-3352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17465078855573273886;
    tri3_xyze(1, 0) = 0.32196323541773003152;
    tri3_xyze(2, 0) = -0.36846538653851473866;
    nids.push_back(30534);
    tri3_xyze(0, 1) = -0.14994517784879118905;
    tri3_xyze(1, 1) = 0.33388429788868712045;
    tri3_xyze(2, 1) = -0.37948829808867440283;
    nids.push_back(30895);
    tri3_xyze(0, 2) = -0.16915861119059610873;
    tri3_xyze(1, 2) = 0.32370840044281695702;
    tri3_xyze(2, 2) = -0.37419968037253864557;
    nids.push_back(-3352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.15578953642382692424;
    tri3_xyze(1, 0) = 0.33081333459436645938;
    tri3_xyze(2, 0) = -0.35617755554977048593;
    nids.push_back(30167);
    tri3_xyze(0, 1) = -0.17063526721107025175;
    tri3_xyze(1, 1) = 0.32453807554352198661;
    tri3_xyze(2, 1) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 2) = -0.17532664244194062775;
    tri3_xyze(1, 2) = 0.32125763382014504987;
    tri3_xyze(2, 2) = -0.35087549665396622345;
    nids.push_back(-3269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17063526721107025175;
    tri3_xyze(1, 0) = 0.32453807554352198661;
    tri3_xyze(2, 0) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 1) = -0.19461701480745746684;
    tri3_xyze(1, 1) = 0.31117597509255690014;
    tri3_xyze(2, 1) = -0.34557666394289426881;
    nids.push_back(29808);
    tri3_xyze(0, 2) = -0.17532664244194062775;
    tri3_xyze(1, 2) = 0.32125763382014504987;
    tri3_xyze(2, 2) = -0.35087549665396622345;
    nids.push_back(-3269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.18026475132540792368;
    tri3_xyze(1, 0) = 0.31850315005013474234;
    tri3_xyze(2, 0) = -0.34515857058472027008;
    nids.push_back(29806);
    tri3_xyze(0, 1) = -0.15578953642382692424;
    tri3_xyze(1, 1) = 0.33081333459436645938;
    tri3_xyze(2, 1) = -0.35617755554977048593;
    nids.push_back(30167);
    tri3_xyze(0, 2) = -0.17532664244194062775;
    tri3_xyze(1, 2) = 0.32125763382014504987;
    tri3_xyze(2, 2) = -0.35087549665396622345;
    nids.push_back(-3269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17063526721107025175;
    tri3_xyze(1, 0) = 0.32453807554352198661;
    tri3_xyze(2, 0) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 1) = -0.18488518651363913814;
    tri3_xyze(1, 1) = 0.31701418425112054544;
    tri3_xyze(2, 1) = -0.35701766019810948771;
    nids.push_back(30171);
    tri3_xyze(0, 2) = -0.18960672942627898596;
    tri3_xyze(1, 2) = 0.31384451490117992734;
    tri3_xyze(2, 2) = -0.35129810253986615276;
    nids.push_back(-3270);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.19461701480745746684;
    tri3_xyze(1, 0) = 0.31117597509255690014;
    tri3_xyze(2, 0) = -0.34557666394289426881;
    nids.push_back(29808);
    tri3_xyze(0, 1) = -0.17063526721107025175;
    tri3_xyze(1, 1) = 0.32453807554352198661;
    tri3_xyze(2, 1) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 2) = -0.18960672942627898596;
    tri3_xyze(1, 2) = 0.31384451490117992734;
    tri3_xyze(2, 2) = -0.35129810253986615276;
    nids.push_back(-3270);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.14574960991965368873;
    tri3_xyze(1, 0) = 0.3361498780639987638;
    tri3_xyze(2, 0) = -0.36760879538345764272;
    nids.push_back(30530);
    tri3_xyze(0, 1) = -0.16050462134856660623;
    tri3_xyze(1, 1) = 0.32967889302806380192;
    tri3_xyze(2, 1) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 2) = -0.16544367124823242121;
    tri3_xyze(1, 2) = 0.32684525772167627444;
    tri3_xyze(2, 2) = -0.36231063619612535742;
    nids.push_back(-3310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.16050462134856660623;
    tri3_xyze(1, 0) = 0.32967889302806380192;
    tri3_xyze(2, 0) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 1) = -0.18488518651363913814;
    tri3_xyze(1, 1) = 0.31701418425112054544;
    tri3_xyze(2, 1) = -0.35701766019810948771;
    nids.push_back(30171);
    tri3_xyze(0, 2) = -0.16544367124823242121;
    tri3_xyze(1, 2) = 0.32684525772167627444;
    tri3_xyze(2, 2) = -0.36231063619612535742;
    nids.push_back(-3310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.18488518651363913814;
    tri3_xyze(1, 0) = 0.31701418425112054544;
    tri3_xyze(2, 0) = -0.35701766019810948771;
    nids.push_back(30171);
    tri3_xyze(0, 1) = -0.17063526721107025175;
    tri3_xyze(1, 1) = 0.32453807554352198661;
    tri3_xyze(2, 1) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 2) = -0.16544367124823242121;
    tri3_xyze(1, 2) = 0.32684525772167627444;
    tri3_xyze(2, 2) = -0.36231063619612535742;
    nids.push_back(-3310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17063526721107025175;
    tri3_xyze(1, 0) = 0.32453807554352198661;
    tri3_xyze(2, 0) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 1) = -0.14574960991965368873;
    tri3_xyze(1, 1) = 0.3361498780639987638;
    tri3_xyze(2, 1) = -0.36760879538345764272;
    nids.push_back(30530);
    tri3_xyze(0, 2) = -0.16544367124823242121;
    tri3_xyze(1, 2) = 0.32684525772167627444;
    tri3_xyze(2, 2) = -0.36231063619612535742;
    nids.push_back(-3310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.16050462134856660623;
    tri3_xyze(1, 0) = 0.32967889302806380192;
    tri3_xyze(2, 0) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 1) = -0.17465078855573273886;
    tri3_xyze(1, 1) = 0.32196323541773003152;
    tri3_xyze(2, 1) = -0.36846538653851473866;
    nids.push_back(30534);
    tri3_xyze(0, 2) = -0.17962003261350922845;
    tri3_xyze(1, 2) = 0.31923721809279742567;
    tri3_xyze(2, 2) = -0.36274402569610442182;
    nids.push_back(-3311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.19843953403609837505;
    tri3_xyze(1, 0) = 0.30829255967427532381;
    tri3_xyze(2, 0) = -0.35746616338333925267;
    nids.push_back(30173);
    tri3_xyze(0, 1) = -0.18488518651363913814;
    tri3_xyze(1, 1) = 0.31701418425112054544;
    tri3_xyze(2, 1) = -0.35701766019810948771;
    nids.push_back(30171);
    tri3_xyze(0, 2) = -0.17962003261350922845;
    tri3_xyze(1, 2) = 0.31923721809279742567;
    tri3_xyze(2, 2) = -0.36274402569610442182;
    nids.push_back(-3311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.18488518651363913814;
    tri3_xyze(1, 0) = 0.31701418425112054544;
    tri3_xyze(2, 0) = -0.35701766019810948771;
    nids.push_back(30171);
    tri3_xyze(0, 1) = -0.16050462134856660623;
    tri3_xyze(1, 1) = 0.32967889302806380192;
    tri3_xyze(2, 1) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 2) = -0.17962003261350922845;
    tri3_xyze(1, 2) = 0.31923721809279742567;
    tri3_xyze(2, 2) = -0.36274402569610442182;
    nids.push_back(-3311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.13528021717245600208;
    tri3_xyze(1, 0) = 0.34055885411217040604;
    tri3_xyze(2, 0) = -0.37906323251383527984;
    nids.push_back(30893);
    tri3_xyze(0, 1) = -0.16050462134856660623;
    tri3_xyze(1, 1) = 0.32967889302806380192;
    tri3_xyze(2, 1) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 2) = -0.1404076790889135784;
    tri3_xyze(1, 2) = 0.33808381271309839722;
    tri3_xyze(2, 2) = -0.37334013770422486145;
    nids.push_back(-3350);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.14574960991965368873;
    tri3_xyze(1, 0) = 0.3361498780639987638;
    tri3_xyze(2, 0) = -0.36760879538345764272;
    nids.push_back(30530);
    tri3_xyze(0, 1) = -0.17063526721107025175;
    tri3_xyze(1, 1) = 0.32453807554352198661;
    tri3_xyze(2, 1) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 2) = -0.15066655223291602894;
    tri3_xyze(1, 2) = 0.33320895248832116398;
    tri3_xyze(2, 2) = -0.36189678738736563712;
    nids.push_back(-3309);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17063526721107025175;
    tri3_xyze(1, 0) = 0.32453807554352198661;
    tri3_xyze(2, 0) = -0.35658919653847992448;
    nids.push_back(30169);
    tri3_xyze(0, 1) = -0.15578953642382692424;
    tri3_xyze(1, 1) = 0.33081333459436645938;
    tri3_xyze(2, 1) = -0.35617755554977048593;
    nids.push_back(30167);
    tri3_xyze(0, 2) = -0.15066655223291602894;
    tri3_xyze(1, 2) = 0.33320895248832116398;
    tri3_xyze(2, 2) = -0.36189678738736563712;
    nids.push_back(-3309);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.16050462134856660623;
    tri3_xyze(1, 0) = 0.32967889302806380192;
    tri3_xyze(2, 0) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 1) = -0.14574960991965368873;
    tri3_xyze(1, 1) = 0.3361498780639987638;
    tri3_xyze(2, 1) = -0.36760879538345764272;
    nids.push_back(30530);
    tri3_xyze(0, 2) = -0.1404076790889135784;
    tri3_xyze(1, 2) = 0.33808381271309839722;
    tri3_xyze(2, 2) = -0.37334013770422486145;
    nids.push_back(-3350);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.14994517784879118905;
    tri3_xyze(1, 0) = 0.33388429788868712045;
    tri3_xyze(2, 0) = -0.37948829808867440283;
    nids.push_back(30895);
    tri3_xyze(0, 1) = -0.17465078855573273886;
    tri3_xyze(1, 1) = 0.32196323541773003152;
    tri3_xyze(2, 1) = -0.36846538653851473866;
    nids.push_back(30534);
    tri3_xyze(0, 2) = -0.15509520123138664793;
    tri3_xyze(1, 2) = 0.33152132011166285386;
    tri3_xyze(2, 2) = -0.37376095245136969902;
    nids.push_back(-3351);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.17465078855573273886;
    tri3_xyze(1, 0) = 0.32196323541773003152;
    tri3_xyze(2, 0) = -0.36846538653851473866;
    nids.push_back(30534);
    tri3_xyze(0, 1) = -0.16050462134856660623;
    tri3_xyze(1, 1) = 0.32967889302806380192;
    tri3_xyze(2, 1) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 2) = -0.15509520123138664793;
    tri3_xyze(1, 2) = 0.33152132011166285386;
    tri3_xyze(2, 2) = -0.37376095245136969902;
    nids.push_back(-3351);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.16050462134856660623;
    tri3_xyze(1, 0) = 0.32967889302806380192;
    tri3_xyze(2, 0) = -0.36802689266445437477;
    nids.push_back(30532);
    tri3_xyze(0, 1) = -0.13528021717245600208;
    tri3_xyze(1, 1) = 0.34055885411217040604;
    tri3_xyze(2, 1) = -0.37906323251383527984;
    nids.push_back(30893);
    tri3_xyze(0, 2) = -0.15509520123138664793;
    tri3_xyze(1, 2) = 0.33152132011166285386;
    tri3_xyze(2, 2) = -0.37376095245136969902;
    nids.push_back(-3351);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.16314110614218244044;
    hex8_xyze(1, 0) = 0.3365426859801072812;
    hex8_xyze(2, 0) = -0.39289473684210446391;
    nids.push_back(402538);
    hex8_xyze(0, 1) = -0.17518475682172152119;
    hex8_xyze(1, 1) = 0.33043350462281251279;
    hex8_xyze(2, 1) = -0.39289473684210446391;
    nids.push_back(402539);
    hex8_xyze(0, 2) = -0.17174976158992308339;
    hex8_xyze(1, 2) = 0.32395441629687499185;
    hex8_xyze(2, 2) = -0.39289473684210446391;
    nids.push_back(402568);
    hex8_xyze(0, 3) = -0.1599422609237082793;
    hex8_xyze(1, 3) = 0.32994380978441889551;
    hex8_xyze(2, 3) = -0.39289473684210446391;
    nids.push_back(402567);
    hex8_xyze(0, 4) = -0.16314110614218232942;
    hex8_xyze(1, 4) = 0.3365426859801072812;
    hex8_xyze(2, 4) = -0.37315789473684218835;
    nids.push_back(390750);
    hex8_xyze(0, 5) = -0.17518475682172141017;
    hex8_xyze(1, 5) = 0.33043350462281251279;
    hex8_xyze(2, 5) = -0.37315789473684218835;
    nids.push_back(390751);
    hex8_xyze(0, 6) = -0.17174976158992297237;
    hex8_xyze(1, 6) = 0.32395441629687499185;
    hex8_xyze(2, 6) = -0.37315789473684218835;
    nids.push_back(390780);
    hex8_xyze(0, 7) = -0.15994226092370816827;
    hex8_xyze(1, 7) = 0.32994380978441889551;
    hex8_xyze(2, 7) = -0.37315789473684218835;
    nids.push_back(390779);

    intersection.add_element(380796, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.15994226092370816827;
    hex8_xyze(1, 0) = 0.32994380978441889551;
    hex8_xyze(2, 0) = -0.37315789473684218835;
    nids.push_back(390779);
    hex8_xyze(0, 1) = -0.17174976158992297237;
    hex8_xyze(1, 1) = 0.32395441629687499185;
    hex8_xyze(2, 1) = -0.37315789473684218835;
    nids.push_back(390780);
    hex8_xyze(0, 2) = -0.16831476635812450682;
    hex8_xyze(1, 2) = 0.31747532797093752643;
    hex8_xyze(2, 2) = -0.37315789473684218835;
    nids.push_back(390809);
    hex8_xyze(0, 3) = -0.15674341570523400713;
    hex8_xyze(1, 3) = 0.32334493358873050983;
    hex8_xyze(2, 3) = -0.37315789473684218835;
    nids.push_back(390808);
    hex8_xyze(0, 4) = -0.15994226092370825154;
    hex8_xyze(1, 4) = 0.32994380978441884;
    hex8_xyze(2, 4) = -0.35342105263158030137;
    nids.push_back(378991);
    hex8_xyze(0, 5) = -0.17174976158992305564;
    hex8_xyze(1, 5) = 0.32395441629687493634;
    hex8_xyze(2, 5) = -0.35342105263158030137;
    nids.push_back(378992);
    hex8_xyze(0, 6) = -0.16831476635812459008;
    hex8_xyze(1, 6) = 0.31747532797093747092;
    hex8_xyze(2, 6) = -0.35342105263158030137;
    nids.push_back(379021);
    hex8_xyze(0, 7) = -0.1567434157052340904;
    hex8_xyze(1, 7) = 0.32334493358873045432;
    hex8_xyze(2, 7) = -0.35342105263158030137;
    nids.push_back(379020);

    intersection.add_element(369125, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.15088475058418834851;
    hex8_xyze(1, 0) = 0.34221307988028037972;
    hex8_xyze(2, 0) = -0.37315789473684218835;
    nids.push_back(390749);
    hex8_xyze(0, 1) = -0.16314110614218232942;
    hex8_xyze(1, 1) = 0.3365426859801072812;
    hex8_xyze(2, 1) = -0.37315789473684218835;
    nids.push_back(390750);
    hex8_xyze(0, 2) = -0.15994226092370816827;
    hex8_xyze(1, 2) = 0.32994380978441889551;
    hex8_xyze(2, 2) = -0.37315789473684218835;
    nids.push_back(390779);
    hex8_xyze(0, 3) = -0.1479262260629297665;
    hex8_xyze(1, 3) = 0.33550301949047095507;
    hex8_xyze(2, 3) = -0.37315789473684218835;
    nids.push_back(390778);
    hex8_xyze(0, 4) = -0.15088475058418843178;
    hex8_xyze(1, 4) = 0.34221307988028032421;
    hex8_xyze(2, 4) = -0.35342105263158030137;
    nids.push_back(378961);
    hex8_xyze(0, 5) = -0.16314110614218241269;
    hex8_xyze(1, 5) = 0.33654268598010722569;
    hex8_xyze(2, 5) = -0.35342105263158030137;
    nids.push_back(378962);
    hex8_xyze(0, 6) = -0.15994226092370825154;
    hex8_xyze(1, 6) = 0.32994380978441884;
    hex8_xyze(2, 6) = -0.35342105263158030137;
    nids.push_back(378991);
    hex8_xyze(0, 7) = -0.14792622606292984977;
    hex8_xyze(1, 7) = 0.33550301949047089956;
    hex8_xyze(2, 7) = -0.35342105263158030137;
    nids.push_back(378990);

    intersection.add_element(369095, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.16314110614218232942;
    hex8_xyze(1, 0) = 0.3365426859801072812;
    hex8_xyze(2, 0) = -0.37315789473684218835;
    nids.push_back(390750);
    hex8_xyze(0, 1) = -0.17518475682172141017;
    hex8_xyze(1, 1) = 0.33043350462281251279;
    hex8_xyze(2, 1) = -0.37315789473684218835;
    nids.push_back(390751);
    hex8_xyze(0, 2) = -0.17174976158992297237;
    hex8_xyze(1, 2) = 0.32395441629687499185;
    hex8_xyze(2, 2) = -0.37315789473684218835;
    nids.push_back(390780);
    hex8_xyze(0, 3) = -0.15994226092370816827;
    hex8_xyze(1, 3) = 0.32994380978441889551;
    hex8_xyze(2, 3) = -0.37315789473684218835;
    nids.push_back(390779);
    hex8_xyze(0, 4) = -0.16314110614218241269;
    hex8_xyze(1, 4) = 0.33654268598010722569;
    hex8_xyze(2, 4) = -0.35342105263158030137;
    nids.push_back(378962);
    hex8_xyze(0, 5) = -0.17518475682172149344;
    hex8_xyze(1, 5) = 0.33043350462281245727;
    hex8_xyze(2, 5) = -0.35342105263158030137;
    nids.push_back(378963);
    hex8_xyze(0, 6) = -0.17174976158992305564;
    hex8_xyze(1, 6) = 0.32395441629687493634;
    hex8_xyze(2, 6) = -0.35342105263158030137;
    nids.push_back(378992);
    hex8_xyze(0, 7) = -0.15994226092370825154;
    hex8_xyze(1, 7) = 0.32994380978441884;
    hex8_xyze(2, 7) = -0.35342105263158030137;
    nids.push_back(378991);

    intersection.add_element(369096, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.17518475682172141017;
    hex8_xyze(1, 0) = 0.33043350462281251279;
    hex8_xyze(2, 0) = -0.37315789473684218835;
    nids.push_back(390751);
    hex8_xyze(0, 1) = -0.18700000000000002731;
    hex8_xyze(1, 1) = 0.32389350101537983662;
    hex8_xyze(2, 1) = -0.37315789473684218835;
    nids.push_back(390752);
    hex8_xyze(0, 2) = -0.18333333333333334814;
    hex8_xyze(1, 2) = 0.31754264805429394114;
    hex8_xyze(2, 2) = -0.37315789473684218835;
    nids.push_back(390781);
    hex8_xyze(0, 3) = -0.17174976158992297237;
    hex8_xyze(1, 3) = 0.32395441629687499185;
    hex8_xyze(2, 3) = -0.37315789473684218835;
    nids.push_back(390780);
    hex8_xyze(0, 4) = -0.17518475682172149344;
    hex8_xyze(1, 4) = 0.33043350462281245727;
    hex8_xyze(2, 4) = -0.35342105263158030137;
    nids.push_back(378963);
    hex8_xyze(0, 5) = -0.18700000000000011058;
    hex8_xyze(1, 5) = 0.3238935010153797811;
    hex8_xyze(2, 5) = -0.35342105263158030137;
    nids.push_back(378964);
    hex8_xyze(0, 6) = -0.1833333333333334314;
    hex8_xyze(1, 6) = 0.31754264805429388563;
    hex8_xyze(2, 6) = -0.35342105263158030137;
    nids.push_back(378993);
    hex8_xyze(0, 7) = -0.17174976158992305564;
    hex8_xyze(1, 7) = 0.32395441629687493634;
    hex8_xyze(2, 7) = -0.35342105263158030137;
    nids.push_back(378992);

    intersection.add_element(369097, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.16314110614218241269;
    hex8_xyze(1, 0) = 0.33654268598010722569;
    hex8_xyze(2, 0) = -0.35342105263158030137;
    nids.push_back(378962);
    hex8_xyze(0, 1) = -0.17518475682172149344;
    hex8_xyze(1, 1) = 0.33043350462281245727;
    hex8_xyze(2, 1) = -0.35342105263158030137;
    nids.push_back(378963);
    hex8_xyze(0, 2) = -0.17174976158992305564;
    hex8_xyze(1, 2) = 0.32395441629687493634;
    hex8_xyze(2, 2) = -0.35342105263158030137;
    nids.push_back(378992);
    hex8_xyze(0, 3) = -0.15994226092370825154;
    hex8_xyze(1, 3) = 0.32994380978441884;
    hex8_xyze(2, 3) = -0.35342105263158030137;
    nids.push_back(378991);
    hex8_xyze(0, 4) = -0.16314110614218241269;
    hex8_xyze(1, 4) = 0.33654268598010722569;
    hex8_xyze(2, 4) = -0.33368421052631519474;
    nids.push_back(367174);
    hex8_xyze(0, 5) = -0.17518475682172149344;
    hex8_xyze(1, 5) = 0.33043350462281245727;
    hex8_xyze(2, 5) = -0.33368421052631519474;
    nids.push_back(367175);
    hex8_xyze(0, 6) = -0.17174976158992305564;
    hex8_xyze(1, 6) = 0.32395441629687493634;
    hex8_xyze(2, 6) = -0.33368421052631519474;
    nids.push_back(367204);
    hex8_xyze(0, 7) = -0.15994226092370825154;
    hex8_xyze(1, 7) = 0.32994380978441884;
    hex8_xyze(2, 7) = -0.33368421052631519474;
    nids.push_back(367203);

    intersection.add_element(357396, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.16633995136065649056;
    hex8_xyze(1, 0) = 0.34314156217579566688;
    hex8_xyze(2, 0) = -0.37315789473684218835;
    nids.push_back(390721);
    hex8_xyze(0, 1) = -0.17861975205351987572;
    hex8_xyze(1, 1) = 0.33691259294875003372;
    hex8_xyze(2, 1) = -0.37315789473684218835;
    nids.push_back(390722);
    hex8_xyze(0, 2) = -0.17518475682172141017;
    hex8_xyze(1, 2) = 0.33043350462281251279;
    hex8_xyze(2, 2) = -0.37315789473684218835;
    nids.push_back(390751);
    hex8_xyze(0, 3) = -0.16314110614218232942;
    hex8_xyze(1, 3) = 0.3365426859801072812;
    hex8_xyze(2, 3) = -0.37315789473684218835;
    nids.push_back(390750);
    hex8_xyze(0, 4) = -0.16633995136065657383;
    hex8_xyze(1, 4) = 0.34314156217579561137;
    hex8_xyze(2, 4) = -0.35342105263158030137;
    nids.push_back(378933);
    hex8_xyze(0, 5) = -0.17861975205351995899;
    hex8_xyze(1, 5) = 0.33691259294874997821;
    hex8_xyze(2, 5) = -0.35342105263158030137;
    nids.push_back(378934);
    hex8_xyze(0, 6) = -0.17518475682172149344;
    hex8_xyze(1, 6) = 0.33043350462281245727;
    hex8_xyze(2, 6) = -0.35342105263158030137;
    nids.push_back(378963);
    hex8_xyze(0, 7) = -0.16314110614218241269;
    hex8_xyze(1, 7) = 0.33654268598010722569;
    hex8_xyze(2, 7) = -0.35342105263158030137;
    nids.push_back(378962);

    intersection.add_element(369067, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  intersection.cut_test_cut(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation);
  intersection.cut_finalize(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation, false, true);
}
