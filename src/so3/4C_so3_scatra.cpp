/*----------------------------------------------------------------------*/
/*! \file

\brief Solid-scatra elements base class

\level 2


*----------------------------------------------------------------------*/


#include "4C_so3_scatra.hpp"

#include "4C_global_data.hpp"
#include "4C_inpar_ssi.hpp"
#include "4C_io_linedefinition.hpp"
#include "4C_lib_element_integration_select.hpp"

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*
 |  ctor (public)                                            vuong 03/12|
 |  id             (in)  this element's global id                       |
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
DRT::ELEMENTS::So3Scatra<so3_ele, distype>::So3Scatra(int id, int owner)
    : so3_ele(id, owner),
      impltype_(INPAR::SCATRA::impltype_undefined),
      intpoints_(distype == CORE::FE::CellType::tet4
                     ? CORE::FE::GaussRule3D::tet_1point
                     : (distype == CORE::FE::CellType::tet10
                               ? CORE::FE::GaussRule3D::tet_4point
                               : DRT::ELEMENTS::DisTypeToOptGaussRule<distype>::rule)),
      numgpt_(intpoints_.nquad),
      xsi_(0.0),
      inv_j_(0.0),
      det_j_(0.0)
{
  return;
}


/*----------------------------------------------------------------------*
 |  copy-ctor (public)                                       vuong 03/12|
 |  id             (in)  this element's global id                       |
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
DRT::ELEMENTS::So3Scatra<so3_ele, distype>::So3Scatra(
    const DRT::ELEMENTS::So3Scatra<so3_ele, distype>& old)
    : so3_ele(old),
      impltype_(old.impltype_),
      intpoints_(old.intpoints_),
      numgpt_(old.numgpt_),
      xsi_(old.xsi_),
      inv_j_(old.inv_j_),
      det_j_(old.det_j_)
{
  return;
}

/*----------------------------------------------------------------------*
 |  Deep copy this instance of Solid3 and return pointer to it (public) |
 |                                                           vuong 03/12|
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
DRT::Element* DRT::ELEMENTS::So3Scatra<so3_ele, distype>::Clone() const
{
  auto* newelement = new DRT::ELEMENTS::So3Scatra<so3_ele, distype>(*this);
  return newelement;
}

/*----------------------------------------------------------------------*
 |  Pack data                                                  (public) |
 |                                                           vuong 03/12|
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
void DRT::ELEMENTS::So3Scatra<so3_ele, distype>::Pack(CORE::COMM::PackBuffer& data) const
{
  CORE::COMM::PackBuffer::SizeMarker sm(data);
  sm.Insert();

  // pack type of this instance of ParObject
  int type = UniqueParObjectId();
  so3_ele::AddtoPack(data, type);

  // pack scalar transport impltype
  so3_ele::AddtoPack(data, impltype_);

  // detJ_
  so3_ele::AddtoPack(data, det_j_);

  // invJ_
  auto size = (int)inv_j_.size();
  so3_ele::AddtoPack(data, size);
  for (int i = 0; i < size; ++i) so3_ele::AddtoPack(data, inv_j_[i]);

  // xsi_
  size = (int)xsi_.size();
  so3_ele::AddtoPack(data, size);
  for (int i = 0; i < size; ++i) so3_ele::AddtoPack(data, xsi_[i]);


  // add base class Element
  so3_ele::Pack(data);
}

/*----------------------------------------------------------------------*
 |  Unpack data                                                (public) |
 |                                                           vuong 03/12|
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
void DRT::ELEMENTS::So3Scatra<so3_ele, distype>::Unpack(const std::vector<char>& data)
{
  std::vector<char>::size_type position = 0;

  CORE::COMM::ExtractAndAssertId(position, data, UniqueParObjectId());

  // extract scalar transport impltype_
  impltype_ = static_cast<INPAR::SCATRA::ImplType>(so3_ele::ExtractInt(position, data));

  // detJ_
  so3_ele::ExtractfromPack(position, data, det_j_);

  // invJ_
  int size = 0;
  so3_ele::ExtractfromPack(position, data, size);
  inv_j_.resize(size, CORE::LINALG::Matrix<numdim_, numdim_>(true));
  for (int i = 0; i < size; ++i) so3_ele::ExtractfromPack(position, data, inv_j_[i]);

  // xsi_
  size = 0;
  so3_ele::ExtractfromPack(position, data, size);
  xsi_.resize(size, CORE::LINALG::Matrix<numdim_, 1>(true));
  for (int i = 0; i < size; ++i) so3_ele::ExtractfromPack(position, data, xsi_[i]);

  // extract base class Element
  std::vector<char> basedata(0);
  so3_ele::ExtractfromPack(position, data, basedata);

  so3_ele::Unpack(basedata);

  if (position != data.size())
    FOUR_C_THROW("Mismatch in size of data %d <-> %d", (int)data.size(), position);
}

/*----------------------------------------------------------------------*
 |  print this element (public)                              vuong 03/12|
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
void DRT::ELEMENTS::So3Scatra<so3_ele, distype>::Print(std::ostream& os) const
{
  os << "So3_scatra ";
  os << " discretization type: " << CORE::FE::CellTypeToString(distype).c_str();
  Element::Print(os);
  return;
}

/*----------------------------------------------------------------------*
 |  read this element (public)                             schmidt 09/17|
 *----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
bool DRT::ELEMENTS::So3Scatra<so3_ele, distype>::ReadElement(
    const std::string& eletype, const std::string& eledistype, INPUT::LineDefinition* linedef)
{
  so3_ele::ReadElement(eletype, eledistype, linedef);

  // read scalar transport implementation type
  std::string impltype;
  linedef->ExtractString("TYPE", impltype);

  if (impltype == "Undefined")
    impltype_ = INPAR::SCATRA::impltype_undefined;
  else if (impltype == "AdvReac")
    impltype_ = INPAR::SCATRA::impltype_advreac;
  else if (impltype == "CardMono")
    impltype_ = INPAR::SCATRA::impltype_cardiac_monodomain;
  else if (impltype == "Chemo")
    impltype_ = INPAR::SCATRA::impltype_chemo;
  else if (impltype == "ChemoReac")
    impltype_ = INPAR::SCATRA::impltype_chemoreac;
  else if (impltype == "ElchDiffCond")
    impltype_ = INPAR::SCATRA::impltype_elch_diffcond;
  else if (impltype == "ElchElectrode")
    impltype_ = INPAR::SCATRA::impltype_elch_electrode;
  else if (impltype == "Loma")
    impltype_ = INPAR::SCATRA::impltype_loma;
  else if (impltype == "RefConcReac")
    impltype_ = INPAR::SCATRA::impltype_refconcreac;
  else if (impltype == "Std")
    impltype_ = INPAR::SCATRA::impltype_std;
  else
    FOUR_C_THROW("Invalid implementation type for So3_Scatra elements!");

  return true;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
inline DRT::Node** DRT::ELEMENTS::So3Scatra<so3_ele, distype>::Nodes()
{
  return so3_ele::Nodes();
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
inline Teuchos::RCP<CORE::MAT::Material> DRT::ELEMENTS::So3Scatra<so3_ele, distype>::material()
    const
{
  return so3_ele::Material();
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
template <class so3_ele, CORE::FE::CellType distype>
inline int DRT::ELEMENTS::So3Scatra<so3_ele, distype>::id() const
{
  return so3_ele::Id();
}


/*--------------------------------------------------------------------------*
 | set the material  (public)                                 schmidt 10/17 |
 *                                                                          */
