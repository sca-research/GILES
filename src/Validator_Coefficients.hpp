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
    @file Validator_Coefficients.hpp
    @brief Contains the validation rules for validating a Coefficients file.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef VALIDATOR_COEFFICIENTS_HPP
#define VALIDATOR_COEFFICIENTS_HPP

#include <string>  // for string

#include <nlohmann/json.hpp>  // for json

namespace ELMO2
{
namespace Internal
{

//! @class Validator_Coefficients
//! @brief A static class that validates Coefficients before they are stored
//! within Coefficient objects. This contains all validation rules, implemented
//! as individual functions, that need to be run to ensure that the JSON is in
//! the correct format as valid JSON does not mean that it is a valid
//! Coefficients file.
class Validator_Coefficients
{
private:
    static void Validate_Not_Empty(const nlohmann::json& p_coefficients,
                                   const std::string& p_exception_message);

    static void Validate_Is_Object(const nlohmann::json& p_json);

    static void Validate_Is_Number(const nlohmann::json& p_json);

    static void Validate_Is_Array(const nlohmann::json& p_json);

    static void Validate_Is_String(const nlohmann::json& p_json);

    static void
    Validate_Category_Headings_Coefficients(const nlohmann::json& p_category);

    static void
    Validate_Category_Headings_Constant(const nlohmann::json& p_category);

    static void Validate_Category_Correct_Interaction_Terms(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients);

    static void Validate_Category_Interaction_Terms_Size(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients);

    static void
    Validate_Category_Instructions_Unique(const nlohmann::json& p_category,
                                          const std::string& p_category_key,
                                          const nlohmann::json& p_coefficients);

    static void
    Validate_Category_Header_Unique(const nlohmann::json& p_category,
                                    const std::string& p_category_key,
                                    const nlohmann::json& p_coefficients);

    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class containing
    //! nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_constructor
    Validator_Coefficients(const Validator_Coefficients&) = delete;

    //! @brief This has been deleted to ensure the copy
    //! assignment operator cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_assignment
    Validator_Coefficients& operator=(const Validator_Coefficients&) = delete;

public:
    static void Validate_Json(const nlohmann::json& p_coefficients);
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // VALIDATOR_COEFFICIENTS_HPP
