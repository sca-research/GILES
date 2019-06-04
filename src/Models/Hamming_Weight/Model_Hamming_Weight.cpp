/*
    This file is part of GILES.

    GILES is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GILES is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with GILES.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @todo Change this
    @file Model_Hamming_Weight.hpp
    @brief This file is a template. This can be used as a base for implementing
    a mathematical model for generating traces of a given target program.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <cstddef>  // for uint8_t, size_t
#include <vector>   // for vector

#include "Model_Hamming_Weight.hpp"

#include "Assembly_Instruction.hpp"  // for Assembly_Instruction
#include "Execution.hpp"             // for Execution

//! The list of interaction terms used by this model in order to generate
//! traces.
const std::unordered_set<std::string>
    GILES::Internal::Model_Hamming_Weight::m_required_interaction_terms{};

//! @brief This function contains the mathematical calculations that generate
//! the Traces.
//! @returns The generated Traces for the target program.
const std::vector<float>
GILES::Internal::Model_Hamming_Weight::Generate_Traces() const
{
    std::vector<float> traces;

    const std::size_t number_of_cycles{m_execution.Get_Cycle_Count()};
    for (std::size_t i{0}; i < number_of_cycles; ++i)
    {
        // Prevents trying to calculate the hamming weight of stalls and
        // flushes.
        if (!m_execution.Is_Normal_State(i, "Execute"))
        {
            // In the case of stalls and flushes just assume these use no power
            // for now.
            traces.push_back(0);
            continue;
        }

        // Calculates the Hamming weight of the first operand of the instruction
        // at clock cycle 'i' and appends it to the traces object.
        traces.push_back(
            Model_Math::Hamming_Weight(m_execution.Get_Operand_Value(
                i, m_execution.Get_Instruction(i, "Execute"), 1)));
    }
    return traces;
}
