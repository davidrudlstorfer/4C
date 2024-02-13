/*----------------------------------------------------------------------*/
/*! \file
\brief Input parameters for cut library

\level 2


*/

/*----------------------------------------------------------------------*/
#ifndef BACI_INPAR_CUT_HPP
#define BACI_INPAR_CUT_HPP

#include "baci_config.hpp"

#include "baci_inpar_parameterlist_utils.hpp"

BACI_NAMESPACE_OPEN

/*----------------------------------------------------------------------*/
namespace INPAR
{
  namespace CUT
  {
    enum VCellGaussPts
    {
      VCellGaussPts_Tessellation,
      VCellGaussPts_MomentFitting,
      VCellGaussPts_DirectDivergence
    };

    enum BCellGaussPts
    {
      BCellGaussPts_Tessellation,
      BCellGaussPts_MomentFitting,
      BCellGaussPts_DirectDivergence
    };

    enum ElementIntegrationType
    {
      EleIntType_Undecided,        ///< for this element, we still have to set the integration type
      EleIntType_StandardUncut,    ///< classical Gauss rule from the underlying element (if element
                                   ///< is uncut)
      EleIntType_Tessellation,     ///< element uses Tessellation for integration of the element's
                                   ///< volume-cells
      EleIntType_MomentFitting,    ///< element uses MomentFitting for integration of the element's
                                   ///< volume-cells
      EleIntType_DirectDivergence  ///< element uses DirectDivergence for integration of the
                                   ///< element's volume-cells
    };

    enum NodalDofSetStrategy
    {
      NDS_Strategy_OneDofset_PerNodeAndPosition,  ///< suppress multiple dofset per node and allow
                                                  ///< just one dofset per node and per dofset
                                                  ///< position (connect std and ghost sets)
      NDS_Strategy_ConnectGhostDofsets_PerNodeAndPosition,  ///< suppress multiple ghost dofsets per
                                                            ///< node and allow just one dofset per
                                                            ///< node and per dofset position
                                                            ///< (connect only ghost sets, one std
                                                            ///< and one ghost dofsets possible)
      NDS_Strategy_full  ///< full strategy based on found connections of volume-cells in support of
                         ///< the nodal shape function
    };

    enum BoundaryCellPosition
    {
      bcells_on_cut_side,   ///< create boundary cells only on the cut side of the volume cell (
                            ///< default )
      bcells_on_all_sides,  ///< create boundary cells on all sides of the volume cell
      bcells_none           ///< create no boundary cells
    };

    // Cut_Floattype
    enum CUT_Floattype
    {
      floattype_double,  ///< use double type for geometric intersection
      floattype_cln,     ///< use cln type (arbitrary precision) for geometric intersection
      floattype_none
    };

    //! Specifiy which Referenceplanes are used in DirectDivergence
    enum CUT_DirectDivergence_Refplane
    {
      DirDiv_refplane_all,            ///< use all strategies for Referenceplane
      DirDiv_refplane_diagonal_side,  ///< use diagonal and side based Referenceplane
      DirDiv_refplane_facet,          ///< use only facet based Referenceplane
      DirDiv_refplane_diagonal,       ///< use only diagonal based Referenceplane
      DirDiv_refplane_side,           ///< use only side based Referenceplane
      DirDiv_refplane_none            ///< specify no Referenceplane
    };

    /// set the cut parameters
    void SetValidParameters(Teuchos::RCP<Teuchos::ParameterList> list);

  }  // namespace CUT

}  // namespace INPAR

/*----------------------------------------------------------------------*/
BACI_NAMESPACE_CLOSE

#endif  // INPAR_CUT_H