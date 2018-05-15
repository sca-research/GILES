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
    @file Model_Factory.cpp
    @brief Contains the Model_Factory class; a factory class for initialisation
    of models. This file is processed by CMake as some of the contents are
    generated at compile time.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <functional>    // for bind
#include <memory>        // for unique_ptr
#include <stdexcept>     // for invalid_argument
#include <string>        // for oper...
#include <unordered_map> // for unordered_map

#include "Model_Factory.hpp"

#include "Model.hpp"

#include "Model_Hamming_Weight.hpp"
#include "Model_TEMPLATE.hpp"

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
class Coefficients;
class Execution;

std::unique_ptr<ELMO2::Internal::Model>
ELMO2::Internal::Model_Factory::Construct(
    const std::string& p_type,
    const ELMO2::Internal::Execution& p_execution,
    const ELMO2::Internal::Coefficients& p_coefficients)
{
    if (Get_All_Models().find(p_type) != Get_All_Models().end())
    {
        return Get_All_Models()[p_type](p_execution, p_coefficients);
    }

    // TODO: find a way to inegrate this? - Put this in register and make
    // Find_Interaction_Terms static? - THIS WONT WORK as Find_Interaction_Terms
    // requires loaded coeffs.json
    // Put this check within Construct? Or Maybe get_models_in_use()? - A lazy
    // approach would suggest in Construct() along with a check if it's enabled?
    /*
     *if (!model->Find_Interaction_Terms())
     *{
     *    throw std::logic_error("Model was not provided with correct "
     *                           "interaction terms by the Coefficients file.");
     *}
     */

    throw std::invalid_argument("Unknown Model type");
}

bool ELMO2::Internal::Model_Factory::Register(
    const std::string& p_type,
    const ELMO2::Internal::Model_Factory::Create_Function& p_constructor)
{
    if (Get_All_Models().find(p_type) == Get_All_Models().end())
    {
        Get_All_Models()[p_type] = p_constructor;
        return true;
    }
    return false;
}

std::unordered_map<std::string,
                   ELMO2::Internal::Model_Factory::Create_Function>&
ELMO2::Internal::Model_Factory::Get_All_Models()
{
    static std::unordered_map<std::string,
                              ELMO2::Internal::Model_Factory::Create_Function>
        models;
    return models;
}
} // namespace Internal
} // namespace ELMO2
