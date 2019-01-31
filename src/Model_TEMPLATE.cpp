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
    @file Model_TEMPLATE.hpp
    @brief This file is a template. This can be used as a base for implementing
    a mathematical model for generating traces of a given target program.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include "Model_TEMPLATE.hpp"

#include <vector>  // for vector

//! The list of interaction terms used by this model in order to generate
//! traces.
const std::unordered_set<std::string>
    ELMO2::Internal::Model_TEMPLATE::m_required_interaction_terms = {
        "Bit_Flip1",
        "Bit_Flip1_Bit_Interactions",
        "Bit_Flip2",
        "Bit_Flip2_Bit_Interactions",
        "Hamming_Distance_Operand1_Previous_Instruction",
        "Hamming_Distance_Operand1_Subsequent_Instruction",
        "Hamming_Distance_Operand2_Previous_Instruction",
        "Hamming_Distance_Operand2_Subsequent_Instruction",
        "Hamming_Weight_Operand1_Previous_Instruction",
        "Hamming_Weight_Operand1_Subsequent_Instruction",
        "Hamming_Weight_Operand2_Previous_Instruction",
        "Hamming_Weight_Operand2_Subsequent_Instruction",
        "Operand1",
        "Operand1_Bit_Interactions",
        "Operand2",
        "Operand2_Bit_Interactions",
        "Previous_Instruction",
        "Subsequent_Instruction"};

//! @brief This function contains the mathematical calculations that generate
//! the Traces.
//! @returns The generated Traces for the target program.
const std::vector<float>
ELMO2::Internal::Model_TEMPLATE::Generate_Traces() const
{
    // *** Place your code here ***
    //! @note m_coefficients and m_execution can be made use of to generate the
    //! traces.
    return std::make_pair(
        "", std::vector<float>());  // temp placeholder for debugging reasons
}
