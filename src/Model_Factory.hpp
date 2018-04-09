/*
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file Model_Factory.hpp
    @brief Contains the Model_Factory class; a factory class for initialisation
    of models.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include <memory>        // for make...
#include <stdexcept>     // for inva...
#include <string>        // for oper...
#include <unordered_map> // for unor...

#include "Model.hpp"

namespace ELMO2
{
namespace Internal
{

//! @class Model_Factory
//! @brief This is a static factory class that assists in the construction of
//! models. By providing abstraction for the caller of the Model_Factory, it
//! hides the type of the model from the caller and allows the caller to make
//! use of the model through the Model base class interface regardless of the
//! derived type.
//! @note This class contains functionality that is generated by CMake. This
//! simplifies the process of adding and removing models and allows models to
//! be selectively compiled as a way to completely remove those as options.
//! @see https://en.wikipedia.org/wiki/Factory_method_pattern
class Model_Factory
{
private:
    //! The list of all available models, regardless of whether or not they
    //! are enabled.
    static const std::unordered_map<std::string, bool> m_all_models;

    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class containing
    //! nothing but static functions.
    Model_Factory(const Model_Factory&) = delete;

public:
    //! @brief This function assists with the initialisation of the models.
    //! This will be passed the parameters to be passed on to the model
    //! constructor. It will selectively construct a model based on the first
    //! parameter, p_model_type and return a pointer to it.
    //! @param p_model_type The type of the model to be constructed as a string.
    //! @param p_execution The Execution as received from the emulator.
    //! @param p_coefficients The Coefficients as loaded from the
    //! coefficients file.
    //! @returns A unique pointer to the constructed model. The type of this
    //! is to the base Model class allowing the caller to ignore the actual
    //! type of the model and make use of the Model class interface.
    static std::unique_ptr<ELMO2::Internal::Model>
    Create_Model(const std::string& p_model_type,
                 const ELMO2::Internal::Execution& p_execution,
                 const ELMO2::Internal::Coefficients& p_coefficients);

    //! @brief Retrieves a list of all of the models that have been compiled
    //! and whether or not they are enabled.
    //! @returns An unordered map, indexed by the name of the model, containing
    //! boolean values indicating as to whether that model is enabled by
    //! default.
    static const std::unordered_map<std::string, bool>& Get_All_Models();
};
} // namespace Internal
} // namespace ELMO2

#endif