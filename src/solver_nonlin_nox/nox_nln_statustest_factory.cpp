/*-----------------------------------------------------------*/
/*!
\file nox_nln_statustest_factory.cpp

\maintainer Michael Hiermeier

\date Jul 31, 2015

\level 3

*/
/*-----------------------------------------------------------*/

#include "nox_nln_statustest_factory.H"   // class definition

#include <NOX_StatusTest_Factory.H>
#include <NOX_Abstract_Vector.H>

#include <Teuchos_ParameterList.hpp>

// different status tests
#include "nox_nln_statustest_normf.H"
#include "nox_nln_statustest_normwrms.H"
#include "nox_nln_statustest_combo.H"


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
NOX::NLN::StatusTest::Factory::Factory() :
noxfactory_(Teuchos::rcp(new const NOX::StatusTest::Factory()))
{
  // empty constructor
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<NOX::StatusTest::Generic> NOX::NLN::StatusTest::Factory::BuildOuterStatusTests(
    Teuchos::ParameterList& p, const NOX::Utils& u,
    std::map<std::string, Teuchos::RCP<NOX::StatusTest::Generic> >* tagged_tests) const
{
  Teuchos::RCP<NOX::StatusTest::Generic> status_test;

  std::string test_type = "???";

  if (Teuchos::isParameterType<std::string>(p, "Test Type"))
    test_type = Teuchos::get<std::string>(p, "Test Type");
  else
  {
    throwError("BuildOuterStatusTests()",
        "The \"Test Type\" is a required parameter!");
  }

  if (test_type == "Combo")
    status_test =  this->BuildComboTest(p, u, tagged_tests);
  else if (test_type == "NormF")
    status_test = this->BuildNormFTest(p, u);
  else if (test_type == "RelativeNormF")
    status_test = this->BuildNormFTest(p,u,true);
  else if (test_type == "NormWRMS")
    status_test = this->BuildNormWRMSTest(p, u);
  else
  {
    status_test = noxfactory_->buildStatusTests(p,u,tagged_tests);
  }

  this->CheckAndTagTest(p, status_test, tagged_tests);

  return status_test;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<NOX::StatusTest::Generic> NOX::NLN::StatusTest::Factory::BuildNormFTest(
    Teuchos::ParameterList& p, const NOX::Utils& u, const bool& relativeNormF) const
{
  // initialized to size zero
  std::vector<double> tolerances = std::vector<double>(0);
  std::vector<NOX::StatusTest::NormF::ToleranceType> toltypes =
      std::vector<NOX::StatusTest::NormF::ToleranceType>(0);
  std::vector<QuantityType> quantitytypes = std::vector<QuantityType>(0);
  std::vector<NOX::Abstract::Vector::NormType> normtypes =
      std::vector<NOX::Abstract::Vector::NormType>(0);
  std::vector<NOX::StatusTest::NormF::ScaleType> scaletypes =
        std::vector<NOX::StatusTest::NormF::ScaleType>(0);


  int i=0;
  std::ostringstream quantity_string;
  quantity_string << "Quantity " << i;
  while (p.isSublist(quantity_string.str()))
  {
    Teuchos::ParameterList& ql = p.sublist(quantity_string.str(), true);

    // ------------------------------------------
    // get the quantity type
    // ------------------------------------------
    std::string quantity_type_string = ql.get("Quantity Type","???");
    QuantityType qType = String2QuantityType(quantity_type_string);
    if (qType == quantity_unknown)
    {
      std::ostringstream msg;
      msg << "The \"Quantity Type\" is a required parameter \n"
          << "and the chosen option \"" << quantity_type_string << "\" is invalid!";
      throwError("BuildNormFTest()",msg.str());
    }
    else
      quantitytypes.push_back(qType);

    // ------------------------------------------
    // get the tolerance type
    // ------------------------------------------
    if (relativeNormF)
      toltypes.push_back(NOX::StatusTest::NormF::Relative);
    else
    {
      std::string tol_type_string = ql.get("Tolerance Type","Absolute");
      if (tol_type_string == "Absolute")
        toltypes.push_back(NOX::StatusTest::NormF::Absolute);
      else if (tol_type_string == "One Norm")
        toltypes.push_back(NOX::StatusTest::NormF::Relative);
      else
      {
        std::string msg = "\"Tolerance Type\" must be either \"Absolute\" or \"Relative\"!";
        throwError("BuildNormFTest()",msg);
      }
    }

    // ------------------------------------------
    // get the tolerance
    // ------------------------------------------
    if (toltypes.at(i)==NOX::StatusTest::NormF::Absolute)
      tolerances.push_back(ql.get("Tolerance",1.0e-8));
    else
      tolerances.push_back(ql.get("Tolerance",1.0e-5));

    // ------------------------------------------
    // get the norm type
    // ------------------------------------------
    std::string norm_type_string = ql.get("Norm Type", "Two Norm");
    if (norm_type_string == "Two Norm")
      normtypes.push_back(NOX::Abstract::Vector::TwoNorm);
    else if (norm_type_string == "One Norm")
      normtypes.push_back(NOX::Abstract::Vector::OneNorm);
    else if (norm_type_string == "Max Norm")
      normtypes.push_back(NOX::Abstract::Vector::MaxNorm);
    else {
      std::string msg = "\"Norm Type\" must be either \"Two Norm\", \"One Norm\", or \"Max Norm\"!";
      throwError("BuildNormFTest()",msg);
    }

    // ------------------------------------------
    // get the scale type
    // ------------------------------------------
    std::string scale_type_string = ql.get("Scale Type", "Unscaled");
    if (scale_type_string == "Unscaled")
      scaletypes.push_back(NOX::StatusTest::NormF::Unscaled);
    else if (scale_type_string == "Scaled")
      scaletypes.push_back(NOX::StatusTest::NormF::Scaled);
    else {
      std::string msg = "\"Scale Type\" must be either \"Unscaled\" or \"Scaled\"!";
      throwError("BuildNormFTest()",msg);
    }
    // ------------------------------------------
    // increase iterator
    // ------------------------------------------
    ++i;
    quantity_string.str("");
    quantity_string << "Quantity " << i;
  } // loop over all quantity types

  // build the normF status test
  Teuchos::RCP<NOX::NLN::StatusTest::NormF> status_test =
      Teuchos::rcp(new NOX::NLN::StatusTest::NormF(
          quantitytypes,
          toltypes,
          tolerances,
          normtypes,
          scaletypes,
          &u));

  return status_test;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<NOX::StatusTest::Generic> NOX::NLN::StatusTest::Factory::BuildNormWRMSTest(
    Teuchos::ParameterList& p, const NOX::Utils& u) const
{
  // initialized to size zero
  std::vector<QuantityType> quantitytypes = std::vector<QuantityType>(0);
  std::vector<double> atol = std::vector<double>(0);
  std::vector<double> rtol = std::vector<double>(0);
  std::vector<double> tol  = std::vector<double>(0);
  std::vector<double> BDFMultiplier = std::vector<double>(0);
  std::vector<bool> disable_implicit_weighting =
      std::vector<bool>(0);

  // ------------------------------------------
  // Get Alpha
  // Minimum step size allowed during a
  // line search for WRMS norm to be flagged
  // as converged.
  // ------------------------------------------
  double alpha = p.get("Alpha", 1.0);

  // ------------------------------------------
  // Get Beta
  // Maximum linear solve tolerance allowed
  // for WRMS norm to be flagged as converged.
  // ------------------------------------------
  double beta = p.get("Beta", 0.5);

  int i=0;
  std::ostringstream quantity_string;
  quantity_string << "Quantity " << i;
  while (p.isSublist(quantity_string.str()))
  {
    Teuchos::ParameterList& ql = p.sublist(quantity_string.str(), true);

    // ------------------------------------------
    // get the Quantity Type
    // ------------------------------------------
    std::string quantity_type_string = ql.get("Quantity Type","???");
    QuantityType qType = String2QuantityType(quantity_type_string);
    if (qType == quantity_unknown)
    {
      std::ostringstream msg;
      msg << "The \"Quantity Type\" is a required parameter \n"
          << "and the chosen option \"" << quantity_type_string << "\" is invalid!";
      throwError("BuildNormWRMSTest()",msg.str());
    }
    else
      quantitytypes.push_back(qType);

    // ------------------------------------------
    // get Absolute Tolerance
    // ------------------------------------------
    atol.push_back(ql.get("Absolute Tolerance",1.0e-8));

    // ------------------------------------------
    // get Relative Tolerance
    // ------------------------------------------
    rtol.push_back(ql.get("Relative Tolerance",1.0e-5));

    // ------------------------------------------
    // get Tolerance
    // Required tolerance for the NormWRMS to be
    // declared converged.
    // ------------------------------------------
    tol.push_back(ql.get("Tolerance",1.0e-5));

    // ------------------------------------------
    // get BDF multiplier
    // ------------------------------------------
    BDFMultiplier.push_back(ql.get("BDF Muliplier",1.0));

    // ------------------------------------------
    // get Disable Implicit Weighting
    // ------------------------------------------
    std::string diw_string = ql.get("Disable Implicit Weighting","Yes");
    if (diw_string == "Yes" or diw_string == "yes")
      disable_implicit_weighting.push_back(true);
    else if (diw_string == "No" or diw_string == "no")
      disable_implicit_weighting.push_back(false);
    else {
      std::string msg = "\"Disable Implicit Weighting\" must be either "
          "\"Yes\", \"yes\", \"No\" or \"no\"!";
      throwError("BuildNormWRMSTest()",msg);
    }

    // ------------------------------------------
    // increase iterator
    // ------------------------------------------
    ++i;
    quantity_string.str("");
    quantity_string << "Quantity " << i;
  } // loop over all quantity types

  Teuchos::RCP<NOX::NLN::StatusTest::NormWRMS> status_test =
      Teuchos::rcp(new NOX::NLN::StatusTest::NormWRMS(
                   quantitytypes,
                   rtol,
                   atol,
                   BDFMultiplier,
                   tol,
                   alpha,
                   beta,
                   disable_implicit_weighting));

  return status_test;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<NOX::StatusTest::Generic> NOX::NLN::StatusTest::Factory::
BuildComboTest(Teuchos::ParameterList& p, const NOX::Utils& u,
           std::map<std::string, Teuchos::RCP<NOX::StatusTest::Generic> >*
           tagged_tests) const
{
  std::string combo_type_string = Teuchos::get<std::string>(p, "Combo Type");
  NOX::StatusTest::Combo::ComboType combo_type = NOX::StatusTest::Combo::AND;
  if (combo_type_string == "AND")
    combo_type = NOX::StatusTest::Combo::AND;
  else if (combo_type_string == "OR")
    combo_type = NOX::StatusTest::Combo::OR;
  else{
    throwError("BuildComboTest()","The \"Combo Type\" must be \"AND\" or \"OR\"!");
  }

  Teuchos::RCP<NOX::NLN::StatusTest::Combo> combo_test =
    Teuchos::rcp(new NOX::NLN::StatusTest::Combo(combo_type, &u));

  int i=0;
  std::ostringstream subtest_name;
  subtest_name << "Test " << i;
  while (p.isSublist(subtest_name.str()))
  {
    Teuchos::ParameterList& subtest_list = p.sublist(subtest_name.str(), true);

    Teuchos::RCP<NOX::StatusTest::Generic> subtest =
      this->BuildOuterStatusTests(subtest_list, u, tagged_tests);

    combo_test->addStatusTest(subtest);

    // increase iterator
    ++i;
    subtest_name.str("");
    subtest_name << "Test " << i;
  }

  return combo_test;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool NOX::NLN::StatusTest::Factory::CheckAndTagTest(
    const Teuchos::ParameterList& p,
    const Teuchos::RCP<NOX::StatusTest::Generic>& test,
    std::map<std::string, Teuchos::RCP<NOX::StatusTest::Generic> >*
    tagged_tests) const
{
  if ( (Teuchos::isParameterType<std::string>(p, "Tag")) && (tagged_tests != NULL) ) {
    (*tagged_tests)[Teuchos::getParameter<std::string>(p, "Tag")] = test;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void NOX::NLN::StatusTest::Factory::throwError(
    const std::string& functionName,
    const std::string& errorMsg) const
{
  std::ostringstream msg;
  msg << "ERROR - NOX::NLN::StatusTest::Factory::" << functionName
      << " - " << errorMsg << std::endl;
  dserror(msg.str());
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
Teuchos::RCP<NOX::StatusTest::Generic> NOX::NLN::StatusTest::BuildOuterStatusTests(
    Teuchos::ParameterList& p, const NOX::Utils& u,
    std::map<std::string, Teuchos::RCP<NOX::StatusTest::Generic> >* tagged_tests)
{
  Factory factory;
  return factory.BuildOuterStatusTests(p,u,tagged_tests);
}