// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_scatra_ele_boundary_calc_std.hpp"

#include "4C_fem_general_utils_boundary_integration.hpp"
#include "4C_fem_general_utils_fem_shapefunctions.hpp"
#include "4C_fem_general_utils_nurbs_shapefunctions.hpp"
#include "4C_fem_geometry_position_array.hpp"
#include "4C_fem_nurbs_discretization.hpp"
#include "4C_fem_nurbs_discretization_utils.hpp"
#include "4C_global_data.hpp"  // for curves and functions
#include "4C_scatra_ele.hpp"
#include "4C_scatra_ele_action.hpp"
#include "4C_scatra_ele_parameter_elch.hpp"
#include "4C_scatra_ele_parameter_std.hpp"
#include "4C_utils_singleton_owner.hpp"

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
template <Core::FE::CellType distype, int probdim>
Discret::Elements::ScaTraEleBoundaryCalcStd<distype, probdim>*
Discret::Elements::ScaTraEleBoundaryCalcStd<distype, probdim>::instance(
    const int numdofpernode, const int numscal, const std::string& disname)
{
  static auto singleton_map = Core::Utils::make_singleton_map<std::string>(
      [](const int numdofpernode, const int numscal, const std::string& disname)
      {
        return std::unique_ptr<ScaTraEleBoundaryCalcStd<distype, probdim>>(
            new ScaTraEleBoundaryCalcStd<distype, probdim>(numdofpernode, numscal, disname));
      });

  return singleton_map[disname].instance(
      Core::Utils::SingletonAction::create, numdofpernode, numscal, disname);
}


/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
template <Core::FE::CellType distype, int probdim>
Discret::Elements::ScaTraEleBoundaryCalcStd<distype, probdim>::ScaTraEleBoundaryCalcStd(
    const int numdofpernode, const int numscal, const std::string& disname)
    : my::ScaTraEleBoundaryCalc(numdofpernode, numscal, disname)
{
  return;
}


/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
// template classes
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::quad4, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::quad8, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::quad9, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::tri3, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::tri6, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::line2, 2>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::line2, 3>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::line3, 2>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::nurbs3, 2>;
template class Discret::Elements::ScaTraEleBoundaryCalcStd<Core::FE::CellType::nurbs9, 3>;

FOUR_C_NAMESPACE_CLOSE
