// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_w1_poro_scatra_eletypes.hpp"

#include "4C_io_linedefinition.hpp"
#include "4C_w1_poro_scatra.hpp"

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*
 |  QUAD 4 Element                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/

Discret::Elements::WallQuad4PoroScatraType Discret::Elements::WallQuad4PoroScatraType::instance_;

Discret::Elements::WallQuad4PoroScatraType& Discret::Elements::WallQuad4PoroScatraType::instance()
{
  return instance_;
}

Core::Communication::ParObject* Discret::Elements::WallQuad4PoroScatraType::create(
    Core::Communication::UnpackBuffer& buffer)
{
  Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad4>* object =
      new Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad4>(-1, -1);
  object->unpack(buffer);
  return object;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallQuad4PoroScatraType::create(
    const std::string eletype, const std::string eledistype, const int id, const int owner)
{
  if (eletype == "WALLQ4POROSCATRA")
  {
    std::shared_ptr<Core::Elements::Element> ele =
        std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad4>>(id, owner);
    return ele;
  }
  return nullptr;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallQuad4PoroScatraType::create(
    const int id, const int owner)
{
  std::shared_ptr<Core::Elements::Element> ele =
      std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad4>>(id, owner);
  return ele;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
void Discret::Elements::WallQuad4PoroScatraType::setup_element_definition(
    std::map<std::string, std::map<std::string, Input::LineDefinition>>& definitions)
{
  std::map<std::string, std::map<std::string, Input::LineDefinition>> definitions_wall;
  WallQuad4PoroType::setup_element_definition(definitions_wall);

  std::map<std::string, Input::LineDefinition>& defs_wall = definitions_wall["WALLQ4PORO"];

  std::map<std::string, Input::LineDefinition>& defs = definitions["WALLQ4POROSCATRA"];

  defs["QUAD4"] =
      Input::LineDefinition::Builder(defs_wall["QUAD4"]).add_named_string("TYPE").build();
}


/*----------------------------------------------------------------------*
 |  QUAD 9 Element                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
Discret::Elements::WallQuad9PoroScatraType Discret::Elements::WallQuad9PoroScatraType::instance_;

Discret::Elements::WallQuad9PoroScatraType& Discret::Elements::WallQuad9PoroScatraType::instance()
{
  return instance_;
}

Core::Communication::ParObject* Discret::Elements::WallQuad9PoroScatraType::create(
    Core::Communication::UnpackBuffer& buffer)
{
  Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad9>* object =
      new Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad9>(-1, -1);
  object->unpack(buffer);
  return object;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallQuad9PoroScatraType::create(
    const std::string eletype, const std::string eledistype, const int id, const int owner)
{
  if (eletype == "WALLQ9POROSCATRA")
  {
    std::shared_ptr<Core::Elements::Element> ele =
        std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad9>>(id, owner);
    return ele;
  }
  return nullptr;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallQuad9PoroScatraType::create(
    const int id, const int owner)
{
  std::shared_ptr<Core::Elements::Element> ele =
      std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::quad9>>(id, owner);
  return ele;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
void Discret::Elements::WallQuad9PoroScatraType::setup_element_definition(
    std::map<std::string, std::map<std::string, Input::LineDefinition>>& definitions)
{
  std::map<std::string, std::map<std::string, Input::LineDefinition>> definitions_wall;
  WallQuad9PoroType::setup_element_definition(definitions_wall);

  std::map<std::string, Input::LineDefinition>& defs_wall = definitions_wall["WALLQ9PORO"];

  std::map<std::string, Input::LineDefinition>& defs = definitions["WALLQ9POROSCATRA"];

  defs["QUAD9"] =
      Input::LineDefinition::Builder(defs_wall["QUAD9"]).add_named_string("TYPE").build();
}

/*----------------------------------------------------------------------*
 |  NURBS 4 Element                                       schmidt 09/17 |
 *----------------------------------------------------------------------*/

Discret::Elements::WallNurbs4PoroScatraType Discret::Elements::WallNurbs4PoroScatraType::instance_;

Discret::Elements::WallNurbs4PoroScatraType& Discret::Elements::WallNurbs4PoroScatraType::instance()
{
  return instance_;
}

Core::Communication::ParObject* Discret::Elements::WallNurbs4PoroScatraType::create(
    Core::Communication::UnpackBuffer& buffer)
{
  Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs4>* object =
      new Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs4>(-1, -1);
  object->unpack(buffer);
  return object;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallNurbs4PoroScatraType::create(
    const std::string eletype, const std::string eledistype, const int id, const int owner)
{
  if (eletype == "WALLN4POROSCATRA")
  {
    std::shared_ptr<Core::Elements::Element> ele =
        std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs4>>(id, owner);
    return ele;
  }
  return nullptr;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallNurbs4PoroScatraType::create(
    const int id, const int owner)
{
  std::shared_ptr<Core::Elements::Element> ele =
      std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs4>>(id, owner);
  return ele;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
void Discret::Elements::WallNurbs4PoroScatraType::setup_element_definition(
    std::map<std::string, std::map<std::string, Input::LineDefinition>>& definitions)
{
  std::map<std::string, std::map<std::string, Input::LineDefinition>> definitions_wall;
  WallNurbs4PoroType::setup_element_definition(definitions_wall);

  std::map<std::string, Input::LineDefinition>& defs_wall = definitions_wall["WALLN4PORO"];

  std::map<std::string, Input::LineDefinition>& defs = definitions["WALLN4POROSCATRA"];

  defs["NURBS4"] =
      Input::LineDefinition::Builder(defs_wall["NURBS4"]).add_named_string("TYPE").build();
}

/*----------------------------------------------------------------------*
 |  NURBS 9 Element                                       schmidt 09/17 |
 *----------------------------------------------------------------------*/

Discret::Elements::WallNurbs9PoroScatraType Discret::Elements::WallNurbs9PoroScatraType::instance_;

Discret::Elements::WallNurbs9PoroScatraType& Discret::Elements::WallNurbs9PoroScatraType::instance()
{
  return instance_;
}

Core::Communication::ParObject* Discret::Elements::WallNurbs9PoroScatraType::create(
    Core::Communication::UnpackBuffer& buffer)
{
  Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs9>* object =
      new Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs9>(-1, -1);
  object->unpack(buffer);
  return object;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallNurbs9PoroScatraType::create(
    const std::string eletype, const std::string eledistype, const int id, const int owner)
{
  if (eletype == "WALLN9POROSCATRA")
  {
    std::shared_ptr<Core::Elements::Element> ele =
        std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs9>>(id, owner);
    return ele;
  }
  return nullptr;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallNurbs9PoroScatraType::create(
    const int id, const int owner)
{
  std::shared_ptr<Core::Elements::Element> ele =
      std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::nurbs9>>(id, owner);
  return ele;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
void Discret::Elements::WallNurbs9PoroScatraType::setup_element_definition(
    std::map<std::string, std::map<std::string, Input::LineDefinition>>& definitions)
{
  std::map<std::string, std::map<std::string, Input::LineDefinition>> definitions_wall;
  WallNurbs9PoroType::setup_element_definition(definitions_wall);

  std::map<std::string, Input::LineDefinition>& defs_wall = definitions_wall["WALLN9PORO"];

  std::map<std::string, Input::LineDefinition>& defs = definitions["WALLN9POROSCATRA"];

  defs["NURBS9"] =
      Input::LineDefinition::Builder(defs_wall["NURBS9"]).add_named_string("TYPE").build();
}

/*----------------------------------------------------------------------*
 |  TRI 3 Element                                         schmidt 09/17 |
 *----------------------------------------------------------------------*/

Discret::Elements::WallTri3PoroScatraType Discret::Elements::WallTri3PoroScatraType::instance_;

Discret::Elements::WallTri3PoroScatraType& Discret::Elements::WallTri3PoroScatraType::instance()
{
  return instance_;
}

Core::Communication::ParObject* Discret::Elements::WallTri3PoroScatraType::create(
    Core::Communication::UnpackBuffer& buffer)
{
  Discret::Elements::Wall1PoroScatra<Core::FE::CellType::tri3>* object =
      new Discret::Elements::Wall1PoroScatra<Core::FE::CellType::tri3>(-1, -1);
  object->unpack(buffer);
  return object;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallTri3PoroScatraType::create(
    const std::string eletype, const std::string eledistype, const int id, const int owner)
{
  if (eletype == "WALLT3POROSCATRA")
  {
    std::shared_ptr<Core::Elements::Element> ele =
        std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::tri3>>(id, owner);
    return ele;
  }
  return nullptr;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
std::shared_ptr<Core::Elements::Element> Discret::Elements::WallTri3PoroScatraType::create(
    const int id, const int owner)
{
  std::shared_ptr<Core::Elements::Element> ele =
      std::make_shared<Discret::Elements::Wall1PoroScatra<Core::FE::CellType::tri3>>(id, owner);
  return ele;
}

/*----------------------------------------------------------------------*
 |                                                        schmidt 09/17 |
 *----------------------------------------------------------------------*/
void Discret::Elements::WallTri3PoroScatraType::setup_element_definition(
    std::map<std::string, std::map<std::string, Input::LineDefinition>>& definitions)
{
  std::map<std::string, std::map<std::string, Input::LineDefinition>> definitions_wall;
  WallTri3PoroType::setup_element_definition(definitions_wall);

  std::map<std::string, Input::LineDefinition>& defs_wall = definitions_wall["WALLT3PORO"];

  std::map<std::string, Input::LineDefinition>& defs = definitions["WALLT3POROSCATRA"];

  defs["TRI3"] = Input::LineDefinition::Builder(defs_wall["TRI3"]).add_named_string("TYPE").build();
}

FOUR_C_NAMESPACE_CLOSE
