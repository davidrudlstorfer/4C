// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_STRUCTURE_RESULTTEST_HPP
#define FOUR_C_STRUCTURE_RESULTTEST_HPP

#include "4C_config.hpp"

#include "4C_linalg_vector.hpp"
#include "4C_utils_result_test.hpp"

FOUR_C_NAMESPACE_OPEN

// forward declarations
namespace Core::FE
{
  class Discretization;
}  // namespace Core::FE

namespace Solid
{
  class TimInt;
}

/*!
  \brief Structure specific result test class
*/
class StruResultTest : public Core::Utils::ResultTest
{
 public:
  //! Constructor for time integrators of general kind
  StruResultTest(Solid::TimInt& tintegrator);

  /*!
  \brief Test nodal values in solid/structure field and time integration

  Possible position flags are
  - displacements "dispx", "dispy", "dispz"
  - velocitites "velx", "vely", "velz",
  - accelerations "accx", "accy", "accz"
  */
  void test_node(
      const Core::IO::InputParameterContainer& container, int& nerr, int& test_count) final;

  /*!
  \brief Test special quantities of structure discretization / time integration

  See get_special_result_for_testing() for a list and extraction mechanisms of special results to be
  tested.
  */
  void test_special(
      const Core::IO::InputParameterContainer& container, int& nerr, int& test_count) final;

 private:
  //! Extract specified quantity from the structure field
  double get_special_result_for_testing(const std::string& quantity);

  //! Structure time integrator
  std::shared_ptr<Solid::TimInt> timeintegrator_;

  //! Structure discretisation
  std::shared_ptr<Core::FE::Discretization> strudisc_;

  //! @name Solution
  //!@{

  //! global displacement DOFs
  std::shared_ptr<const Core::LinAlg::Vector<double>> dis_;

  //! global velocity DOFs
  std::shared_ptr<const Core::LinAlg::Vector<double>> vel_;

  //! global acceleration DOFs
  std::shared_ptr<const Core::LinAlg::Vector<double>> acc_;

  //!@}
};

FOUR_C_NAMESPACE_CLOSE

#endif
