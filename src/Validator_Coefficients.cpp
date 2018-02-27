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
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <algorithm> // for find
#include <iostream>  // for ios_base::failure, ios_base
#include <map>       // for map<>::key_type
#include <string>    // for basic_string, string

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
    Validate_Not_Empty(p_coefficients);
    Validate_Interaction_Terms_Not_Empty(
        p_coefficients); // TODO: Maybe call this later?

    // Check each instruction category
    for (const auto& category : p_coefficients)
    {
        Validate_Category_Not_Empty(category);
        Validate_Category_Headings_Constant(category);
        Validate_Category_Headings_Coefficients(category);
        Validate_Category_Interaction_Terms_Not_Empty(category);
        Validate_Category_Instructions_Not_Empty(category);
        Validate_Category_Correct_Interaction_Terms(category, p_coefficients);
        Validate_Category_Interaction_Terms_Size(category, p_coefficients);
        Validate_Category_Instructions_Unique(category, p_coefficients);
    }
}

//! @brief Ensures that the given JSON is not an empty structure.
//! @param p_coefficients The coefficients to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the JSON is an empty structure.
void ELMO2::Internal::Validator_Coefficients::Validate_Not_Empty(
    const nlohmann::json& p_coefficients)
{
    // JSON must not be empty.
    if (p_coefficients.empty())
    {
        throw std::ios_base::failure("Coefficients file must not be empty.");
    }
}

//! @brief Ensures that the first category in the given JSON contains at least
//! one interaction term to be used. Otherwise there will be no actual
//! coefficient values in the file.
//! The first category is used as all categories should be the same. This is
//! checked in a separate validation rule.
//! @param p_coefficients The coefficients to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the coefficients contain no interaction
//! terms and therefore no coefficient values.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Interaction_Terms_Not_Empty(const nlohmann::json& p_coefficients)
{
    // There must be at least one interaction term
    if (p_coefficients.front().at("Coefficients").empty())
    {
        throw std::ios_base::failure("There must be at least one interaction "
                                     "term in the Coefficients file.");
    }
}

// TODO: Merge this with other not empty function? - Maybe do this when adding
// more to exception messages.

//! @brief Ensures that the given category is not an empty structure.
//! @param p_category The instruction category to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the category is an empty structure.
void ELMO2::Internal::Validator_Coefficients::Validate_Category_Not_Empty(
    const nlohmann::json& p_category)
{
    // No category can be empty
    if (p_category.empty())
    {
        throw std::ios_base::failure("Coefficients file must not contain "
                                     "empty coefficient categories.");
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

//! @brief Ensures that the given category contains the same interaction terms
//! as every other category. The Model will expect to find the same interaction
//! terms for each category.
//! The first category is used to check against as all categories should be
//! the same.
//! @param p_category The instruction category to be validated.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, the correct interaction terms were not
//! found.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Correct_Interaction_Terms(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients)
{
    // check each interaction term individually.
    for (nlohmann::json::const_iterator interaction_term =
             p_category["Coefficients"].begin();
         interaction_term != p_category["Coefficients"].end();
         ++interaction_term)
    {
        if (p_coefficients.front()
                .at("Coefficients")
                .find(interaction_term.key()) ==
            p_coefficients.front().at("Coefficients").end())
        {
            throw std::ios_base::failure(
                "The same interaction terms must be provided for all "
                "categories in the Coefficients file.");
        }
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
    Validate_Category_Instructions_Not_Empty(const nlohmann::json& p_category)
{
    // If the "Instructions" tag is present, it should not be empty
    if (p_category.find("Instructions") != p_category.end() &&
        p_category.at("Instructions").empty())
    {
        throw std::ios_base::failure(
            "Categories in the Coefficients file must not contain "
            "an "
            "empty list of instructions.");
    }
}

//! @brief Ensures that each of the interaction terms within the given category
//! is not an empty structure.
//! @param p_category The instruction category to be validated.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an interaction term was found to not
//! contain any values.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Interaction_Terms_Not_Empty(
        const nlohmann::json& p_category)
{
    // There must be at least one value for each interaction term.
    for (const auto& interaction_term : p_category["Coefficients"])
    {
        if (interaction_term.empty())
        {
            throw std::ios_base::failure(
                "Each interaction term in the Coefficients file must "
                "contain at least one value.");
        }
    }
}

//! @brief Ensures that each of the interaction terms within the given category
//! contains the same amount of values.
//! The first category is used to check against as all categories should be
//! the same.
//! @param p_category The instruction category to be validated.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an interaction term does not contain
//! the correct number of values.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Interaction_Terms_Size(
        const nlohmann::json& p_category, const nlohmann::json& p_coefficients)
{
    for (const auto& interaction_term :
         nlohmann::json::iterator_wrapper(p_category["Coefficients"]))
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

//! @brief Ensures that no instruction is contained within multiple
//! categories. In the case that an instruction has the same name as a
//! category. This is also regarded as invalid as the same name can refer to
//! two different sets of coefficients. The first category is used to check
//! against as all categories should be the same.
//! @param p_category The instruction category to be validated.
//! @param p_coefficients The coefficients, in order to compare against the
//! first category.
//! @exception std::ios_base::failure This is thrown in the case that this
//! validation rule fails. In this case, an instruction was found to have
//! more than one set of coefficients associated with it.
void ELMO2::Internal::Validator_Coefficients::
    Validate_Category_Instructions_Unique(const nlohmann::json& p_category,
                                          const nlohmann::json& p_coefficients)
{
    // Instructions should not be in multiple categories.
    // For each instruction in that category
    for (const auto& instruction : p_category["Instructions"])
    {
        // Check if it's in every other instruction category
        for (const auto& search_category : p_coefficients)
        {
            // Don't search the category it was originally found in.
            if (p_category == search_category)
            {
                continue;
            }

            // If the category has the same name as the instruction or
            // If the category has a list of instructions in it and
            // instruction is in this list.
            if (search_category == instruction ||
                (search_category.find("Instructions") !=
                     search_category.end() &&
                 std::find(search_category.at("Instructions").begin(),
                           search_category.at("Instructions").end(),
                           instruction.get<std::string>()) !=
                     search_category.at("Instructions").end()))
            {
                throw std::ios_base::failure(
                    "Each instruction in the Coefficients file  must have "
                    "only one set of Coefficients associated with it.");
            }
        }
    }
}
} // namespace Internal
} // namespace ELMO2
