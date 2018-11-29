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
    @file Model_Power.hpp
    @brief TODO: Add this.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include "Model_Power.hpp"

#include <cstdint>  // for size_t
#include <deque>    // for deque
#include <queue>    // for queue
#include <utility>  // for pair, make_pair
#include <vector>   // for vector

//! The list of interaction terms used by this model in order to generate
//! traces.
const std::unordered_set<std::string>
    ELMO2::Internal::Model_Power::m_required_interaction_terms = {
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
const std::vector<float> ELMO2::Internal::Model_Power::Generate_Traces() const
{
    //! @todo : Document
    //! A sliding window that stores information about the previous, current and
    //! next instructions.
    // Populate with the first two instructions, ready to be used as the
    // previous and current instruction in calculations.
    // This is done in advance as the loop below only adds the next instruction.
    std::deque<ELMO2::Internal::Model_Power::Assembly_Instruction_Power>
        instructions_window(
            {get_instruction_terms(0), get_instruction_terms(1)});

    /*
     *std::cout << instructions_window.front().Get_Opcode() << std::endl;
     *std::cout << instructions_window[1].Get_Opcode() << std::endl;
     */

    //! @todo : Document
    // The interactions between the instructions stored in instructions_window.
    /*
     *std::queue<ELMO2::Internal::Model_Power::Instruction_Terms_Interactions>
     *    instruction_interactions_window(
     *        {ELMO2::Internal::Model_Power::Instruction_Terms_Interactions(
     *            instructions_window.front(),
     *            instructions_window[1].Operand_1)});
     */

    std::vector<float> traces;

    // Start at 1 as we are taking the previous cycle into account in the
    // calculations.
    // Finish at size - 1 As we are taking the next cycle into account in the
    // calculations.
    for (std::size_t i = 1; i < m_execution.Get_Cycle_Count() - 1; ++i)
    {
        // TODO: Add a special case for when i = 0 and i = last (and i = 1?/
        // last-1?).

        // Add the next set of operands.
        instructions_window.emplace_back(get_instruction_terms(i + 1));

        // Add the next set of cross instruction interactions.
        /*
         *instruction_interactions_window.emplace(
         *    ELMO2::Internal::Model_Power::Instruction_Terms_Interactions(
         *        instructions_window[1],
         *instructions_window.back().Operand_1));
         */

        // std::cout << instruction_interactions_window.size() << std::endl;

        // Just to simplify the syntax
        // static const auto& previous_instruction =
        // instructions_window.front();
        // TODO: Replace with something that isn't a macro
#define previous_instruction instructions_window.front()

        // static const auto& current_instruction = instructions_window[1];
        // TODO: Replace with something that isn't a macro
#define current_instruction instructions_window[1]

        // TODO: Can/ Should this be made constexpr?
        // static const auto& next_instruction = instructions_window.back();
        // TODO: Replace with something that isn't a macro
#define next_instruction instructions_window.back()

        // TODO: Delete this?
        // TODO: Can/ Should this be made constexpr?
        const auto& current_opcode  = current_instruction.Get_Opcode();
        const auto& previous_opcode = previous_instruction.Get_Opcode();
        // static const auto& next_opcode     = next_instruction.Get_Opcode();

        /*
         *const auto previous_instruction_data =
         *    instructions_window.front() *
         *    m_coefficients.Get_Coefficients(
         *        m_execution.Get_Instruction(p_cycle,
         *"Execute").Get_Opcode());
         */

        // TODO: Bit flip 1 and 2 bit interactions is always 0 in the coeffs
        // file. Can these be removed? Is this a bug when turning into json?
        // Operand 1 and 2 bit interactions are only non 0 for muls and
        // stores

        /*
         *        // TODO: How does this work when the bit flip is based on 2
         *        // instructions but the opcode isn't?
         *        const auto bit_flip_1 =
         *            calculate_term(current_opcode,
         *                           "Bitflip1",
         *                           previous_instruction.Operand_1_Bit_Flip);
         *
         *        // TODO: How does this work when the bit flip is based on 2
         *        // instructions but the opcode isn't?
         *        const auto bit_flip_2 =
         *            calculate_term(current_opcode,
         *                           "Bitflip2",
         *                           previous_instruction.Operand_2_Bit_Flip);
         */

        const auto operand_1 = calculate_term(
            current_opcode, "Operand1", current_instruction.Operand_1);

        const auto operand_2 = calculate_term(
            current_opcode, "Operand2", current_instruction.Operand_2);

        // Linear regression means that nothing is done for ALU
        // 0 is shifts
        // 1 is stores
        // 2 is loads
        // 3 is multiply
        const auto previous_instruction_term =
            Get_Coefficient(current_opcode,
                            "Previous_Instruction",
                            Get_Instruction_Category(previous_opcode));

        const auto subsequent_instruction_term =
            // TODO: Replace this with a function?
            Get_Coefficient(current_opcode,
                            "Subsequent_Instruction",
                            Get_Instruction_Category(previous_opcode));

        const auto hamming_weight_previous_operand_1 =
            calculate_hamming_weight(current_instruction,
                                     1,
                                     Instruction::Previous,
                                     Get_Instruction_Category(previous_opcode));

        const auto hamming_weight_subsequent_operand_1 =
            calculate_hamming_weight(current_instruction,
                                     1,
                                     Instruction::Subsequent,
                                     Get_Instruction_Category(previous_opcode));

        const auto hamming_weight_previous_operand_2 =
            calculate_hamming_weight(current_instruction,
                                     2,
                                     Instruction::Previous,
                                     Get_Instruction_Category(previous_opcode));

        const auto hamming_weight_subsequent_operand_2 =
            calculate_hamming_weight(current_instruction,
                                     2,
                                     Instruction::Subsequent,
                                     Get_Instruction_Category(previous_opcode));

        // TODO: Replace this with a function.
        const auto hamming_distance_previous_operand_1 =
            Get_Coefficient(current_opcode,
                            "Hamming_Distance_Operand1_Previous_Instruction",
                            Get_Instruction_Category(previous_opcode)) *
            hamming_distance(current_instruction.Operand_1,
                             previous_instruction.Operand_1);

        const auto hamming_distance_subsequent_operand_1 =
            Get_Coefficient(current_opcode,
                            "Hamming_Distance_Operand1_Subsequent_Instruction",
                            Get_Instruction_Category(previous_opcode)) *
            hamming_distance(current_instruction.Operand_1,
                             next_instruction.Operand_1);

        const auto hamming_distance_previous_operand_2 =
            Get_Coefficient(current_opcode,
                            "Hamming_Distance_Operand2_Previous_Instruction",
                            Get_Instruction_Category(previous_opcode)) *
            hamming_distance(current_instruction.Operand_2,
                             previous_instruction.Operand_2);

        const auto hamming_distance_subsequent_operand_2 =
            Get_Coefficient(current_opcode,
                            "Hamming_Distance_Operand2_Subsequent_Instruction",
                            Get_Instruction_Category(previous_opcode)) *
            hamming_distance(current_instruction.Operand_2,
                             next_instruction.Operand_2);

        const auto constant = Get_Constant(current_opcode);

        // clang-format off
        traces.emplace_back(constant *
                            previous_instruction_term +
                            subsequent_instruction_term +
                            operand_1 +
                            operand_2 +
                            hamming_weight_previous_operand_1 +
                            hamming_weight_previous_operand_2 +
                            hamming_weight_subsequent_operand_1 +
                            hamming_weight_subsequent_operand_2);
        // clang-format on

        std::cout << constant * previous_instruction_term +
                         subsequent_instruction_term + operand_1 + operand_2 +
                         hamming_weight_previous_operand_1 +
                         hamming_weight_previous_operand_2 +
                         hamming_weight_subsequent_operand_1 +
                         hamming_weight_subsequent_operand_2;

        std::cout << ' ' << current_instruction.Get_Opcode() << std::endl;

        // Get rid of the now unneeded instruction 2 cycles before.
        instructions_window.pop_front();
    }
    return traces;
}