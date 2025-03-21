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

void test_generated_197489()
{
  Cut::MeshIntersection intersection;
  intersection.get_options().init_for_cuttests();  // use full cln
  std::vector<int> nids;

  int sidecount = 0;
  std::vector<double> lsvs(8);
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.2803668070851643157;
    tri3_xyze(1, 0) = -0.24577027998275005616;
    tri3_xyze(2, 0) = -0.084537890842469459729;
    nids.push_back(21942);
    tri3_xyze(0, 1) = -0.26801168156329863157;
    tri3_xyze(1, 1) = -0.25634847947332217721;
    tri3_xyze(2, 1) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 2) = -0.26459658922426110284;
    tri3_xyze(1, 2) = -0.26065449907482368674;
    tri3_xyze(2, 2) = -0.078525220089367325782;
    nids.push_back(-2353);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26801168156329863157;
    tri3_xyze(1, 0) = -0.25634847947332217721;
    tri3_xyze(2, 0) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 1) = -0.24844814453998392834;
    tri3_xyze(1, 1) = -0.27506406191961535468;
    tri3_xyze(2, 1) = -0.072524101898957096801;
    nids.push_back(21583);
    tri3_xyze(0, 2) = -0.26459658922426110284;
    tri3_xyze(1, 2) = -0.26065449907482368674;
    tri3_xyze(2, 2) = -0.078525220089367325782;
    nids.push_back(-2353);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.24844814453998392834;
    tri3_xyze(1, 0) = -0.27506406191961535468;
    tri3_xyze(2, 0) = -0.072524101898957096801;
    nids.push_back(21583);
    tri3_xyze(0, 1) = -0.26155972370859770226;
    tri3_xyze(1, 1) = -0.26543517492360724219;
    tri3_xyze(2, 1) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 2) = -0.26459658922426110284;
    tri3_xyze(1, 2) = -0.26065449907482368674;
    tri3_xyze(2, 2) = -0.078525220089367325782;
    nids.push_back(-2353);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26155972370859770226;
    tri3_xyze(1, 0) = -0.26543517492360724219;
    tri3_xyze(2, 0) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 1) = -0.2803668070851643157;
    tri3_xyze(1, 1) = -0.24577027998275005616;
    tri3_xyze(2, 1) = -0.084537890842469459729;
    nids.push_back(21942);
    tri3_xyze(0, 2) = -0.26459658922426110284;
    tri3_xyze(1, 2) = -0.26065449907482368674;
    tri3_xyze(2, 2) = -0.078525220089367325782;
    nids.push_back(-2353);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.2908018566837546337;
    tri3_xyze(1, 0) = -0.22594647262061773318;
    tri3_xyze(2, 0) = -0.10837965044467855191;
    nids.push_back(22668);
    tri3_xyze(0, 1) = -0.27351555589340520447;
    tri3_xyze(1, 1) = -0.24651201355209087507;
    tri3_xyze(2, 1) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 2) = -0.28824128113719604816;
    tri3_xyze(1, 2) = -0.23084494127032048616;
    tri3_xyze(2, 2) = -0.10241829060701920784;
    nids.push_back(-2434);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27351555589340520447;
    tri3_xyze(1, 0) = -0.24651201355209087507;
    tri3_xyze(2, 0) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 1) = -0.28608169972546920734;
    tri3_xyze(1, 1) = -0.23619504717930928095;
    tri3_xyze(2, 1) = -0.096455402697275802182;
    nids.push_back(22305);
    tri3_xyze(0, 2) = -0.28824128113719604816;
    tri3_xyze(1, 2) = -0.23084494127032048616;
    tri3_xyze(2, 2) = -0.10241829060701920784;
    nids.push_back(-2434);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28608169972546920734;
    tri3_xyze(1, 0) = -0.23619504717930928095;
    tri3_xyze(2, 0) = -0.096455402697275802182;
    nids.push_back(22305);
    tri3_xyze(0, 1) = -0.26801168156329863157;
    tri3_xyze(1, 1) = -0.25634847947332217721;
    tri3_xyze(2, 1) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 2) = -0.28301804920446765879;
    tri3_xyze(1, 2) = -0.2407610318813915018;
    tri3_xyze(2, 2) = -0.0904984974577590362;
    nids.push_back(-2393);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26801168156329863157;
    tri3_xyze(1, 0) = -0.25634847947332217721;
    tri3_xyze(2, 0) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 1) = -0.2803668070851643157;
    tri3_xyze(1, 1) = -0.24577027998275005616;
    tri3_xyze(2, 1) = -0.084537890842469459729;
    nids.push_back(21942);
    tri3_xyze(0, 2) = -0.28301804920446765879;
    tri3_xyze(1, 2) = -0.2407610318813915018;
    tri3_xyze(2, 2) = -0.0904984974577590362;
    nids.push_back(-2393);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28608169972546920734;
    tri3_xyze(1, 0) = -0.23619504717930928095;
    tri3_xyze(2, 0) = -0.096455402697275802182;
    nids.push_back(22305);
    tri3_xyze(0, 1) = -0.27351555589340520447;
    tri3_xyze(1, 1) = -0.24651201355209087507;
    tri3_xyze(2, 1) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 2) = -0.27057918594825569603;
    tri3_xyze(1, 2) = -0.25118973038580322266;
    tri3_xyze(2, 2) = -0.09045459148877052491;
    nids.push_back(-2394);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27351555589340520447;
    tri3_xyze(1, 0) = -0.24651201355209087507;
    tri3_xyze(2, 0) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 1) = -0.2547078066108496297;
    tri3_xyze(1, 1) = -0.26570338133849064066;
    tri3_xyze(2, 1) = -0.084464264302583261235;
    nids.push_back(21946);
    tri3_xyze(0, 2) = -0.27057918594825569603;
    tri3_xyze(1, 2) = -0.25118973038580322266;
    tri3_xyze(2, 2) = -0.09045459148877052491;
    nids.push_back(-2394);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.2547078066108496297;
    tri3_xyze(1, 0) = -0.26570338133849064066;
    tri3_xyze(2, 0) = -0.084464264302583261235;
    nids.push_back(21946);
    tri3_xyze(0, 1) = -0.26801168156329863157;
    tri3_xyze(1, 1) = -0.25634847947332217721;
    tri3_xyze(2, 1) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 2) = -0.27057918594825569603;
    tri3_xyze(1, 2) = -0.25118973038580322266;
    tri3_xyze(2, 2) = -0.09045459148877052491;
    nids.push_back(-2394);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28733362372012893893;
    tri3_xyze(1, 0) = -0.23805831545788488923;
    tri3_xyze(2, 0) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 1) = -0.29612985035866545847;
    tri3_xyze(1, 1) = -0.22439479603483949988;
    tri3_xyze(2, 1) = -0.061276941170846879747;
    nids.push_back(21140);
    tri3_xyze(0, 2) = -0.29983201089849231824;
    tri3_xyze(1, 2) = -0.22033938088866131255;
    tri3_xyze(2, 2) = -0.067167182429967650648;
    nids.push_back(-2308);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.29507109623711441637;
    tri3_xyze(1, 0) = -0.23013649376180614348;
    tri3_xyze(2, 0) = -0.072908837853593838618;
    nids.push_back(21505);
    tri3_xyze(0, 1) = -0.27735972432630207063;
    tri3_xyze(1, 1) = -0.25089662814817359493;
    tri3_xyze(2, 1) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 2) = -0.2909554091383325658;
    tri3_xyze(1, 2) = -0.23392074925843173161;
    tri3_xyze(2, 2) = -0.066996871629586493557;
    nids.push_back(-2309);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27735972432630207063;
    tri3_xyze(1, 0) = -0.25089662814817359493;
    tri3_xyze(2, 0) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 1) = -0.28733362372012893893;
    tri3_xyze(1, 1) = -0.23805831545788488923;
    tri3_xyze(2, 1) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 2) = -0.2909554091383325658;
    tri3_xyze(1, 2) = -0.23392074925843173161;
    tri3_xyze(2, 2) = -0.066996871629586493557;
    nids.push_back(-2309);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28733362372012893893;
    tri3_xyze(1, 0) = -0.23805831545788488923;
    tri3_xyze(2, 0) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 1) = -0.30405719226978483727;
    tri3_xyze(1, 1) = -0.21659155966586221553;
    tri3_xyze(2, 1) = -0.073068402322055039932;
    nids.push_back(21503);
    tri3_xyze(0, 2) = -0.2909554091383325658;
    tri3_xyze(1, 2) = -0.23392074925843173161;
    tri3_xyze(2, 2) = -0.066996871629586493557;
    nids.push_back(-2309);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.29507109623711441637;
    tri3_xyze(1, 0) = -0.23013649376180614348;
    tri3_xyze(2, 0) = -0.072908837853593838618;
    nids.push_back(21505);
    tri3_xyze(0, 1) = -0.28494289489681412508;
    tri3_xyze(1, 1) = -0.24285952484772521665;
    tri3_xyze(2, 1) = -0.072764852574928415296;
    nids.push_back(21507);
    tri3_xyze(0, 2) = -0.28092047386762858396;
    tri3_xyze(1, 2) = -0.2466803868034942171;
    tri3_xyze(2, 2) = -0.066842789760613016692;
    nids.push_back(-2310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28494289489681412508;
    tri3_xyze(1, 0) = -0.24285952484772521665;
    tri3_xyze(2, 0) = -0.072764852574928415296;
    nids.push_back(21507);
    tri3_xyze(0, 1) = -0.26630818001028366826;
    tri3_xyze(1, 1) = -0.26282890045627188558;
    tri3_xyze(2, 1) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 2) = -0.28092047386762858396;
    tri3_xyze(1, 2) = -0.2466803868034942171;
    tri3_xyze(2, 2) = -0.066842789760613016692;
    nids.push_back(-2310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27735972432630207063;
    tri3_xyze(1, 0) = -0.25089662814817359493;
    tri3_xyze(2, 0) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 1) = -0.26630818001028366826;
    tri3_xyze(1, 1) = -0.26282890045627188558;
    tri3_xyze(2, 1) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 2) = -0.26199849850899048498;
    tri3_xyze(1, 2) = -0.26631984196295049871;
    tri3_xyze(2, 2) = -0.054860853619544180937;
    nids.push_back(-2270);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26630818001028366826;
    tri3_xyze(1, 0) = -0.26282890045627188558;
    tri3_xyze(2, 0) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 1) = -0.24620724313536429539;
    tri3_xyze(1, 1) = -0.28131752730765957615;
    tri3_xyze(2, 1) = -0.048813219722751322038;
    nids.push_back(20785);
    tri3_xyze(0, 2) = -0.26199849850899048498;
    tri3_xyze(1, 2) = -0.26631984196295049871;
    tri3_xyze(2, 2) = -0.054860853619544180937;
    nids.push_back(-2270);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25811884656401190563;
    tri3_xyze(1, 0) = -0.2702363119396969382;
    tri3_xyze(2, 0) = -0.048932726141495588856;
    nids.push_back(20783);
    tri3_xyze(0, 1) = -0.27735972432630207063;
    tri3_xyze(1, 1) = -0.25089662814817359493;
    tri3_xyze(2, 1) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 2) = -0.26199849850899048498;
    tri3_xyze(1, 2) = -0.26631984196295049871;
    tri3_xyze(2, 2) = -0.054860853619544180937;
    nids.push_back(-2270);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26630818001028366826;
    tri3_xyze(1, 0) = -0.26282890045627188558;
    tri3_xyze(2, 0) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 1) = -0.25426288368766031267;
    tri3_xyze(1, 1) = -0.2737629199984892292;
    tri3_xyze(2, 1) = -0.060675789661875000058;
    nids.push_back(21148);
    tri3_xyze(0, 2) = -0.25003867163025061959;
    tri3_xyze(1, 2) = -0.27730968983010451101;
    tri3_xyze(2, 2) = -0.054754175327154416819;
    nids.push_back(-2271);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.24620724313536429539;
    tri3_xyze(1, 0) = -0.28131752730765957615;
    tri3_xyze(2, 0) = -0.048813219722751322038;
    nids.push_back(20785);
    tri3_xyze(0, 1) = -0.26630818001028366826;
    tri3_xyze(1, 1) = -0.26282890045627188558;
    tri3_xyze(2, 1) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 2) = -0.25003867163025061959;
    tri3_xyze(1, 2) = -0.27730968983010451101;
    tri3_xyze(2, 2) = -0.054754175327154416819;
    nids.push_back(-2271);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.29171617001122079893;
    tri3_xyze(1, 0) = -0.23411997544768833879;
    tri3_xyze(2, 0) = -0.084632899976135184117;
    nids.push_back(21870);
    tri3_xyze(0, 1) = -0.27374799966884272306;
    tri3_xyze(1, 1) = -0.25466146886843143982;
    tri3_xyze(2, 1) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 2) = -0.28810102918042190812;
    tri3_xyze(1, 2) = -0.23828977145504826041;
    tri3_xyze(2, 2) = -0.078698349867775990729;
    nids.push_back(-2351);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27374799966884272306;
    tri3_xyze(1, 0) = -0.25466146886843143982;
    tri3_xyze(2, 0) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 1) = -0.28494289489681412508;
    tri3_xyze(1, 1) = -0.24285952484772521665;
    tri3_xyze(2, 1) = -0.072764852574928415296;
    nids.push_back(21507);
    tri3_xyze(0, 2) = -0.28810102918042190812;
    tri3_xyze(1, 2) = -0.23828977145504826041;
    tri3_xyze(2, 2) = -0.078698349867775990729;
    nids.push_back(-2351);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.29171617001122079893;
    tri3_xyze(1, 0) = -0.23411997544768833879;
    tri3_xyze(2, 0) = -0.084632899976135184117;
    nids.push_back(21870);
    tri3_xyze(0, 1) = -0.2803668070851643157;
    tri3_xyze(1, 1) = -0.24577027998275005616;
    tri3_xyze(2, 1) = -0.084537890842469459729;
    nids.push_back(21942);
    tri3_xyze(0, 2) = -0.27684767511845637111;
    tri3_xyze(1, 2) = -0.24999672480561926924;
    tri3_xyze(2, 2) = -0.078594952114539595733;
    nids.push_back(-2352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.2803668070851643157;
    tri3_xyze(1, 0) = -0.24577027998275005616;
    tri3_xyze(2, 0) = -0.084537890842469459729;
    nids.push_back(21942);
    tri3_xyze(0, 1) = -0.26155972370859770226;
    tri3_xyze(1, 1) = -0.26543517492360724219;
    tri3_xyze(2, 1) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 2) = -0.27684767511845637111;
    tri3_xyze(1, 2) = -0.24999672480561926924;
    tri3_xyze(2, 2) = -0.078594952114539595733;
    nids.push_back(-2352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26155972370859770226;
    tri3_xyze(1, 0) = -0.26543517492360724219;
    tri3_xyze(2, 0) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 1) = -0.27374799966884272306;
    tri3_xyze(1, 1) = -0.25466146886843143982;
    tri3_xyze(2, 1) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 2) = -0.27684767511845637111;
    tri3_xyze(1, 2) = -0.24999672480561926924;
    tri3_xyze(2, 2) = -0.078594952114539595733;
    nids.push_back(-2352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27374799966884272306;
    tri3_xyze(1, 0) = -0.25466146886843143982;
    tri3_xyze(2, 0) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 1) = -0.29171617001122079893;
    tri3_xyze(1, 1) = -0.23411997544768833879;
    tri3_xyze(2, 1) = -0.084632899976135184117;
    nids.push_back(21870);
    tri3_xyze(0, 2) = -0.27684767511845637111;
    tri3_xyze(1, 2) = -0.24999672480561926924;
    tri3_xyze(2, 2) = -0.078594952114539595733;
    nids.push_back(-2352);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25426288368766031267;
    tri3_xyze(1, 0) = -0.2737629199984892292;
    tri3_xyze(2, 0) = -0.060675789661875000058;
    nids.push_back(21148);
    tri3_xyze(0, 1) = -0.27374799966884272306;
    tri3_xyze(1, 1) = -0.25466146886843143982;
    tri3_xyze(2, 1) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 2) = -0.2577189493881151261;
    tri3_xyze(1, 2) = -0.2693649877509103896;
    tri3_xyze(2, 2) = -0.066626262895025156752;
    nids.push_back(-2312);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26155972370859770226;
    tri3_xyze(1, 0) = -0.26543517492360724219;
    tri3_xyze(2, 0) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 1) = -0.24844814453998392834;
    tri3_xyze(1, 1) = -0.27506406191961535468;
    tri3_xyze(2, 1) = -0.072524101898957096801;
    nids.push_back(21583);
    tri3_xyze(0, 2) = -0.24470553370581515917;
    tri3_xyze(1, 2) = -0.27908227037960636752;
    tri3_xyze(2, 2) = -0.06658209221501996955;
    nids.push_back(-2313);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.24130519048735973864;
    tri3_xyze(1, 0) = -0.28360038721311359167;
    tri3_xyze(2, 0) = -0.060620244278671887861;
    nids.push_back(21220);
    tri3_xyze(0, 1) = -0.26155972370859770226;
    tri3_xyze(1, 1) = -0.26543517492360724219;
    tri3_xyze(2, 1) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 2) = -0.24470553370581515917;
    tri3_xyze(1, 2) = -0.27908227037960636752;
    tri3_xyze(2, 2) = -0.06658209221501996955;
    nids.push_back(-2313);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28736968919220490237;
    tri3_xyze(1, 0) = -0.23139215598820483955;
    tri3_xyze(2, 0) = -0.049502928272956034372;
    nids.push_back(20777);
    tri3_xyze(0, 1) = -0.26952031322136427161;
    tri3_xyze(1, 1) = -0.25150892825134441022;
    tri3_xyze(2, 1) = -0.03753096127872288662;
    nids.push_back(20416);
    tri3_xyze(0, 2) = -0.282297243439400658;
    tri3_xyze(1, 2) = -0.23431176357815403088;
    tri3_xyze(2, 2) = -0.043633540673708795521;
    nids.push_back(-2226);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26952031322136427161;
    tri3_xyze(1, 0) = -0.25150892825134441022;
    tri3_xyze(2, 0) = -0.03753096127872288662;
    nids.push_back(20416);
    tri3_xyze(0, 1) = -0.27776093618937042207;
    tri3_xyze(1, 1) = -0.23751821860111724249;
    tri3_xyze(2, 1) = -0.037772095460652382881;
    nids.push_back(20414);
    tri3_xyze(0, 2) = -0.282297243439400658;
    tri3_xyze(1, 2) = -0.23431176357815403088;
    tri3_xyze(2, 2) = -0.043633540673708795521;
    nids.push_back(-2226);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28736968919220490237;
    tri3_xyze(1, 0) = -0.23139215598820483955;
    tri3_xyze(2, 0) = -0.049502928272956034372;
    nids.push_back(20777);
    tri3_xyze(0, 1) = -0.27881288481265931178;
    tri3_xyze(1, 1) = -0.24520034312614255589;
    tri3_xyze(2, 1) = -0.049291667540834652184;
    nids.push_back(20779);
    tri3_xyze(0, 2) = -0.27391360690215571516;
    tri3_xyze(1, 2) = -0.24818551730755214479;
    tri3_xyze(2, 2) = -0.04341177934498383767;
    nids.push_back(-2227);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27881288481265931178;
    tri3_xyze(1, 0) = -0.24520034312614255589;
    tri3_xyze(2, 0) = -0.049291667540834652184;
    nids.push_back(20779);
    tri3_xyze(0, 1) = -0.25995154038239431937;
    tri3_xyze(1, 1) = -0.26464064186451674576;
    tri3_xyze(2, 1) = -0.037321560287421784441;
    nids.push_back(20418);
    tri3_xyze(0, 2) = -0.27391360690215571516;
    tri3_xyze(1, 2) = -0.24818551730755214479;
    tri3_xyze(2, 2) = -0.04341177934498383767;
    nids.push_back(-2227);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25995154038239431937;
    tri3_xyze(1, 0) = -0.26464064186451674576;
    tri3_xyze(2, 0) = -0.037321560287421784441;
    nids.push_back(20418);
    tri3_xyze(0, 1) = -0.26952031322136427161;
    tri3_xyze(1, 1) = -0.25150892825134441022;
    tri3_xyze(2, 1) = -0.03753096127872288662;
    nids.push_back(20416);
    tri3_xyze(0, 2) = -0.27391360690215571516;
    tri3_xyze(1, 2) = -0.24818551730755214479;
    tri3_xyze(2, 2) = -0.04341177934498383767;
    nids.push_back(-2227);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.30405719226978483727;
    tri3_xyze(1, 0) = -0.21659155966586221553;
    tri3_xyze(2, 0) = -0.073068402322055039932;
    nids.push_back(21503);
    tri3_xyze(0, 1) = -0.28733362372012893893;
    tri3_xyze(1, 1) = -0.23805831545788488923;
    tri3_xyze(2, 1) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 2) = -0.29983201089849231824;
    tri3_xyze(1, 2) = -0.22033938088866131255;
    tri3_xyze(2, 2) = -0.067167182429967650648;
    nids.push_back(-2308);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26801168156329863157;
    tri3_xyze(1, 0) = -0.25634847947332217721;
    tri3_xyze(2, 0) = -0.084476464604561374294;
    nids.push_back(21944);
    tri3_xyze(0, 1) = -0.28608169972546920734;
    tri3_xyze(1, 1) = -0.23619504717930928095;
    tri3_xyze(2, 1) = -0.096455402697275802182;
    nids.push_back(22305);
    tri3_xyze(0, 2) = -0.27057918594825569603;
    tri3_xyze(1, 2) = -0.25118973038580322266;
    tri3_xyze(2, 2) = -0.09045459148877052491;
    nids.push_back(-2394);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27351555589340520447;
    tri3_xyze(1, 0) = -0.24651201355209087507;
    tri3_xyze(2, 0) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 1) = -0.25996519164189313411;
    tri3_xyze(1, 1) = -0.25548335561239216007;
    tri3_xyze(2, 1) = -0.096440986034917866587;
    nids.push_back(22309);
    tri3_xyze(0, 2) = -0.25717718645008674772;
    tri3_xyze(1, 2) = -0.26033493427611492654;
    tri3_xyze(2, 2) = -0.090460727835116550022;
    nids.push_back(-2395);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.2547078066108496297;
    tri3_xyze(1, 0) = -0.26570338133849064066;
    tri3_xyze(2, 0) = -0.084464264302583261235;
    nids.push_back(21946);
    tri3_xyze(0, 1) = -0.27351555589340520447;
    tri3_xyze(1, 1) = -0.24651201355209087507;
    tri3_xyze(2, 1) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 2) = -0.25717718645008674772;
    tri3_xyze(1, 2) = -0.26033493427611492654;
    tri3_xyze(2, 2) = -0.090460727835116550022;
    nids.push_back(-2395);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.29612985035866545847;
    tri3_xyze(1, 0) = -0.22439479603483949988;
    tri3_xyze(2, 0) = -0.061276941170846879747;
    nids.push_back(21140);
    tri3_xyze(0, 1) = -0.28733362372012893893;
    tri3_xyze(1, 1) = -0.23805831545788488923;
    tri3_xyze(2, 1) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 2) = -0.2828244028841324087;
    tri3_xyze(1, 2) = -0.24145639222727102724;
    tri3_xyze(2, 2) = -0.055189276902725251106;
    nids.push_back(-2268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28733362372012893893;
    tri3_xyze(1, 0) = -0.23805831545788488923;
    tri3_xyze(2, 0) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 1) = -0.26902125264507592561;
    tri3_xyze(1, 1) = -0.25817211429021708069;
    tri3_xyze(2, 1) = -0.049097911239752392576;
    nids.push_back(20781);
    tri3_xyze(0, 2) = -0.2828244028841324087;
    tri3_xyze(1, 2) = -0.24145639222727102724;
    tri3_xyze(2, 2) = -0.055189276902725251106;
    nids.push_back(-2268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26902125264507592561;
    tri3_xyze(1, 0) = -0.25817211429021708069;
    tri3_xyze(2, 0) = -0.049097911239752392576;
    nids.push_back(20781);
    tri3_xyze(0, 1) = -0.27881288481265931178;
    tri3_xyze(1, 1) = -0.24520034312614255589;
    tri3_xyze(2, 1) = -0.049291667540834652184;
    nids.push_back(20779);
    tri3_xyze(0, 2) = -0.2828244028841324087;
    tri3_xyze(1, 2) = -0.24145639222727102724;
    tri3_xyze(2, 2) = -0.055189276902725251106;
    nids.push_back(-2268);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28733362372012893893;
    tri3_xyze(1, 0) = -0.23805831545788488923;
    tri3_xyze(2, 0) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 1) = -0.27735972432630207063;
    tri3_xyze(1, 1) = -0.25089662814817359493;
    tri3_xyze(2, 1) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 2) = -0.27295836181387972408;
    tri3_xyze(1, 2) = -0.2543408424589931327;
    tri3_xyze(2, 2) = -0.055010220930986258869;
    nids.push_back(-2269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27735972432630207063;
    tri3_xyze(1, 0) = -0.25089662814817359493;
    tri3_xyze(2, 0) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 1) = -0.25811884656401190563;
    tri3_xyze(1, 1) = -0.2702363119396969382;
    tri3_xyze(2, 1) = -0.048932726141495588856;
    nids.push_back(20783);
    tri3_xyze(0, 2) = -0.27295836181387972408;
    tri3_xyze(1, 2) = -0.2543408424589931327;
    tri3_xyze(2, 2) = -0.055010220930986258869;
    nids.push_back(-2269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25811884656401190563;
    tri3_xyze(1, 0) = -0.2702363119396969382;
    tri3_xyze(2, 0) = -0.048932726141495588856;
    nids.push_back(20783);
    tri3_xyze(0, 1) = -0.26902125264507592561;
    tri3_xyze(1, 1) = -0.25817211429021708069;
    tri3_xyze(2, 1) = -0.049097911239752392576;
    nids.push_back(20781);
    tri3_xyze(0, 2) = -0.27295836181387972408;
    tri3_xyze(1, 2) = -0.2543408424589931327;
    tri3_xyze(2, 2) = -0.055010220930986258869;
    nids.push_back(-2269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26902125264507592561;
    tri3_xyze(1, 0) = -0.25817211429021708069;
    tri3_xyze(2, 0) = -0.049097911239752392576;
    nids.push_back(20781);
    tri3_xyze(0, 1) = -0.28733362372012893893;
    tri3_xyze(1, 1) = -0.23805831545788488923;
    tri3_xyze(2, 1) = -0.061090587659467100734;
    nids.push_back(21142);
    tri3_xyze(0, 2) = -0.27295836181387972408;
    tri3_xyze(1, 2) = -0.2543408424589931327;
    tri3_xyze(2, 2) = -0.055010220930986258869;
    nids.push_back(-2269);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25996519164189313411;
    tri3_xyze(1, 0) = -0.25548335561239216007;
    tri3_xyze(2, 0) = -0.096440986034917866587;
    nids.push_back(22309);
    tri3_xyze(0, 1) = -0.27351555589340520447;
    tri3_xyze(1, 1) = -0.24651201355209087507;
    tri3_xyze(2, 1) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 2) = -0.27555952306328429513;
    tri3_xyze(1, 2) = -0.24095993926458808554;
    tri3_xyze(2, 2) = -0.10240625415726509195;
    nids.push_back(-2435);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27351555589340520447;
    tri3_xyze(1, 0) = -0.24651201355209087507;
    tri3_xyze(2, 0) = -0.096422234350661661928;
    nids.push_back(22307);
    tri3_xyze(0, 1) = -0.2908018566837546337;
    tri3_xyze(1, 1) = -0.22594647262061773318;
    tri3_xyze(2, 1) = -0.10837965044467855191;
    nids.push_back(22668);
    tri3_xyze(0, 2) = -0.27555952306328429513;
    tri3_xyze(1, 2) = -0.24095993926458808554;
    tri3_xyze(2, 2) = -0.10240625415726509195;
    nids.push_back(-2435);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26952031322136427161;
    tri3_xyze(1, 0) = -0.25150892825134441022;
    tri3_xyze(2, 0) = -0.03753096127872288662;
    nids.push_back(20416);
    tri3_xyze(0, 1) = -0.28736968919220490237;
    tri3_xyze(1, 1) = -0.23139215598820483955;
    tri3_xyze(2, 1) = -0.049502928272956034372;
    nids.push_back(20777);
    tri3_xyze(0, 2) = -0.27391360690215571516;
    tri3_xyze(1, 2) = -0.24818551730755214479;
    tri3_xyze(2, 2) = -0.04341177934498383767;
    nids.push_back(-2227);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27881288481265931178;
    tri3_xyze(1, 0) = -0.24520034312614255589;
    tri3_xyze(2, 0) = -0.049291667540834652184;
    nids.push_back(20779);
    tri3_xyze(0, 1) = -0.26902125264507592561;
    tri3_xyze(1, 1) = -0.25817211429021708069;
    tri3_xyze(2, 1) = -0.049097911239752392576;
    nids.push_back(20781);
    tri3_xyze(0, 2) = -0.26425071056604276798;
    tri3_xyze(1, 2) = -0.26121522093164450951;
    tri3_xyze(2, 2) = -0.043213799530352481337;
    nids.push_back(-2228);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25995154038239431937;
    tri3_xyze(1, 0) = -0.26464064186451674576;
    tri3_xyze(2, 0) = -0.037321560287421784441;
    nids.push_back(20418);
    tri3_xyze(0, 1) = -0.27881288481265931178;
    tri3_xyze(1, 1) = -0.24520034312614255589;
    tri3_xyze(2, 1) = -0.049291667540834652184;
    nids.push_back(20779);
    tri3_xyze(0, 2) = -0.26425071056604276798;
    tri3_xyze(1, 2) = -0.26121522093164450951;
    tri3_xyze(2, 2) = -0.043213799530352481337;
    nids.push_back(-2228);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27776093618937042207;
    tri3_xyze(1, 0) = -0.23751821860111724249;
    tri3_xyze(2, 0) = -0.037772095460652382881;
    nids.push_back(20414);
    tri3_xyze(0, 1) = -0.26952031322136427161;
    tri3_xyze(1, 1) = -0.25150892825134441022;
    tri3_xyze(2, 1) = -0.03753096127872288662;
    nids.push_back(20416);
    tri3_xyze(0, 2) = -0.2642246634870675237;
    tri3_xyze(1, 2) = -0.25401616657797865262;
    tri3_xyze(2, 2) = -0.03166369496477748624;
    nids.push_back(-2186);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26630818001028366826;
    tri3_xyze(1, 0) = -0.26282890045627188558;
    tri3_xyze(2, 0) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 1) = -0.27735972432630207063;
    tri3_xyze(1, 1) = -0.25089662814817359493;
    tri3_xyze(2, 1) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 2) = -0.28092047386762858396;
    tri3_xyze(1, 2) = -0.2466803868034942171;
    tri3_xyze(2, 2) = -0.066842789760613016692;
    nids.push_back(-2310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27735972432630207063;
    tri3_xyze(1, 0) = -0.25089662814817359493;
    tri3_xyze(2, 0) = -0.060919658683229946372;
    nids.push_back(21144);
    tri3_xyze(0, 1) = -0.29507109623711441637;
    tri3_xyze(1, 1) = -0.23013649376180614348;
    tri3_xyze(2, 1) = -0.072908837853593838618;
    nids.push_back(21505);
    tri3_xyze(0, 2) = -0.28092047386762858396;
    tri3_xyze(1, 2) = -0.2466803868034942171;
    tri3_xyze(2, 2) = -0.066842789760613016692;
    nids.push_back(-2310);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.28494289489681412508;
    tri3_xyze(1, 0) = -0.24285952484772521665;
    tri3_xyze(2, 0) = -0.072764852574928415296;
    nids.push_back(21507);
    tri3_xyze(0, 1) = -0.27374799966884272306;
    tri3_xyze(1, 1) = -0.25466146886843143982;
    tri3_xyze(2, 1) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 2) = -0.26981548956590017951;
    tri3_xyze(1, 2) = -0.25852820354272942893;
    tri3_xyze(2, 2) = -0.066716261698893941645;
    nids.push_back(-2311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27374799966884272306;
    tri3_xyze(1, 0) = -0.25466146886843143982;
    tri3_xyze(2, 0) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 1) = -0.25426288368766031267;
    tri3_xyze(1, 1) = -0.2737629199984892292;
    tri3_xyze(2, 1) = -0.060675789661875000058;
    nids.push_back(21148);
    tri3_xyze(0, 2) = -0.26981548956590017951;
    tri3_xyze(1, 2) = -0.25852820354272942893;
    tri3_xyze(2, 2) = -0.066716261698893941645;
    nids.push_back(-2311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.25426288368766031267;
    tri3_xyze(1, 0) = -0.2737629199984892292;
    tri3_xyze(2, 0) = -0.060675789661875000058;
    nids.push_back(21148);
    tri3_xyze(0, 1) = -0.26630818001028366826;
    tri3_xyze(1, 1) = -0.26282890045627188558;
    tri3_xyze(2, 1) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 2) = -0.26981548956590017951;
    tri3_xyze(1, 2) = -0.25852820354272942893;
    tri3_xyze(2, 2) = -0.066716261698893941645;
    nids.push_back(-2311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26630818001028366826;
    tri3_xyze(1, 0) = -0.26282890045627188558;
    tri3_xyze(2, 0) = -0.060777809930699894236;
    nids.push_back(21146);
    tri3_xyze(0, 1) = -0.28494289489681412508;
    tri3_xyze(1, 1) = -0.24285952484772521665;
    tri3_xyze(2, 1) = -0.072764852574928415296;
    nids.push_back(21507);
    tri3_xyze(0, 2) = -0.26981548956590017951;
    tri3_xyze(1, 2) = -0.25852820354272942893;
    tri3_xyze(2, 2) = -0.066716261698893941645;
    nids.push_back(-2311);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.27374799966884272306;
    tri3_xyze(1, 0) = -0.25466146886843143982;
    tri3_xyze(2, 0) = -0.072646594628072422295;
    nids.push_back(21509);
    tri3_xyze(0, 1) = -0.26155972370859770226;
    tri3_xyze(1, 1) = -0.26543517492360724219;
    tri3_xyze(2, 1) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 2) = -0.2577189493881151261;
    tri3_xyze(1, 2) = -0.2693649877509103896;
    tri3_xyze(2, 2) = -0.066626262895025156752;
    nids.push_back(-2312);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix tri3_xyze(3, 3);

    nids.clear();
    tri3_xyze(0, 0) = -0.26155972370859770226;
    tri3_xyze(1, 0) = -0.26543517492360724219;
    tri3_xyze(2, 0) = -0.07256242301148133067;
    nids.push_back(21581);
    tri3_xyze(0, 1) = -0.24130519048735973864;
    tri3_xyze(1, 1) = -0.28360038721311359167;
    tri3_xyze(2, 1) = -0.060620244278671887861;
    nids.push_back(21220);
    tri3_xyze(0, 2) = -0.2577189493881151261;
    tri3_xyze(1, 2) = -0.2693649877509103896;
    tri3_xyze(2, 2) = -0.066626262895025156752;
    nids.push_back(-2312);
    intersection.add_cut_side(++sidecount, nids, tri3_xyze, Core::FE::CellType::tri3);
  }
  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.28937346392544061047;
    hex8_xyze(1, 0) = -0.23693669697999861223;
    hex8_xyze(2, 0) = -0.077105263157895148618;
    nids.push_back(217851);
    hex8_xyze(0, 1) = -0.28063083348649442428;
    hex8_xyze(1, 1) = -0.24722931722729699078;
    hex8_xyze(2, 1) = -0.077105263157895148618;
    nids.push_back(217852);
    hex8_xyze(0, 2) = -0.27512826812401414145;
    hex8_xyze(1, 2) = -0.24238168355617351057;
    hex8_xyze(2, 2) = -0.077105263157895148618;
    nids.push_back(217881);
    hex8_xyze(0, 3) = -0.28369947443670645582;
    hex8_xyze(1, 3) = -0.23229087939215550818;
    hex8_xyze(2, 3) = -0.077105263157895148618;
    nids.push_back(217880);
    hex8_xyze(0, 4) = -0.28937346392544061047;
    hex8_xyze(1, 4) = -0.23693669697999861223;
    hex8_xyze(2, 4) = -0.057368421052631630996;
    nids.push_back(206063);
    hex8_xyze(0, 5) = -0.28063083348649442428;
    hex8_xyze(1, 5) = -0.24722931722729699078;
    hex8_xyze(2, 5) = -0.057368421052631630996;
    nids.push_back(206064);
    hex8_xyze(0, 6) = -0.27512826812401414145;
    hex8_xyze(1, 6) = -0.24238168355617351057;
    hex8_xyze(2, 6) = -0.057368421052631630996;
    nids.push_back(206093);
    hex8_xyze(0, 7) = -0.28369947443670645582;
    hex8_xyze(1, 7) = -0.23229087939215550818;
    hex8_xyze(2, 7) = -0.057368421052631630996;
    nids.push_back(206092);

    intersection.add_element(197488, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.28063083348649442428;
    hex8_xyze(1, 0) = -0.24722931722729699078;
    hex8_xyze(2, 0) = -0.077105263157895148618;
    nids.push_back(217852);
    hex8_xyze(0, 1) = -0.27152231397925075118;
    hex8_xyze(1, 1) = -0.25719959761118055264;
    hex8_xyze(2, 1) = -0.077105263157895148618;
    nids.push_back(217853);
    hex8_xyze(0, 2) = -0.26619834703848110902;
    hex8_xyze(1, 2) = -0.25215646824625542743;
    hex8_xyze(2, 2) = -0.077105263157895148618;
    nids.push_back(217882);
    hex8_xyze(0, 3) = -0.27512826812401414145;
    hex8_xyze(1, 3) = -0.24238168355617351057;
    hex8_xyze(2, 3) = -0.077105263157895148618;
    nids.push_back(217881);
    hex8_xyze(0, 4) = -0.28063083348649442428;
    hex8_xyze(1, 4) = -0.24722931722729699078;
    hex8_xyze(2, 4) = -0.057368421052631630996;
    nids.push_back(206064);
    hex8_xyze(0, 5) = -0.27152231397925075118;
    hex8_xyze(1, 5) = -0.25719959761118055264;
    hex8_xyze(2, 5) = -0.057368421052631630996;
    nids.push_back(206065);
    hex8_xyze(0, 6) = -0.26619834703848110902;
    hex8_xyze(1, 6) = -0.25215646824625542743;
    hex8_xyze(2, 6) = -0.057368421052631630996;
    nids.push_back(206094);
    hex8_xyze(0, 7) = -0.27512826812401414145;
    hex8_xyze(1, 7) = -0.24238168355617351057;
    hex8_xyze(2, 7) = -0.057368421052631630996;
    nids.push_back(206093);

    intersection.add_element(197489, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.27152231397925075118;
    hex8_xyze(1, 0) = -0.25719959761118055264;
    hex8_xyze(2, 0) = -0.077105263157895148618;
    nids.push_back(217853);
    hex8_xyze(0, 1) = -0.26205978117542993999;
    hex8_xyze(1, 1) = -0.2668345387881519426;
    hex8_xyze(2, 1) = -0.077105263157895148618;
    nids.push_back(217854);
    hex8_xyze(0, 2) = -0.25692135409355870701;
    hex8_xyze(1, 2) = -0.26160248900799210059;
    hex8_xyze(2, 2) = -0.077105263157895148618;
    nids.push_back(217883);
    hex8_xyze(0, 3) = -0.26619834703848110902;
    hex8_xyze(1, 3) = -0.25215646824625542743;
    hex8_xyze(2, 3) = -0.077105263157895148618;
    nids.push_back(217882);
    hex8_xyze(0, 4) = -0.27152231397925075118;
    hex8_xyze(1, 4) = -0.25719959761118055264;
    hex8_xyze(2, 4) = -0.057368421052631630996;
    nids.push_back(206065);
    hex8_xyze(0, 5) = -0.26205978117542993999;
    hex8_xyze(1, 5) = -0.2668345387881519426;
    hex8_xyze(2, 5) = -0.057368421052631630996;
    nids.push_back(206066);
    hex8_xyze(0, 6) = -0.25692135409355870701;
    hex8_xyze(1, 6) = -0.26160248900799210059;
    hex8_xyze(2, 6) = -0.057368421052631630996;
    nids.push_back(206095);
    hex8_xyze(0, 7) = -0.26619834703848110902;
    hex8_xyze(1, 7) = -0.25215646824625542743;
    hex8_xyze(2, 7) = -0.057368421052631630996;
    nids.push_back(206094);

    intersection.add_element(197490, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.28063083348649436877;
    hex8_xyze(1, 0) = -0.24722931722729701853;
    hex8_xyze(2, 0) = -0.096842105263157687856;
    nids.push_back(229640);
    hex8_xyze(0, 1) = -0.27152231397925069567;
    hex8_xyze(1, 1) = -0.25719959761118060815;
    hex8_xyze(2, 1) = -0.096842105263157687856;
    nids.push_back(229641);
    hex8_xyze(0, 2) = -0.26619834703848105351;
    hex8_xyze(1, 2) = -0.25215646824625548295;
    hex8_xyze(2, 2) = -0.096842105263157687856;
    nids.push_back(229670);
    hex8_xyze(0, 3) = -0.27512826812401408594;
    hex8_xyze(1, 3) = -0.24238168355617353833;
    hex8_xyze(2, 3) = -0.096842105263157687856;
    nids.push_back(229669);
    hex8_xyze(0, 4) = -0.28063083348649442428;
    hex8_xyze(1, 4) = -0.24722931722729699078;
    hex8_xyze(2, 4) = -0.077105263157895148618;
    nids.push_back(217852);
    hex8_xyze(0, 5) = -0.27152231397925075118;
    hex8_xyze(1, 5) = -0.25719959761118055264;
    hex8_xyze(2, 5) = -0.077105263157895148618;
    nids.push_back(217853);
    hex8_xyze(0, 6) = -0.26619834703848110902;
    hex8_xyze(1, 6) = -0.25215646824625542743;
    hex8_xyze(2, 6) = -0.077105263157895148618;
    nids.push_back(217882);
    hex8_xyze(0, 7) = -0.27512826812401414145;
    hex8_xyze(1, 7) = -0.24238168355617351057;
    hex8_xyze(2, 7) = -0.077105263157895148618;
    nids.push_back(217881);

    intersection.add_element(209189, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.27512826812401414145;
    hex8_xyze(1, 0) = -0.24238168355617351057;
    hex8_xyze(2, 0) = -0.077105263157895148618;
    nids.push_back(217881);
    hex8_xyze(0, 1) = -0.26619834703848110902;
    hex8_xyze(1, 1) = -0.25215646824625542743;
    hex8_xyze(2, 1) = -0.077105263157895148618;
    nids.push_back(217882);
    hex8_xyze(0, 2) = -0.26087438009771152236;
    hex8_xyze(1, 2) = -0.24711333888133032999;
    hex8_xyze(2, 2) = -0.077105263157895148618;
    nids.push_back(217911);
    hex8_xyze(0, 3) = -0.26962570276153385862;
    hex8_xyze(1, 3) = -0.23753404988505005813;
    hex8_xyze(2, 3) = -0.077105263157895148618;
    nids.push_back(217910);
    hex8_xyze(0, 4) = -0.27512826812401414145;
    hex8_xyze(1, 4) = -0.24238168355617351057;
    hex8_xyze(2, 4) = -0.057368421052631630996;
    nids.push_back(206093);
    hex8_xyze(0, 5) = -0.26619834703848110902;
    hex8_xyze(1, 5) = -0.25215646824625542743;
    hex8_xyze(2, 5) = -0.057368421052631630996;
    nids.push_back(206094);
    hex8_xyze(0, 6) = -0.26087438009771152236;
    hex8_xyze(1, 6) = -0.24711333888133032999;
    hex8_xyze(2, 6) = -0.057368421052631630996;
    nids.push_back(206123);
    hex8_xyze(0, 7) = -0.26962570276153385862;
    hex8_xyze(1, 7) = -0.23753404988505005813;
    hex8_xyze(2, 7) = -0.057368421052631630996;
    nids.push_back(206122);

    intersection.add_element(197518, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.28613339884897470711;
    hex8_xyze(1, 0) = -0.25207695089842047098;
    hex8_xyze(2, 0) = -0.077105263157895148618;
    nids.push_back(217823);
    hex8_xyze(0, 1) = -0.27684628092002039335;
    hex8_xyze(1, 1) = -0.26224272697610567784;
    hex8_xyze(2, 1) = -0.077105263157895148618;
    nids.push_back(217824);
    hex8_xyze(0, 2) = -0.27152231397925075118;
    hex8_xyze(1, 2) = -0.25719959761118055264;
    hex8_xyze(2, 2) = -0.077105263157895148618;
    nids.push_back(217853);
    hex8_xyze(0, 3) = -0.28063083348649442428;
    hex8_xyze(1, 3) = -0.24722931722729699078;
    hex8_xyze(2, 3) = -0.077105263157895148618;
    nids.push_back(217852);
    hex8_xyze(0, 4) = -0.28613339884897470711;
    hex8_xyze(1, 4) = -0.25207695089842047098;
    hex8_xyze(2, 4) = -0.057368421052631630996;
    nids.push_back(206035);
    hex8_xyze(0, 5) = -0.27684628092002039335;
    hex8_xyze(1, 5) = -0.26224272697610567784;
    hex8_xyze(2, 5) = -0.057368421052631630996;
    nids.push_back(206036);
    hex8_xyze(0, 6) = -0.27152231397925075118;
    hex8_xyze(1, 6) = -0.25719959761118055264;
    hex8_xyze(2, 6) = -0.057368421052631630996;
    nids.push_back(206065);
    hex8_xyze(0, 7) = -0.28063083348649442428;
    hex8_xyze(1, 7) = -0.24722931722729699078;
    hex8_xyze(2, 7) = -0.057368421052631630996;
    nids.push_back(206064);

    intersection.add_element(197460, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  {
    Core::LinAlg::SerialDenseMatrix hex8_xyze(3, 8);

    nids.clear();
    hex8_xyze(0, 0) = -0.28063083348649442428;
    hex8_xyze(1, 0) = -0.24722931722729699078;
    hex8_xyze(2, 0) = -0.057368421052631630996;
    nids.push_back(206064);
    hex8_xyze(0, 1) = -0.27152231397925075118;
    hex8_xyze(1, 1) = -0.25719959761118055264;
    hex8_xyze(2, 1) = -0.057368421052631630996;
    nids.push_back(206065);
    hex8_xyze(0, 2) = -0.26619834703848110902;
    hex8_xyze(1, 2) = -0.25215646824625542743;
    hex8_xyze(2, 2) = -0.057368421052631630996;
    nids.push_back(206094);
    hex8_xyze(0, 3) = -0.27512826812401414145;
    hex8_xyze(1, 3) = -0.24238168355617351057;
    hex8_xyze(2, 3) = -0.057368421052631630996;
    nids.push_back(206093);
    hex8_xyze(0, 4) = -0.28063083348649436877;
    hex8_xyze(1, 4) = -0.24722931722729690751;
    hex8_xyze(2, 4) = -0.037631578947368654608;
    nids.push_back(194276);
    hex8_xyze(0, 5) = -0.27152231397925069567;
    hex8_xyze(1, 5) = -0.25719959761118049713;
    hex8_xyze(2, 5) = -0.037631578947368654608;
    nids.push_back(194277);
    hex8_xyze(0, 6) = -0.26619834703848105351;
    hex8_xyze(1, 6) = -0.25215646824625537192;
    hex8_xyze(2, 6) = -0.037631578947368654608;
    nids.push_back(194306);
    hex8_xyze(0, 7) = -0.27512826812401408594;
    hex8_xyze(1, 7) = -0.24238168355617342731;
    hex8_xyze(2, 7) = -0.037631578947368654608;
    nids.push_back(194305);

    intersection.add_element(185789, nids, hex8_xyze, Core::FE::CellType::hex8);
  }

  intersection.cut_test_cut(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation);
  intersection.cut_finalize(
      true, Cut::VCellGaussPts_DirectDivergence, Cut::BCellGaussPts_Tessellation, false, true);
}
