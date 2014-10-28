/*----------------------------------------------------------------------*/
/*!
 \file fluid_ele_xwall_boundary.cpp

 \brief

<pre>
Maintainer: Benjamin Krank
            krank@lnm.mw.tum.de
            http://www.lnm.mw.tum.de
            089 - 289-15252

 </pre>
 *----------------------------------------------------------------------*/


#include "fluid_ele_xwall.H"

#include "../drt_lib/drt_discret.H"
#include "../drt_lib/drt_utils_factory.H"
#include "../drt_lib/drt_utils_nullspace.H"
#include "../drt_lib/drt_linedefinition.H"
#include "../drt_lib/drt_globalproblem.H"

#include "../drt_inpar/inpar_fluid.H"


DRT::ELEMENTS::FluidXWallType DRT::ELEMENTS::FluidXWallType::instance_;

DRT::ParObject* DRT::ELEMENTS::FluidXWallType::Create( const std::vector<char> & data )
{
  DRT::ELEMENTS::FluidXWall* object = new DRT::ELEMENTS::FluidXWall(-1,-1);
  object->Unpack(data);
  return object;
}

Teuchos::RCP<DRT::Element> DRT::ELEMENTS::FluidXWallType::Create(const std::string  eletype,
                                                             const std::string  eledistype,
                                                             const int     id,
                                                             const int     owner)
{

  if ( eletype=="FLUID3XW" )
  {
    return Teuchos::rcp(new DRT::ELEMENTS::FluidXWall(id,owner));
  }
  return Teuchos::null;
}

Teuchos::RCP<DRT::Element> DRT::ELEMENTS::FluidXWallType::Create( const int id, const int owner )
{
  return Teuchos::rcp(new DRT::ELEMENTS::FluidXWall(id,owner));
}

void DRT::ELEMENTS::FluidXWallType::NodalBlockInformation( Element * dwele, int & numdf, int & dimns, int & nv, int & np )
{
  //this is necessary here! Otherwise it would not be consistent with the non-enriched nodes
  //since we are assuming that all elements are equal during nullspace computation
  numdf = 4;
  dimns = numdf;
  nv = numdf-1;
  np = 1;
}

void DRT::ELEMENTS::FluidXWallType::ComputeNullSpace( DRT::Discretization & dis, std::vector<double> & ns, const double * x0, int numdf, int dimns )
{
  DRT::UTILS::ComputeFluidDNullSpace( dis, ns, x0, numdf, dimns );
}

void DRT::ELEMENTS::FluidXWallType::SetupElementDefinition( std::map<std::string,std::map<std::string,DRT::INPUT::LineDefinition> > & definitions )
{
  std::map<std::string,DRT::INPUT::LineDefinition>& defsxwall = definitions["FLUID3XW"];

  defsxwall["HEX8"]
    .AddIntVector("HEX8",8)
    .AddNamedInt("MAT")
    .AddNamedString("NA")
    ;
}


/*----------------------------------------------------------------------*
 |  ctor (public)                                            gammi 02/08|
 |  id             (in)  this element's global id                       |
 *----------------------------------------------------------------------*/
DRT::ELEMENTS::FluidXWall::FluidXWall(int id, int owner) :
Fluid(id,owner)
{
  return;
}

/*----------------------------------------------------------------------*
 |  copy-ctor (public)                                       gammi 02/08|
 *----------------------------------------------------------------------*/
DRT::ELEMENTS::FluidXWall::FluidXWall(const DRT::ELEMENTS::FluidXWall& old) :
Fluid(old)
{
  return;
}

/*----------------------------------------------------------------------*
 |  Deep copy this instance of Fluid and return pointer to it (public) |
 |                                                          gammi 02/08 |
 *----------------------------------------------------------------------*/
DRT::Element* DRT::ELEMENTS::FluidXWall::Clone() const
{
  DRT::ELEMENTS::FluidXWall* newelement = new DRT::ELEMENTS::FluidXWall(*this);
  return newelement;
}

/*----------------------------------------------------------------------*
 |  dtor (public)                                            gammi 02/08|
 *----------------------------------------------------------------------*/
DRT::ELEMENTS::FluidXWall::~FluidXWall()
{
  return;
}

/*----------------------------------------------------------------------*
 |  Pack data                                                  (public) |
 |                                                                      |
 *----------------------------------------------------------------------*/
