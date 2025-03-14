// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_ADAPTER_SCATRA_BASE_ALGORITHM_HPP
#define FOUR_C_ADAPTER_SCATRA_BASE_ALGORITHM_HPP

#include "4C_config.hpp"

#include "4C_utils_parameter_list.fwd.hpp"
#include "4C_utils_result_test.hpp"

#include <memory>

FOUR_C_NAMESPACE_OPEN

// forward declarations
namespace Core::FE
{
  class Discretization;
}  // namespace Core::FE

namespace ScaTra
{
  class ScaTraTimIntImpl;
}

namespace Core::LinAlg
{
  class Solver;
}

namespace Adapter
{
  /// general scalar transport field interface for multiphysics problems
  /*!

  */

  /// basic scalar transport solver
  class ScaTraBaseAlgorithm
  {
   public:
    /// constructor
    ScaTraBaseAlgorithm(
        const Teuchos::ParameterList& prbdyn,  ///< parameter list for global problem
        const Teuchos::ParameterList&
            scatradyn,  ///< parameter list for scalar transport subproblem
        const Teuchos::ParameterList& solverparams,  ///< parameter list for scalar transport solver
        const std::string& disname = "scatra",       ///< name of scalar transport discretization
        const bool isale = false                     ///< ALE flag
    );

    /// virtual destructor to support polymorph destruction
    virtual ~ScaTraBaseAlgorithm() = default;

    /// initialize this class
    virtual void init();

    /// setup this class
    virtual void setup();

    /// access to the scalar transport field solver
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_field() { return scatra_; }

    /// create result test for scalar transport field
    std::shared_ptr<Core::Utils::ResultTest> create_scatra_field_test();

   private:
    /// scalar transport field solver
    std::shared_ptr<ScaTra::ScaTraTimIntImpl> scatra_;

   private:
    //! flag indicating if class is setup
    bool issetup_;

    //! flag indicating if class is initialized
    bool isinit_;

   protected:
    //! returns true if setup() was called and is still valid
    bool is_setup() const { return issetup_; };

    //! returns true if init(..) was called and is still valid
    bool is_init() const { return isinit_; };

    //! check if \ref setup() was called
    void check_is_setup() const;

    //! check if \ref init() was called
    void check_is_init() const;

   private:
    //! set flag true after setup or false if setup became invalid
    void set_is_setup(bool trueorfalse) { issetup_ = trueorfalse; };

    //! set flag true after init or false if init became invalid
    void set_is_init(bool trueorfalse) { isinit_ = trueorfalse; };

  };  // class ScaTraBaseAlgorithm

}  // namespace Adapter

FOUR_C_NAMESPACE_CLOSE

#endif
