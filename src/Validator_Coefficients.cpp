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
    @file Validator_Coefficients.cpp
    @brief Contains the validation rules for validating a Coefficients file.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <algorithm>  // for find
#include <iostream>   // for ios_base::failure, ios_base
#include <string>     // for basic_string, string

#include "Validator_Coefficients.hpp"

namespace ELMO2
{
namespace Internal
{
//! @brief This function serves as an entry point to the validation rules.
//! This function will invoke each validation rule in turn.
//! @param p_coefficients The coefficients to be validated, stored within a
//! nlohmann::json object.
//! returns This does not return anything as instead an exception will be
//! thrown if a rule fails.
void ELMO2::Internal::Validator_Coefficients::Validate_Json(
    const nlohmann::json& p_coefficients)
{
    Validate_Not_Empty(p_coefficients, "Coefficients file must not be empty.");
    Validate_Is_Object(p_coefficients);

    // Check each instruction category
    for (const auto& category : p_coefficients)
    {
        Validate_Is_Object(category);
        Validate_Not_Empty(
            category,
            "Coefficients file must not contain empty coefficient categories.");
        Validate_Category_Headings_Constant(category);
        Validate_Category_Headings_Coefficients(category);
        Validate_Is_Number(category["Constant"]);
        Validate_Is_Object(category["Coefficients"]);
    }
    Validate_Not_Empty(p_coefficients.front().at("Coefficients"),
                       "There must be at least one interaction term in the "
                       "Coefficients file.");
    for (const auto& category : p_coefficients.items())  // .items() is used as
    // Validate_category_Instruction_Unique
    // requires the key as well as the value
    {
        for (const auto& interaction_term : category.value()["Coefficients"])
        {
            Validate_Not_Empty(interaction_term,
                               "Each interaction term in the "
                               "Coefficients file must "
                               "contain at least one value.");
            Validate_Is_Array(interaction_term);
            for (const auto& value : interaction_term)
            {
                Validate_Is_Number(value);
            }
        }

        if (category.value().find("Instructions") != category.value().end())
        {
            Validate_Not_Empty(category.value()["Instructions"],
                               "Categories in the Coefficients file must "
                               "not contain an empty list of instructions.");
            Validate_Is_Array(category.value()["Instructions"]);
            for (const auto& instruction : category.value()["Instructions"])
            {
                Validate_Is_String(instruction);
            }
        }

        // If there is only one category then there is no need to perform
        // this validation check
        if (1 < p_coefficients.size())
        {
            Validate_Category_Correct_Interaction_Terms(category.value(),
                                                        p_coefficients);
        }
        Validate_Category_Interaction_Terms_Size(category.value(),
                                                 p_coefficients);

        // If the "Instructions" tag not is present then there is no need to
        // apply these validation rules as other catergories validations will
        // check this.
        // Additionally JSON specification does not allow two
        // objects with the same name.
        if (category.value().find("Instructions") != category.value().end())
        {
            Validate_Category_Instructions_Unique(
                category.value(), category.key(), p_coefficients);
            Validate_Category_Header_Unique(
                category.value(), category.key(), p_coefficients);
        }
    }
}

//! @brief Ensures that the given JSON is not an empty structure.
//! @param p_json The json to be validated
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an empty structure.
void ELMO2::Internal::Validator_Coefficients::Validate_Not_Empty(
    const nlohmann::json& p_json, const std::string& p_exception_message)
{
    if (p_json.empty())
    {
        throw std::ios_base::failure(p_exception_message);
    }
}

//! @brief Ensures that the given JSON is a JSON object.
//! @param p_json The json to be validated
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an not an object.
void ELMO2::Internal::Validator_Coefficients::Validate_Is_Object(
    const nlohmann::json& p_json)
{
    if (!p_json.is_object())
    {
        throw std::ios_base::failure("Expected a JSON object, found: " +
                                     p_json.dump());
    }
}

//! @brief Ensures that the given JSON is a JSON number.
//! @param p_json The json to be validated
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an not a number.
void ELMO2::Internal::Validator_Coefficients::Validate_Is_Number(
    const nlohmann::json& p_json)
{
    if (!p_json.is_number())
    {
        throw std::ios_base::failure("Expected a JSON number, found: " +
                                     p_json.dump());
    }
}

//! @brief Ensures that the given JSON is a JSON array.
//! @param p_json The json to be validated
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an not an array.
void ELMO2::Internal::Validator_Coefficients::Validate_Is_Array(
    const nlohmann::json& p_json)
{
    // TODO: Seperate newly added functionality into different methods.
    if (p_json.is_object())
    {
        for (const auto& item : p_json)
        {
            if (!item.is_number())
            {
                throw std::ios_base::failure(
                    "Expected a key value pair, found: " + p_json.dump());
            }
        }
        return;
    }
    if (!p_json.is_array())
    {
        throw std::ios_base::failure("Expected a JSON array, found: " +
                                     p_json.dump());
    }
}

//! @brief Ensures that the given JSON is a JSON string.
//! @param p_json The json to be validated
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an not a string.
void ELMO2::Internal::Validator_Coefficients::Validate_Is_String(
    const nlohmann::json& p_json)
{
    if (!p_json.is_string())
    {
        throw std::ios_base::failure("Expected a JSON string, found: " +
                                     p_json.dump());
    }
}

//! @brief Ensures that the given category contains a "Coefficients"
//! subheading. Without this, the category would not contain any actual
//! coefficient values.
//! @param p_category The instruction category to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the "Coefficients" heading was not
//! found.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Headings_Coefficients(const nlohmann::json& p_category)
{
    if (p_category.find("Coefficients") == p_category.end())
    {
        // Each category must contain Coefficients
        throw std::ios_base::failure(
            "Each category in the Coefficients file must contain a set of "
            "Coefficients.");
    }
}

//! @brief Ensures that the given category contains a "Constant" value.
//! subheading. Without this, the coefficient values could not be utilised
//! within a Model.
//! @param p_category The instruction category to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the "Constant" heading was not
//! found.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Headings_Constant(const nlohmann::json& p_category)
{
    if (p_category.find("Constant") == p_category.end())
    {
        // Each category must contain a Constant
        throw std::ios_base::failure("Each category in the Coefficients file "
                                     "must contain a Constant value.");
    }
}

//! @brief Ensures that the given category contains the same interaction
//! terms as every other category. The Model will expect to find the same
//! interaction terms for each category. The first category is used to check
//! against as all categories should be the same except when checking the
//! contents of the first category. When checking the contents of the first
//! category, the last category is used to check against.
//! @param p_category The instruction category to be validated.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the correct interaction terms were
//! not found.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Correct_Interaction_Terms(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients)
{
    // If this is the first category don't compare against itself, instead
    // use the last element as a reference as it is easy to access. When not
    // checking the first category, compare against the first.
    const nlohmann::json& reference_category =
        p_coefficients.front() == p_category ? p_coefficients.back()
                                             : p_coefficients.front();

    // check each interaction term individually.
    for (const auto& interaction_term : p_category["Coefficients"].items())
    {
        // if the reference category doesn't contain interaction_term.key()
        if (reference_category.at("Coefficients")
                .find(interaction_term.key()) ==
            reference_category.at("Coefficients").end())
        {
            throw std::ios_base::failure(
                "The same interaction terms must be provided for all "
                "categories in the Coefficients file.");
        }
    }
}

//! @brief Ensures that each of the interaction terms within the given
//! category contains the same amount of values. The first category is used
//! to check against as all categories should be the same.
//! @param p_category The instruction category to be validated.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an interaction term does not
//! contain the correct number of values.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Interaction_Terms_Size(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients)
{
    for (const auto& interaction_term : p_category["Coefficients"].items())
    {
        // Interaction terms must all have the same number of
        // Coefficient values.
        if (interaction_term.value().size() !=
            p_coefficients.front()["Coefficients"][interaction_term.key()]
                .size())
        {
            throw std::ios_base::failure(
                "Each interaction term in the Coefficients file must "
                "contain the same amount of values for each "
                "category.");
        }
    }
}

//! @brief Ensures that no category has the same name as an instruction within a
//! different category. This removes ambiguity as the same name can refer to two
//! different sets of coefficients.
//! @param p_category The instruction category to be validated.
//! @param p_category_key The heading of the category contained within
//! p_category.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an instruction was found to have
//! more than one set of coefficients associated with it.
void ELMO2::Internal::Validator_Coefficients::Validate_Category_Header_Unique(
    const nlohmann::json& p_category,
    const std::string& p_category_key,
    const nlohmann::json& p_coefficients)
{
    // For each category
    for (const auto& search_category : p_coefficients.items())
    {
        // For each instruction
        for (const auto& instruction : p_category["Instructions"])
        {
            // If the category has the same name as the instruction then
            // throw an exception.
            if (search_category.key() == instruction)
            {
                throw std::ios_base::failure(
                    "Each instruction in the Coefficients file must have "
                    "only one set of Coefficients associated with it. " +
                    instruction.dump() +
                    " was used as category name and also under the "
                    "'Instructions' tag in the "
                    "category: \"" +
                    p_category_key + "\"");
            }
        }
    }
}

//! @brief Ensures that no instruction is contained within multiple
//! categories.
//! @param p_category The instruction category to be validated.
//! @param p_category_key The heading of the category contained within
//! p_category.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an instruction was found to have
//! more than one set of coefficients associated with it.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Instructions_Unique(const nlohmann::json& p_category,
                                          const std::string& p_category_key,
                                          const nlohmann::json& p_coefficients)
{
    // Check against every other category
    for (const auto& search_category : p_coefficients.items())
    {
        // Don't search the category it was originally found in.
        if (p_category_key == search_category.key())
        {
            continue;
        }

        // For each instruction in the category
        for (const auto& instruction : p_category["Instructions"])
        {
            // If the Instructions heading is not present there is
            // nothing
            // to check.
            if (search_category.value().find("Instructions") ==
                search_category.value().end())
            {
                continue;
            }
            // If the instruction is not the search category, there is
            // no
            // problem
            if (std::find(search_category.value().at("Instructions").begin(),
                          search_category.value().at("Instructions").end(),
                          instruction.get<std::string>()) ==
                search_category.value().at("Instructions").end())
            {
                continue;
            }
            // If the above checks fail then throw an exception
            throw std::ios_base::failure(
                "Each instruction in the Coefficients file must have "
                "only one set of Coefficients associated with it. Found: " +
                instruction.dump() + " in: '" + p_category_key + "'" +
                p_category.at("Instructions").dump() + "' and also in: '" +
                search_category.key() + "'" +
                search_category.value().at("Instructions").dump());
        }
    }
}
}  // namespace Internal
}  // namespace ELMO2
