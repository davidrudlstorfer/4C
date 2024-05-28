/*----------------------------------------------------------------------*/
/*! \file

\brief Input parameters for 0d cardiovascular-structure coupling

\level 2

*/

/*----------------------------------------------------------------------*/



#include "4C_inpar_cardiovascular0d.hpp"

#include "4C_io_condition_definition.hpp"
#include "4C_utils_parameter_list.hpp"

FOUR_C_NAMESPACE_OPEN



void INPAR::CARDIOVASCULAR0D::SetValidParameters(Teuchos::RCP<Teuchos::ParameterList> list)
{
  using namespace INPUT;
  using Teuchos::setStringToIntegralParameter;
  using Teuchos::tuple;

  Teuchos::ParameterList& cardvasc0dstruct =
      list->sublist("CARDIOVASCULAR 0D-STRUCTURE COUPLING", false, "");

  CORE::UTILS::DoubleParameter("TOL_CARDVASC0D_RES", 1.0E-08,
      "tolerance in the cardiovascular0d error norm for the Newton iteration", &cardvasc0dstruct);
  CORE::UTILS::DoubleParameter("TOL_CARDVASC0D_DOFINCR", 1.0E-08,
      "tolerance in the cardiovascular0d dof increment error norm for the Newton iteration",
      &cardvasc0dstruct);
  CORE::UTILS::DoubleParameter("TIMINT_THETA", 0.5,
      "theta for one-step-theta time-integration scheme of Cardiovascular0D", &cardvasc0dstruct);
  CORE::UTILS::BoolParameter("RESTART_WITH_CARDVASC0D", "No",
      "Must be chosen if a non-cardiovascular0d simulation is to be restarted as "
      "cardiovascular0d-structural coupled problem.",
      &cardvasc0dstruct);
  CORE::UTILS::BoolParameter("ENHANCED_OUTPUT", "No",
      "Set to yes for enhanced output (like e.g. derivative information)", &cardvasc0dstruct);

  // linear solver id used for monolithic 0D cardiovascular-structural problems
  CORE::UTILS::IntParameter("LINEAR_COUPLED_SOLVER", -1,
      "number of linear solver used for cardiovascular 0D-structural problems", &cardvasc0dstruct);

  setStringToIntegralParameter<int>("SOLALGORITHM", "direct", "",
      tuple<std::string>("simple", "direct", "AMGnxn"),
      tuple<int>(INPAR::CARDIOVASCULAR0D::cardvasc0dsolve_simple,
          INPAR::CARDIOVASCULAR0D::cardvasc0dsolve_direct,
          INPAR::CARDIOVASCULAR0D::cardvasc0dsolve_AMGnxn),
      &cardvasc0dstruct);

  CORE::UTILS::DoubleParameter("T_PERIOD", -1.0, "periodic time", &cardvasc0dstruct);
  CORE::UTILS::DoubleParameter(
      "EPS_PERIODIC", 1.0e-16, "tolerance for periodic state", &cardvasc0dstruct);

  CORE::UTILS::BoolParameter(
      "PTC_3D0D", "No", "Set to yes for doing PTC 2x2 block system.", &cardvasc0dstruct);

  CORE::UTILS::DoubleParameter("K_PTC", 0.0,
      "PTC parameter: 0 means normal Newton, ->infty means steepest desc", &cardvasc0dstruct);

  Teuchos::ParameterList& cardvasc0dsyspulcirc =
      cardvasc0dstruct.sublist("SYS-PUL CIRCULATION PARAMETERS", false, "");

  CORE::UTILS::DoubleParameter("R_arvalve_max_l", 0.0,
      "maximal left arterial (semilunar) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_arvalve_min_l", 0.0,
      "minimal left arterial (semilunar) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_atvalve_max_l", 0.0,
      "maximal left atrial (atrioventricular) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_atvalve_min_l", 0.0,
      "minimal left atrial (atrioventricular) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_arvalve_max_r", 0.0,
      "maximal right arterial (semilunar) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_arvalve_min_r", 0.0,
      "minimal right arterial (semilunar) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_atvalve_max_r", 0.0,
      "maximal right atrial (atrioventricular) valve resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("R_atvalve_min_r", 0.0,
      "minimal right atrial (atrioventricular) valve resistance", &cardvasc0dsyspulcirc);

  setStringToIntegralParameter<int>("ATRIUM_MODEL", "0D", "",
      tuple<std::string>("0D", "3D", "prescribed"),
      tuple<int>(INPAR::CARDIOVASCULAR0D::atr_elastance_0d,
          INPAR::CARDIOVASCULAR0D::atr_structure_3d, INPAR::CARDIOVASCULAR0D::atr_prescribed),
      &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Atrium_act_curve_l", -1,
      "left atrial activation curve (ONLY for ATRIUM_MODEL '0D'!)", &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Atrium_act_curve_r", -1,
      "right atrial activation curve (ONLY for ATRIUM_MODEL '0D'!)", &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Atrium_prescr_E_curve_l", -1,
      "left atrial elastance prescription curve (ONLY for ATRIUM_MODEL 'prescribed'!)",
      &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Atrium_prescr_E_curve_r", -1,
      "right atrial elastance prescription curve (ONLY for ATRIUM_MODEL 'prescribed'!)",
      &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_at_max_l", 0.0, "0D maximum left atrial elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_at_min_l", 0.0, "0D baseline left atrial elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_at_max_r", 0.0, "0D maximum right atrial elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_at_min_r", 0.0, "0D baseline right atrial elastance", &cardvasc0dsyspulcirc);

  setStringToIntegralParameter<int>("VENTRICLE_MODEL", "3D", "",
      tuple<std::string>("3D", "0D", "prescribed"),
      tuple<int>(INPAR::CARDIOVASCULAR0D::ventr_structure_3d,
          INPAR::CARDIOVASCULAR0D::ventr_elastance_0d, INPAR::CARDIOVASCULAR0D::ventr_prescribed),
      &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Ventricle_act_curve_l", -1,
      "left ventricular activation curve (ONLY for VENTRICLE_MODEL '0D'!)", &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Ventricle_act_curve_r", -1,
      "right ventricular activation curve (ONLY for VENTRICLE_MODEL '0D'!)", &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Ventricle_prescr_E_curve_l", -1,
      "left ventricular elastance prescription curve (ONLY for VENTRICLE_MODEL 'prescribed'!)",
      &cardvasc0dsyspulcirc);
  CORE::UTILS::IntParameter("Ventricle_prescr_E_curve_r", -1,
      "right ventricular elastance prescription curve (ONLY for VENTRICLE_MODEL 'prescribed'!)",
      &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_v_max_l", 0.0, "0D maximum left ventricular elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_v_min_l", 0.0, "0D baseline left ventricular elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_v_max_r", 0.0, "0D maximum right ventricular elastance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "E_v_min_r", 0.0, "0D baseline right ventricular elastance", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "C_ar_sys", 0.0, "systemic arterial compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_ar_sys", 0.0, "systemic arterial resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "L_ar_sys", 0.0, "systemic arterial inertance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "Z_ar_sys", 0.0, "systemic arterial impedance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_ar_pul", 0.0, "pulmonary arterial compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_ar_pul", 0.0, "pulmonary arterial resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "L_ar_pul", 0.0, "pulmonary arterial inertance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "Z_ar_pul", 0.0, "pulmonary arterial impedance", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "C_ven_sys", 0.0, "systemic venous compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_ven_sys", 0.0, "systemic venous resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "L_ven_sys", 0.0, "systemic venous inertance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_ven_pul", 0.0, "pulmonary venous compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_ven_pul", 0.0, "pulmonary venous resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "L_ven_pul", 0.0, "pulmonary venous inertance", &cardvasc0dsyspulcirc);

  // intital conditions
  CORE::UTILS::DoubleParameter(
      "q_vin_l_0", 0.0, "initial left ventricular in-flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_at_l_0", 0.0, "initial left atrial pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_vout_l_0", 0.0, "initial left ventricular out-flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_v_l_0", 0.0, "initial left ventricular pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_ar_sys_0", 0.0, "initial systemic arterial pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_ar_sys_0", 0.0, "initial systemic arterial flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_ven_sys_0", 0.0, "initial systemic venous pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_ven_sys_0", 0.0, "initial systemic venous flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_vin_r_0", 0.0, "initial right ventricular in-flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_at_r_0", 0.0, "initial right atrial pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_vout_r_0", 0.0, "initial right ventricular out-flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_v_r_0", 0.0, "initial right ventricular pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_ar_pul_0", 0.0, "initial pulmonary arterial pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_ar_pul_0", 0.0, "initial pulmonary arterial flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_ven_pul_0", 0.0, "initial pulmonary venous pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_ven_pul_0", 0.0, "initial pulmonary venous flux", &cardvasc0dsyspulcirc);

  // unstressed volumes - only for postprocessing matters!
  CORE::UTILS::DoubleParameter(
      "V_at_l_u", 0.0, "unstressed volume of left 0D atrium", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_v_l_u", 0.0, "unstressed volume of left 0D ventricle", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_ar_sys_u", 0.0,
      "unstressed volume of systemic arteries and capillaries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_ven_sys_u", 100.0e3, "unstressed volume of systemic veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_at_r_u", 0.0, "unstressed volume of right 0D atrium", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_v_r_u", 0.0, "unstressed volume of right 0D ventricle", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_ar_pul_u", 0.0,
      "unstressed volume of pulmonary arteries and capillaries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_ven_pul_u", 120.0e3, "unstressed volume of pulmonary veins", &cardvasc0dsyspulcirc);



  // parameters for extended sys pul circulation including periphery
  CORE::UTILS::DoubleParameter(
      "C_arspl_sys", 0.0, "systemic arterial splanchnic compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_arspl_sys", 0.0, "systemic arterial splanchnic resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_arespl_sys", 0.0, "systemic arterial extra-splanchnic compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_arespl_sys", 0.0, "systemic arterial extra-splanchnic resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_armsc_sys", 0.0, "systemic arterial muscular compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_armsc_sys", 0.0, "systemic arterial muscular resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_arcer_sys", 0.0, "systemic arterial cerebral compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_arcer_sys", 0.0, "systemic arterial cerebral resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_arcor_sys", 0.0, "systemic arterial coronary compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_arcor_sys", 0.0, "systemic arterial coronary resistance", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "C_venspl_sys", 0.0, "systemic venous splanchnic compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_venspl_sys", 0.0, "systemic venous splanchnic resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_venespl_sys", 0.0, "systemic venous extra-splanchnic compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_venespl_sys", 0.0, "systemic venous extra-splanchnic resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_venmsc_sys", 0.0, "systemic venous muscular compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_venmsc_sys", 0.0, "systemic venous muscular resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_vencer_sys", 0.0, "systemic venous cerebral compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_vencer_sys", 0.0, "systemic venous cerebral resistance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "C_vencor_sys", 0.0, "systemic venous coronary compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_vencor_sys", 0.0, "systemic venous coronary resistance", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "C_cap_pul", 0.0, "pulmonary capillary compliance", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "R_cap_pul", 0.0, "pulmonary capillary resistance", &cardvasc0dsyspulcirc);

  // initial conditions for extended sys pul circulation including periphery
  CORE::UTILS::DoubleParameter("p_arperi_sys_0", 0.0,
      "initial systemic peripheral arterial pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_arspl_sys_0", 0.0, "initial systemic arterial splanchnic flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("q_arespl_sys_0", 0.0,
      "initial systemic arterial extra-splanchnic flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_armsc_sys_0", 0.0, "initial systemic arterial muscular flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_arcer_sys_0", 0.0, "initial systemic arterial cerebral flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_arcor_sys_0", 0.0, "initial systemic arterial coronary flux", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "p_venspl_sys_0", 0.0, "initial systemic venous splanchnic pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_venspl_sys_0", 0.0, "initial systemic venous splanchnic flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("p_venespl_sys_0", 0.0,
      "initial systemic venous extra-splanchnic pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("q_venespl_sys_0", 0.0,
      "initial systemic venous extra-splanchnic flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_venmsc_sys_0", 0.0, "initial systemic venous muscular pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_venmsc_sys_0", 0.0, "initial systemic venous muscular flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_vencer_sys_0", 0.0, "initial systemic venous cerebral pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_vencer_sys_0", 0.0, "initial systemic venous cerebral flux", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "p_vencor_sys_0", 0.0, "initial systemic venous coronary pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_vencor_sys_0", 0.0, "initial systemic venous coronary flux", &cardvasc0dsyspulcirc);

  CORE::UTILS::DoubleParameter(
      "p_cap_pul_0", 0.0, "initial pulmonary capillary pressure", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "q_cap_pul_0", 0.0, "initial pulmonary capillary flux", &cardvasc0dsyspulcirc);


  // unstressed volumes
  // default values according to Ursino et al. Am J Physiol Heart Circ Physiol (2000), in mm^3
  CORE::UTILS::DoubleParameter("V_arspl_sys_u", 274.4e3,
      "unstressed volume of systemic splanchnic arteries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_arespl_sys_u", 134.64e3,
      "unstressed volume of systemic extra-splanchnic arteries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_armsc_sys_u", 105.8e3,
      "unstressed volume of systemic muscular arteries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_arcer_sys_u", 72.13e3,
      "unstressed volume of systemic cerebral arteries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_arcor_sys_u", 24.0e3,
      "unstressed volume of systemic coronary arteries", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_venspl_sys_u", 1121.0e3,
      "unstressed volume of systemic splanchnic veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_venespl_sys_u", 550.0e3,
      "unstressed volume of systemic extra-splanchnic veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_venmsc_sys_u", 432.14e3,
      "unstressed volume of systemic muscular veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_vencer_sys_u", 294.64e3,
      "unstressed volume of systemic cerebral veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter("V_vencor_sys_u", 98.21e3,
      "unstressed volume of systemic coronary veins", &cardvasc0dsyspulcirc);
  CORE::UTILS::DoubleParameter(
      "V_cap_pul_u", 123.0e3, "unstressed volume of pulmonary capillaries", &cardvasc0dsyspulcirc);



  Teuchos::ParameterList& cardvascrespir0d =
      cardvasc0dstruct.sublist("RESPIRATORY PARAMETERS", false, "");

  setStringToIntegralParameter<int>("RESPIRATORY_MODEL", "None", "",
      tuple<std::string>("None", "Standard"),
      tuple<int>(INPAR::CARDIOVASCULAR0D::resp_none, INPAR::CARDIOVASCULAR0D::resp_standard),
      &cardvascrespir0d);


  CORE::UTILS::DoubleParameter("L_alv", 0.0, "alveolar inertance", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("R_alv", 0.0, "alveolar resistance", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("E_alv", 0.0, "alveolar elastance", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("V_lung_tidal", 0.4e6,
      "tidal volume (the total volume of inspired air, in a single breath)", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("V_lung_dead", 0.15e6, "dead space volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("V_lung_u", 0.0,
      "unstressed lung volume (volume of the lung when it is fully collapsed outside the body)",
      &cardvascrespir0d);

  CORE::UTILS::IntParameter("U_t_curve", -1,
      "time-varying, prescribed pleural pressure curve driven by diaphragm", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("U_m", 0.0, "in-breath pressure", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("fCO2_ext", 0.03, "atmospheric CO2 gas fraction", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("fO2_ext", 0.21, "atmospheric O2 gas fraction", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("kappa_CO2", 0.0,
      "diffusion coefficient for CO2 across the hemato-alveolar membrane, in molar value / (time * "
      "pressure)",
      &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("kappa_O2", 0.0,
      "diffusion coefficient for O2 across the hemato-alveolar membrane, in molar value / (time * "
      "pressure)",
      &cardvascrespir0d);

  // should be 22.4 liters per mol !
  // however we specify it as an input parameter since its decimal power depends on the system of
  // units your whole model is specified in! i.e. if you have kg - mm - s - mmol, it's 22.4e3 mm^3 /
  // mmol
  CORE::UTILS::DoubleParameter(
      "V_m_gas", 22.4e3, "molar volume of an ideal gas", &cardvascrespir0d);

  // should be 47.1 mmHg = 6.279485 kPa !
  // however we specify it as an input parameter since its decimal power depends on the system of
  // units your whole model is specified in! i.e. if you have kg - mm - s - mmol, it's 6.279485 kPa
  CORE::UTILS::DoubleParameter("p_vap_water_37", 6.279485,
      "vapor pressure of water at 37  degrees celsius", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("alpha_CO2", 0.0,
      "CO2 solubility constant, in molar value / (volume * pressure)", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("alpha_O2", 0.0,
      "O2 solubility constant, in molar value / (volume * pressure)", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("c_Hb", 0.0,
      "hemoglobin concentration of the blood, in molar value / volume", &cardvascrespir0d);


  CORE::UTILS::DoubleParameter(
      "M_CO2_arspl", 0.0, "splanchnic metabolic rate of CO2 production", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_O2_arspl", 0.0, "splanchnic metabolic rate of O2 consumption", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_CO2_arespl", 0.0, "extra-splanchnic metabolic rate of CO2 production", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_O2_arespl", 0.0, "extra-splanchnic metabolic rate of O2 consumption", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_CO2_armsc", 0.0, "muscular metabolic rate of CO2 production", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_O2_armsc", 0.0, "muscular metabolic rate of O2 consumption", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_CO2_arcer", 0.0, "cerebral metabolic rate of CO2 production", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_O2_arcer", 0.0, "cerebral metabolic rate of O2 consumption", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_CO2_arcor", 0.0, "coronary metabolic rate of CO2 production", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "M_O2_arcor", 0.0, "coronary metabolic rate of O2 consumption", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter("V_tissspl", 1.0, "splanchnic tissue volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "V_tissespl", 1.0, "extra-splanchnic tissue volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("V_tissmsc", 1.0, "muscular tissue volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("V_tisscer", 1.0, "cerebral tissue volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("V_tisscor", 1.0, "coronary tissue volume", &cardvascrespir0d);


  // initial conditions for respiratory model
  CORE::UTILS::DoubleParameter("V_alv_0", -1.0, "initial alveolar volume", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("q_alv_0", 0.0, "initial alveolar flux", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("p_alv_0", -1.0, "initial alveolar pressure", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter(
      "fCO2_alv_0", 0.05263, "initial alveolar CO2 fraction", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "fO2_alv_0", 0.1368, "initial alveolar O2 fraction", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter(
      "q_arspl_sys_in_0", 0.0, "initial arterial splanchnic in-flux", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "q_arsspl_sys_in_0", 0.0, "initial arterial extra-splanchnic in-flux", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "q_armsc_sys_in_0", 0.0, "initial arterial muscular in-flux", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "q_arcer_sys_in_0", 0.0, "initial arterial cerebral in-flux", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "q_arcor_sys_in_0", 0.0, "initial arterial coronary in-flux", &cardvascrespir0d);

  CORE::UTILS::DoubleParameter(
      "ppCO2_at_r_0", 1.0, "initial right atrial CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_at_r_0", 1.0, "initial right atrial O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_v_r_0", 1.0, "initial right ventricular CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_v_r_0", 1.0, "initial right ventricular O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_ar_pul_0", 1.0, "initial pulmonary arterial CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_ar_pul_0", 1.0, "initial pulmonary arterial O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_cap_pul_0", 1.0,
      "initial pulmonary capillary CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_cap_pul_0", 1.0, "initial pulmonary capillary O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_ven_pul_0", 1.0, "initial pulmonary venous CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_ven_pul_0", 1.0, "initial pulmonary venous O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_at_l_0", 1.0, "initial left atrial CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_at_l_0", 1.0, "initial left atrial O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_v_l_0", 1.0, "initial left ventricular CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_v_l_0", 1.0, "initial left ventricular O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_ar_sys_0", 1.0, "initial systemic arterial CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_ar_sys_0", 1.0, "initial systemic arterial O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_arspl_sys_0", 1.0,
      "initial systemic arterial splanchnic CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_arspl_sys_0", 1.0,
      "initial systemic arterial splanchnic O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_arespl_sys_0", 1.0,
      "initial systemic arterial extra-splanchnic CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_arespl_sys_0", 1.0,
      "initial systemic arterial extra-splanchnic O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_armsc_sys_0", 1.0,
      "initial systemic arterial muscular CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_armsc_sys_0", 1.0,
      "initial systemic arterial muscular O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_arcer_sys_0", 1.0,
      "initial systemic arterial cerebral CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_arcer_sys_0", 1.0,
      "initial systemic arterial cerebral O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_arcor_sys_0", 1.0,
      "initial systemic arterial coronary CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_arcor_sys_0", 1.0,
      "initial systemic arterial coronary O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_venspl_sys_0", 1.0,
      "initial systemic venous splanchnic CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_venspl_sys_0", 1.0,
      "initial systemic venous splanchnic O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_venespl_sys_0", 1.0,
      "initial systemic venous extra-splanchnic CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_venespl_sys_0", 1.0,
      "initial systemic venous extra-splanchnic O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_venmsc_sys_0", 1.0,
      "initial systemic venous muscular CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_venmsc_sys_0", 1.0,
      "initial systemic venous muscular O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_vencer_sys_0", 1.0,
      "initial systemic venous cerebral CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_vencer_sys_0", 1.0,
      "initial systemic venous cerebral O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppCO2_vencor_sys_0", 1.0,
      "initial systemic venous coronary CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter("ppO2_vencor_sys_0", 1.0,
      "initial systemic venous coronary O2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppCO2_ven_sys_0", 1.0, "initial systemic venous CO2 partial pressure", &cardvascrespir0d);
  CORE::UTILS::DoubleParameter(
      "ppO2_ven_sys_0", 1.0, "initial systemic venous O2 partial pressure", &cardvascrespir0d);
}



void INPAR::CARDIOVASCULAR0D::SetValidConditions(
    std::vector<Teuchos::RCP<INPUT::ConditionDefinition>>& condlist)
{
  using namespace INPUT;


  /*--------------------------------------------------------------------*/
  // Monolithic coupling of structure and a four-element windkessel - mhv 11/13

  Teuchos::RCP<ConditionDefinition> cardiovascular0d4elementwindkesselcondition = Teuchos::rcp(
      new ConditionDefinition("DESIGN SURF CARDIOVASCULAR 0D 4-ELEMENT WINDKESSEL CONDITIONS",
          "Cardiovascular0D4ElementWindkesselStructureCond", "Surface Cardiovascular0D",
          CORE::Conditions::Cardiovascular0D4ElementWindkessel_Structure, true,
          CORE::Conditions::geometry_type_surface));

  INPUT::AddNamedInt(cardiovascular0d4elementwindkesselcondition, "id");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "C");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "R_p");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "Z_c");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "L");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "p_ref");
  INPUT::AddNamedReal(cardiovascular0d4elementwindkesselcondition, "p_0");

  condlist.push_back(cardiovascular0d4elementwindkesselcondition);

  /*--------------------------------------------------------------------*/
  // Monolithic coupling of structure and an arterial cardiovascular 0D flow model accounting for
  // proximal and distal arterial pressure formulation proposed by Cristobal Bertoglio - mhv 03/14

  Teuchos::RCP<ConditionDefinition> cardiovascular0darterialproxdistcond = Teuchos::rcp(
      new ConditionDefinition("DESIGN SURF CARDIOVASCULAR 0D ARTERIAL PROX DIST CONDITIONS",
          "Cardiovascular0DArterialProxDistStructureCond",
          "Surface 0D cardiovascular arterial proximal and distal",
          CORE::Conditions::Cardiovascular0DArterialProxDist_Structure, true,
          CORE::Conditions::geometry_type_surface));

  INPUT::AddNamedInt(cardiovascular0darterialproxdistcond, "id");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_arvalve_max");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_arvalve_min");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_atvalve_max");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_atvalve_min");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "k_p");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "L_arp");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "C_arp");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_arp");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "C_ard");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "R_ard");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "p_ref");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "p_v_0");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "p_arp_0");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "y_arp_0");
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "p_ard_0");
  cardiovascular0darterialproxdistcond->AddComponent(
      Teuchos::rcp(new INPUT::SeparatorComponent("P_AT")));
  INPUT::AddNamedReal(cardiovascular0darterialproxdistcond, "fac");
  cardiovascular0darterialproxdistcond->AddComponent(
      Teuchos::rcp(new INPUT::SeparatorComponent("crv")));
  cardiovascular0darterialproxdistcond->AddComponent(
      Teuchos::rcp(new INPUT::IntComponent("curve", {0, true, true})));

  condlist.push_back(cardiovascular0darterialproxdistcond);

  /*--------------------------------------------------------------------*/
  // Monolithic coupling of structure and a full closed-loop 0D cardiovascular flow model
  // (closed-loop circulatory system model) mhv 02/15

  Teuchos::RCP<ConditionDefinition> cardiovascular0dsyspulcirculationcond = Teuchos::rcp(
      new ConditionDefinition("DESIGN SURF CARDIOVASCULAR 0D SYS-PUL CIRCULATION CONDITIONS",
          "Cardiovascular0DSysPulCirculationStructureCond",
          "Surface cardiovascular 0D sys pul circulation condition",
          CORE::Conditions::Cardiovascular0DSysPulCirculation_Structure, true,
          CORE::Conditions::geometry_type_surface));

  INPUT::AddNamedInt(cardiovascular0dsyspulcirculationcond, "id");
  cardiovascular0dsyspulcirculationcond->AddComponent(
      Teuchos::rcp(new INPUT::SeparatorComponent("TYPE")));
  cardiovascular0dsyspulcirculationcond->AddComponent(
      Teuchos::rcp(new INPUT::SelectionComponent("type", "ventricle_left",
          Teuchos::tuple<std::string>(
              "ventricle_left", "ventricle_right", "atrium_left", "atrium_right", "dummy"),
          Teuchos::tuple<std::string>(
              "ventricle_left", "ventricle_right", "atrium_left", "atrium_right", "dummy"),
          false)));

  condlist.push_back(cardiovascular0dsyspulcirculationcond);

  /*--------------------------------------------------------------------*/
  // Monolithic coupling of structure and a full closed-loop 0D cardiovascular flow model
  // (closed-loop circulatory system model) mhv 02/15

  Teuchos::RCP<ConditionDefinition> cardiovascularrespiratory0dsyspulperiphcirculationcond =
      Teuchos::rcp(new ConditionDefinition(
          "DESIGN SURF CARDIOVASCULAR RESPIRATORY 0D SYS-PUL PERIPH CIRCULATION CONDITIONS",
          "CardiovascularRespiratory0DSysPulPeriphCirculationStructureCond",
          "Surface 0D cardiovascular respiratory sys-pul periph circulation condition",
          CORE::Conditions::CardiovascularRespiratory0DSysPulPeriphCirculation_Structure, true,
          CORE::Conditions::geometry_type_surface));

  INPUT::AddNamedInt(cardiovascularrespiratory0dsyspulperiphcirculationcond, "id");
  cardiovascularrespiratory0dsyspulperiphcirculationcond->AddComponent(
      Teuchos::rcp(new INPUT::SeparatorComponent("TYPE")));
  cardiovascularrespiratory0dsyspulperiphcirculationcond->AddComponent(
      Teuchos::rcp(new INPUT::SelectionComponent("type", "ventricle_left",
          Teuchos::tuple<std::string>(
              "ventricle_left", "ventricle_right", "atrium_left", "atrium_right", "dummy"),
          Teuchos::tuple<std::string>(
              "ventricle_left", "ventricle_right", "atrium_left", "atrium_right", "dummy"),
          false)));

  condlist.push_back(cardiovascularrespiratory0dsyspulperiphcirculationcond);


  /*--------------------------------------------------------------------*/
  // Monolithic coupling of structure and 0D cardiovascular flow models: Neumann coupling surface -
  // mhv 11/13

  Teuchos::RCP<ConditionDefinition> cardiovascular0dstructurecouplingcond = Teuchos::rcp(
      new ConditionDefinition("DESIGN SURF CARDIOVASCULAR 0D-STRUCTURE COUPLING CONDITIONS",
          "SurfaceNeumannCardiovascular0D",
          "structure 0d cardiovascular coupling surface condition",
          CORE::Conditions::Cardiovascular0DStructureCoupling, true,
          CORE::Conditions::geometry_type_surface));

  INPUT::AddNamedInt(cardiovascular0dstructurecouplingcond, "coupling_id");

  condlist.push_back(cardiovascular0dstructurecouplingcond);
}

FOUR_C_NAMESPACE_CLOSE