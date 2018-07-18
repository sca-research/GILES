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
    @todo Change this
    @file Model_Hamming_Weight.hpp
    @brief This file is a template. This can be used as a base for implementing
    a mathematical model for generating traces of a given target program.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include "Assembly_Instruction.hpp"  // for Assembly_Instruction
#include "Execution.hpp"             // for Execution
#include <bits/stdint-uintn.h>       // for uint8_t
#include <bitset>                    // for bitset
#include <vector>                    // for vector

#include <iostream>  // for temp debugging

#include "Model_Hamming_Weight.hpp"

//! The list of interaction terms used by this model in order to generate
//! traces.
const std::unordered_set<std::string>
    ELMO2::Internal::Model_Hamming_Weight::m_required_interaction_terms = {};

//! @brief This function contains the mathematical calculations that generate
//! the Traces.
//! @returns The generated Traces for the target program.
const ELMO2::Internal::Traces
ELMO2::Internal::Model_Hamming_Weight::Generate_Traces() const
{
    // TODO: If traces are serialised upon generation then do we need a
    // traces object?
    ELMO2::Internal::Traces traces;

    for (size_t i = 0; i < m_execution.Get_Cycle_Count(); ++i)
    {
        // Prevents trying to calculate the hamming weight of stalls and
        // flushes.
        if (ELMO2::Internal::Execution::State::Normal !=
            m_execution.Get_State(i, "Execute"))
        {
            std::cout << "Abnormal state reached" << std::endl;
            continue;
        }

        std::cout << m_execution.Get_Value<std::string>(i, "Execute")
                  << std::endl;

        // Retrieves what is in the "Execute" pipeline stage at clock cycle "i".
        auto instruction = m_execution.Get_Instruction(i, "Execute");

        // auto operand_value = instruction.Get_Operand(operand_number);

        std::cout << instruction.Get_Opcode();

        for (const auto& operand : instruction.Get_Operands())
        {
            // If the operand is a register look up the value in that register,
            // else treat it as a raw value.
            if (!m_execution.Is_Register(operand))
            {
                std::cout << "," << operand;
                continue;
            }
            std::cout << ",|" << std::dec
                      << ELMO2::Internal::Model_Hamming_Weight::Hamming_Weight<
                             size_t>(m_execution.Get_Register_Value(i, operand))
                      << "|";
        }
        std::cout << std::endl;

        // Calculates the Hamming weight of the second operand and appends
        // it to the traces object
        // traces.append(instruction.get_operand(2).count());  // TODO: Add
        // a function that actually makes this work to Assembly_Instruction.
    }
    return traces;
}
