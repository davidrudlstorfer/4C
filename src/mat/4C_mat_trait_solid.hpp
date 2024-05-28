/*! \file
\brief Interface for every material that can evaluate solid material laws

\level 3

*/

#ifndef FOUR_C_MAT_TRAIT_SOLID_HPP
#define FOUR_C_MAT_TRAIT_SOLID_HPP

#include "4C_config.hpp"

FOUR_C_NAMESPACE_OPEN

namespace MAT
{
  namespace TRAIT
  {
    /*!
     * This interface should define all core functionality for solid materials. Currently this is
     * done in So3Material.
     */
    class Solid
    {
     public:
      virtual ~Solid() = default;
    };
  }  // namespace TRAIT
}  // namespace MAT

FOUR_C_NAMESPACE_CLOSE

#endif