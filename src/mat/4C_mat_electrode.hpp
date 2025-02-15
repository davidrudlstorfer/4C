// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_MAT_ELECTRODE_HPP
#define FOUR_C_MAT_ELECTRODE_HPP

#include "4C_config.hpp"

#include "4C_comm_parobjectfactory.hpp"
#include "4C_mat_elchsinglemat.hpp"
#include "4C_utils_function_of_scalar.hpp"

FOUR_C_NAMESPACE_OPEN

namespace Mat
{
  namespace PAR
  {
    //! models for half cell open circuit potential of electrode
    enum class OCPModels
    {
      function,
      redlichkister,
      taralov
    };

    //! parameters for electrode material
    class Electrode : public ElchSingleMat
    {
     public:
      //! constructor
      explicit Electrode(const Core::Mat::PAR::Parameter::Data& matdata);

      //! create instance of electrode material
      std::shared_ptr<Core::Mat::Material> create_material() override;

      //! @name parameters for electrode material
      //! @{
      //! saturation value of intercalated Lithium concentration
      const double cmax_;

      //! lithiation value corresponding to saturation value of intercalated Lithium concentration
      //! #cmax_
      const double chimax_;

      //! model for half cell open circuit potential
      const OCPModels ocpmodel_;

      //! number of function defining the open circuit potential
      int ocpfunctnum_;

      //! parameters underlying half cell open circuit potential model
      std::vector<double> ocppara_;

      //! lower bound of validity (as a fraction of c_max) for prescribed open circuit potential
      //! calculation model
      const double xmin_;

      //! upper bound of validity (as a fraction of c_max) for prescribed open circuit potential
      //! calculation model
      const double xmax_;
      //! @}
    };  // class Mat::PAR::Electrode
  }  // namespace PAR


  /*----------------------------------------------------------------------*/
  class ElectrodeType : public Core::Communication::ParObjectType
  {
   public:
    [[nodiscard]] std::string name() const override { return "ElectrodeType"; };

    static ElectrodeType& instance() { return instance_; };

    Core::Communication::ParObject* create(Core::Communication::UnpackBuffer& buffer) override;

   private:
    static ElectrodeType instance_;
  };


  /*----------------------------------------------------------------------*/
  //! wrapper for electrode material
  class Electrode : public ElchSingleMat
  {
   public:
    //! construct empty electrode material
    Electrode() = default;

    //! construct electrode material with specific material parameters
    explicit Electrode(Mat::PAR::Electrode* params);

    //! @name packing and unpacking
    /*!
      \brief Return unique ParObject id

      Every class implementing ParObject needs a unique id defined at the
      top of parobject.H (this file) and should return it in this method.
    */
    [[nodiscard]] int unique_par_object_id() const override
    {
      return ElectrodeType::instance().unique_par_object_id();
    };

    /*!
      \brief Pack this class so it can be communicated

      Resizes the vector data and stores all information of a class in it.
      The first information to be stored in data has to be the
      unique ParObject ID delivered by unique_par_object_id() which will then
      identify the exact class on the receiving processor.

      \param data (in/out): char vector to store class information
    */
    void pack(Core::Communication::PackBuffer& data) const override;

    /*!
      \brief Unpack data from a char vector into this class

      The vector data contains all information to rebuild the
      exact copy of an instance of a class on a different processor.
      The first entry in data has to be an integer which is the unique
      parobject id defined at the top of this file and delivered by
      unique_par_object_id().

      \param data (in) : vector storing all data to be unpacked into this instance.
    */
    void unpack(Core::Communication::UnpackBuffer& buffer) override;
    //@}

    //! return material type
    [[nodiscard]] Core::Materials::MaterialType material_type() const override
    {
      return Core::Materials::m_electrode;
    };

    //! clone electrode material
    [[nodiscard]] std::shared_ptr<Core::Mat::Material> clone() const override
    {
      return std::make_shared<Electrode>(*this);
    };

    //! return lithiation value corresponding to saturation value of intercalated Lithium
    //! concentration
    [[nodiscard]] double chi_max() const { return params_->chimax_; }

    //! return saturation value of intercalated Lithium concentration
    [[nodiscard]] double c_max() const { return params_->cmax_; }

    /*!
     * @brief compute the current lithiation
     *
     * @param[in] concentration  concentration
     * @param[in] chi_max        lithiation value of saturation value of intercalated lithium
     *                           concentration
     * @param[in] c_max          saturation value of intercalated lithium concentration
     * @param[in] detjacobian    determinant of the deformation gradient
     * @return the current lithiation
     */
    static double compute_intercalation_fraction(const double concentration, const double chi_max,
        const double c_max, const double detjacobian)
    {
      return (concentration * chi_max * detjacobian) / c_max;
    }