void DRT::ELEMENTS::FluidXWall::Pack(DRT::PackBuffer& data) const
{
  DRT::PackBuffer::SizeMarker sm( data );
  sm.Insert();

  // pack type of this instance of ParObject
  int type = UniqueParObjectId();
  AddtoPack(data,type);

  // add base class Element
  Fluid::Pack(data);

  return;
}


/*----------------------------------------------------------------------*
 |  Unpack data                                                (public) |
 |                                                                      |
 *----------------------------------------------------------------------*/
void DRT::ELEMENTS::FluidXWall::Unpack(const std::vector<char>& data)
{
  std::vector<char>::size_type position = 0;
  // extract type
  int type = 0;
  ExtractfromPack(position,data,type);
  dsassert(type == UniqueParObjectId(), "wrong instance type data");

  // extract base class Element
  std::vector<char> basedata(0);
  Fluid::ExtractfromPack(position,data,basedata);
  Fluid::Unpack(basedata);

  if (position != data.size())
    dserror("Mismatch in size of data %d <-> %d",(int)data.size(),position);
  return;
}


/*----------------------------------------------------------------------*
 |  get vector of lines              (public)                           |
 *----------------------------------------------------------------------*/
std::vector<Teuchos::RCP<DRT::Element> > DRT::ELEMENTS::FluidXWall::Lines()
{
  // do NOT store line or surface elements inside the parent element
  // after their creation.
  // Reason: if a Redistribute() is performed on the discretization,
  // stored node ids and node pointers owned by these boundary elements might
  // have become illegal and you will get a nice segmentation fault ;-)

  // so we have to allocate new line elements:

  if (NumLine()>1) // 1D boundary element and 2D/3D parent element
  {
    return DRT::UTILS::ElementBoundaryFactory<FluidXWallBoundary,FluidXWall>(DRT::UTILS::buildLines,this);
  }
  else if (NumLine()==1) // 1D boundary element and 1D parent element -> body load (calculated in evaluate)
  {
    // 1D (we return the element itself)
    std::vector<Teuchos::RCP<Element> > surfaces(1);
    surfaces[0]= Teuchos::rcp(this, false);
    return surfaces;
  }
  else
  {
    dserror("Lines() does not exist for points ");
    return DRT::Element::Surfaces();
  }
}


/*----------------------------------------------------------------------*
 |  get vector of surfaces (public)                                     |
 *----------------------------------------------------------------------*/
std::vector<Teuchos::RCP<DRT::Element> > DRT::ELEMENTS::FluidXWall::Surfaces()
{
  // do NOT store line or surface elements inside the parent element
  // after their creation.
  // Reason: if a Redistribute() is performed on the discretization,
  // stored node ids and node pointers owned by these boundary elements might
  // have become illegal and you will get a nice segmentation fault ;-)

  // so we have to allocate new line elements:

  if (NumSurface() > 1)   // 2D boundary element and 3D parent element
    return DRT::UTILS::ElementBoundaryFactory<FluidXWallBoundary,FluidXWall>(DRT::UTILS::buildSurfaces,this);
  else if (NumSurface() == 1) // 2D boundary element and 2D parent element -> body load (calculated in evaluate)
  {
    // 2D (we return the element itself)
    std::vector<Teuchos::RCP<Element> > surfaces(1);
    surfaces[0]= Teuchos::rcp(this, false);
    return surfaces;
  }
  else  // 1D elements
  {
    dserror("Surfaces() does not exist for 1D-element ");
    return DRT::Element::Surfaces();
  }
}


/*----------------------------------------------------------------------*
 |  get vector of volumes (length 1) (public)                            |
 *----------------------------------------------------------------------*/
std::vector<Teuchos::RCP<DRT::Element> > DRT::ELEMENTS::FluidXWall::Volumes()
{
  if (NumVolume()==1) // 3D boundary element and a 3D parent element -> body load (calculated in evaluate)
  {
    std::vector<Teuchos::RCP<Element> > volumes(1);
    volumes[0]= Teuchos::rcp(this, false);
    return volumes;
  }
  else //
  {
    dserror("Volumes() does not exist for 1D/2D-elements");
    return DRT::Element::Surfaces();
  }
}

/*----------------------------------------------------------------------*
 |  print this element (public)                                         |
 *----------------------------------------------------------------------*/
void DRT::ELEMENTS::FluidXWall::Print(std::ostream& os) const
{
  os << "FluidXWall ";
  Element::Print(os);
  return;
}
