/*---------------------------------------------------------------------------*/
/*! \file
\brief neighbor pair struct for discrete element method (DEM) interactions
\level 3
*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 | definitions                                                               |
 *---------------------------------------------------------------------------*/
#ifndef FOUR_C_PARTICLE_INTERACTION_DEM_NEIGHBOR_PAIR_STRUCT_HPP
#define FOUR_C_PARTICLE_INTERACTION_DEM_NEIGHBOR_PAIR_STRUCT_HPP

#include "4C_config.hpp"

#include "4C_particle_engine_typedefs.hpp"

#include <set>

FOUR_C_NAMESPACE_OPEN

namespace DRT
{
  class Element;
}

/*---------------------------------------------------------------------------*
 | class declarations                                                        |
 *---------------------------------------------------------------------------*/
namespace PARTICLEINTERACTION
{
  //! struct to store quantities of interacting particles
  struct DEMParticlePair final
  {
    //! local index tuple of particles i and j
    PARTICLEENGINE::LocalIndexTuple tuple_i_;
    PARTICLEENGINE::LocalIndexTuple tuple_j_;

    //! gap between particles
    double gap_;

    //! versor from particle i to j
    double e_ji_[3];

    //! effective mass of particles i and j
    double m_eff_;
  };

  //! struct to store quantities of interacting particles and wall elements
  struct DEMParticleWallPair final
  {
    //! local index tuple of particle i
    PARTICLEENGINE::LocalIndexTuple tuple_i_;

    //! pointer to column wall element
    DRT::Element* ele_;

    //! gap between particle and wall contact point
    double gap_;

    //! versor from particle i to wall contact point j
    double e_ji_[3];

    //! parameter space coordinates of wall contact point
    double elecoords_[2];

    //! global ids of relevant wall elements in penetration volume for interaction history
    std::set<int> histeles_;
  };
}  // namespace PARTICLEINTERACTION

/*---------------------------------------------------------------------------*/
FOUR_C_NAMESPACE_CLOSE

#endif