    /*!
     * @brief compute the derivative of the lithiation w.r.t. the concentration
     *
     * @param[in] chi_max      lithiation value of saturation value of intercalated lithium
     *                         concentration
     * @param[in] c_max        saturation value of intercalated lithium concentration
     * @param[in] detjacobian  determinant of the deformation gradient
     * @return the derivative of current lithiation w.r.t. concentration
     */
    static double compute_d_intercalation_fraction_d_concentration(
        const double chi_max, const double c_max, const double detjacobian)
    {
      return chi_max * detjacobian / c_max;
    }

    /*!
     * @brief compute the derivative of the lithiation w.r.t. the determinant of the deformation
     * gradient
     *
     * @param[in] concentration  concentration
     * @param[in] chi_max        lithiation value of saturation value of intercalated lithium
     *                           concentration
     * @param[in] c_max          saturation value of intercalated lithium concentration
     * @return  the derivative of the lithiation w.r.t. the determinant of the deformation gradient
     */
    static double compute_d_intercalation_fraction_d_det_f(
        const double concentration, const double chi_max, const double c_max)
    {
      return concentration * chi_max / c_max;
    }

    /*!
     * @brief compute first derivative of half cell open circuit potential w.r.t. concentration
     *
     * @param[in] concentration  concentration
     * @param[in] faraday        Faraday constant
     * @param[in] frt            factor F/RT
     * @param[in] detF           determinant of Jacobian from deformation at Gauss point
     * @return derivative of open circuit potential w.r.t. concentration
     */
    [[nodiscard]] double compute_d_open_circuit_potential_d_concentration(
        double concentration, double faraday, double frt, double detF) const;

    /*!
     * @brief calculate the first derivative of the open circuit potential w.r.t. the determinant of
     * the deformation gradient
     *
     * @param[in] concentration  concentration
     * @param[in] faraday        Faraday constant
     * @param[in] frt            factor F/RT
     * @param[in] detF           determinant of Jacobian from deformation at Gauss point
     * @return derivative of open circuit potential w.r.t. the determinant of the deformation
     * gradient
     */
    [[nodiscard]] double compute_d_open_circuit_potential_d_det_f(
        double concentration, double faraday, double frt, double detF) const;

    /*!
     * @brief calculate the first derivative of the open circuit potential w.r.t. the intercalation
     * fraction
     *
     * @param[in] X         intercalation fraction
     * @param[in] faraday   Faraday constant
     * @param[in] frt       factor F/RT
     * @return derivative of the open circuit potential w.r.t. the intercalation fraction
     */
    [[nodiscard]] double compute_d_open_circuit_potential_d_intercalation_fraction(
        double X, double faraday, double frt) const;

    /*!
     * @brief compute first derivative of half cell open circuit potential w.r.t. temperature
     *
     * @param[in] concentration  concentration
     * @param[in] faraday        Faraday constant
     * @param[in] gasconstant    General gas constant
     * @return derivative of open circuit potential w.r.t. temperature
     */
    [[nodiscard]] double compute_d_open_circuit_potential_d_temperature(
        const double concentration, const double faraday, const double gasconstant) const;

    /*!
     * @brief compute half cell open circuit potential
     *
     * @param[in] concentration  concentration
     * @param[in] faraday        Faraday constant
     * @param[in] frt            factor F/RT
     * @param[in] detF           determinant of jacobian at gauss point
     * @return open circuit potential
     */
    [[nodiscard]] double compute_open_circuit_potential(
        double concentration, double faraday, double frt, double detF) const;

    /*!
     * @brief compute second derivative of half cell open circuit potential w.r.t. concentration
     *
     * @param[in] concentration  concentration
     * @param[in] faraday        Faraday constant
     * @param[in] frt            factor F/RT
     * @param[in] detF           determinant of jacobian at gauss point
     * @return 2nd derivative of open circuit potential w.r.t. concentration
     */
    [[nodiscard]] double compute_d2_open_circuit_potential_d_concentration_d_concentration(
        double concentration, double faraday, double frt, double detF) const;

   protected:
    //! return material parameters
    [[nodiscard]] Core::Mat::PAR::Parameter* parameter() const override { return params_; }

   private:
    //! my material parameters
    Mat::PAR::Electrode* params_;
  };
}  // namespace Mat
FOUR_C_NAMESPACE_CLOSE

#endif