template <class so3_ele, CORE::FE::CellType distype>
void DRT::ELEMENTS::So3Scatra<so3_ele, distype>::SetMaterial(
    int matnum, Teuchos::RCP<CORE::MAT::Material> mat)
{
  // call base class
  so3_ele::SetMaterial(0, mat);

  // get the scatra structure control parameter list
  const Teuchos::ParameterList& ssicontrol = GLOBAL::Problem::Instance()->SSIControlParams();

  if ((Teuchos::getIntegralValue<INPAR::SSI::SolutionSchemeOverFields>(ssicontrol, "COUPALGO") ==
          INPAR::SSI::SolutionSchemeOverFields::ssi_Monolithic) and
      (mat->MaterialType() != CORE::Materials::m_multiplicative_split_defgrad_elasthyper))
    FOUR_C_THROW(
        "When you use the 'COUPALGO' 'ssi_Monolithic' from the 'SSI CONTROL' section,"
        " you need to use the material 'MAT_MultiplicativeSplitDefgradElastHyper'! If you want to "
        "use another material feel free to implement it! ;-)");

  // call base class
  so3_ele::SetMaterial(0, mat);

  return;
}


template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoHex8, CORE::FE::CellType::hex8>;
template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoHex8fbar, CORE::FE::CellType::hex8>;
template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoHex27, CORE::FE::CellType::hex27>;
template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoTet4, CORE::FE::CellType::tet4>;
template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoTet10, CORE::FE::CellType::tet10>;
template class DRT::ELEMENTS::So3Scatra<DRT::ELEMENTS::SoWeg6, CORE::FE::CellType::wedge6>;

FOUR_C_NAMESPACE_CLOSE