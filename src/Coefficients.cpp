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
    @file Coefficients.hpp
    @brief Contains the internal representation of the Coefficients file.
    @author Scott Egerton
    @date 2017
    @copyright GNU Affero General Public License Version 3+
*/

#include <algorithm> // for find
#include <stdexcept> // for out_of_range

#include "Coefficients.hpp"

namespace ELMO2
{
namespace Internal
{
//! @brief Retrives the catergory that the given instruction is contained
//! within. This is a utility function that is used to assist in retriving other
//! data from the coefficients.
//! @param p_opcode The opcode of the instruction for which the catergory will
//! be retrived.
//! @returns The name of the catergory that the instruction is contained within.
//! If the coefficients are not catergoried then the instruction opcode is
//! returned.
//! @exception std::out_of_range This exception is thrown if the instruction
//! given by p_opcode is not found within any catergory in the coefficients.
//! @see https://eprint.iacr.org/2016/517 Section 4.2 for more on the
//! catergories.
const std::string ELMO2::Internal::Coefficients::get_instruction_catergory(
    const std::string& p_opcode) const
{
    // For each instruction catergory
    for (const auto& catergory : m_coefficients)
    {
        try
        {
            // If the instruction is in this catergory.
            if (std::find(catergory.at("Instructions").begin(),
                          catergory.at("Instructions").end(),
                          p_opcode) != catergory.at("Instructions").end())
            {
                // Return the name of the catergory as a string
                return catergory; // TODO: test this this returns
                                  // what it's supposed to
                                  // return.
            }
        }
        // It's okay for an exception to be thrown here, the Instruction
        // Catergories are optional. If there are no catergories then the
        // 'catergory' is simply the opcode
        catch (nlohmann::json::out_of_range&)
        {
            if (catergory.get<std::string>() == p_opcode)
            {
                return p_opcode;
            }
        }
    }
    throw std::out_of_range("This intruction (" + p_opcode +
                            ") was not found within the Coefficients");
}

//! @brief Retrives a list of all interaction terms contained within the
//! coefficients. This is needed in order to ensure the Model will be
//! provided with the terms it requires. The interaction terms of the first
//! coefficient are used as they should be identical to all other
//! coefficients.
//! @returns All interaction terms contained, as an unordered set. An
//! unordered set is used as the json standard dictates that json data is
//! unordered therefore we should not expect any particular order.
const std::unordered_set<std::string>
ELMO2::Internal::Coefficients::Get_Interaction_Terms() const
{
    return m_coefficients[0]["Coefficients"]
        .get<std::unordered_set<std::string>>();
}

//! @brief Retrives the coefficients for the interaction term given by
//! p_interaction_term under the instruction catergory that contains the
//! instruction given by p_opcode.
//! @param p_opcode  The opcode of the instruction that the Coefficients are
//! required for.
//! @param p_interaction_term The interaction term from within the model
//! that the Coefficients are required for.
//! @returns An ordered vector of the coefficient values.
//! @exception nholmann::json::out_of_range This is thrown when the iteraction
//! term given by p_interaction_term is not found.
const std::vector<double> ELMO2::Internal::Coefficients::Get_Coefficients(
    const std::string& p_opcode, const std::string& p_interaction_term) const
{
    return m_coefficients
        .at(get_instruction_catergory(p_opcode))["Coefficients"]
        .at(p_interaction_term)
        .get<std::vector<double>>();
}

//! @brief Retrives the Constant for the Instruction Catergory that contains
//! the instruction provided by p_opcode.
//! @param p_opcode The opcode of the instruction that the Constant is
//! required for.
//! @returns The value of the constant for the catergory that the
//! instruction belongs to.
double
ELMO2::Internal::Coefficients::Get_Constant(const std::string& p_opcode) const
{
    return m_coefficients.at(get_instruction_catergory(p_opcode))["Constant"]
        .get<double>();
}
} // namespace Internal
} // namespace ELMO2
