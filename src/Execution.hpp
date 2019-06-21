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
    @file Execution.hpp
    @brief This file contains the internal representation of the Execution of a
    program.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <algorithm>  // for min
#include <any>        // for any, any_cast, bad_any_cast
#include <deque>      // for deque
#include <map>        // for map
#include <memory>     // for shared_ptr
#include <stdexcept>  // for range_error
#include <string>     // for string
#include <vector>     // for vector

#include <boost/algorithm/string.hpp>  // TODO: Convert Uility.h over to boost algorithms (or the other way around?)

#include "Assembly_Instruction.hpp"
#include "Error.hpp"    // for Report_Error
#include "Utility.hpp"  // for string_split

#include <iostream>  // for temp debugging

namespace GILES
{
namespace Internal
{
//! @class Execution
//! @todo Change this
//! @brief The internal representation of the Execution of a program. This
//! contains the state of every pipeline stage during every clock cycle as well
//! as the state of every Register before the Execution started
//! @see https://en.wikipedia.org/wiki/Instruction_pipelining
//! @see https://en.wikipedia.org/wiki/Clock_cycle
class Execution
{
public:
    //! The states that a processor pipeline stage can be in. These are needed
    //! to be stored as if the pipeline stage is not running smoothly (i.e.
    //! Normal state) then it could prevent leakage from being calculated in a
    //! normal fashion.
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    //! @see https://en.wikipedia.org/wiki/Pipeline_flush
    enum class State
    {
        Normal,
        Stalled,
        Flushing
    };

private:
    //! @brief A data structure for storing the per clock cycle pipeline of any
    //! given processor. The values are indexed by clock cycle and then by
    //! pipeline stage. For each clock cycle (stored as a vector) there will be
    //! a map storing the value for that pipeline stage, during that cycle, as
    //! indexed by the name of the pipeline stage.
    //! clock cycle no<pipe stage<pipe_name, <value state, value>>>
    //! e.g. 20<<"Decode", <Normal, <"str r1, r2">>>>
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @note Ideally this would contain unique_ptr objects instead of
    //! shared_ptr however there were issues implementing this due to the
    //! containers. Resizing of the vector caused the copy constructor to be
    //! called on unique_ptr causing a compile error.
    // TODO: const correctness
    std::vector<std::map<const std::string, std::any>> m_pipeline;

    // TODO: const correctness
    //! The state of the processor registers during each cycle of the execution
    //! of the target program.
    //! @see https://en.wikipedia.org/wiki/Processor_register
    std::vector<std::map<std::string, std::size_t>> m_registers;

    //! @brief Retrieves the type of state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle. This is
    //! different from retrieving the value as this will return an enum
    //! representing the type of value. If the pipeline stage was behaving in a
    //! regular fashion at the given clock cycle, 'Normal' will be returned.
    //! Other states such as 'Stalled' may also be returned.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns The requested type of state using the State enum.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    GILES::Internal::Execution::State
    get_state(const uint32_t p_cycle,
              const std::string& p_pipeline_stage_name) const
    {
        try
        {
            // TODO: If entire program is outside of trigger points then this
            // will crash (size=0 out of range)
            return std::any_cast<State>(
                m_pipeline.at(p_cycle).at(p_pipeline_stage_name));
        }
        catch (const std::bad_any_cast&)
        {
            // if the cast failed then it is not a state and instead a value
            // therefore the state is implicitly normal.
            return State::Normal;
        }
    }

public:
    //! @brief The constructor for the Execution class.
    //! This initialises the pipeline and the registers to be of the size
    //! given by the parameter p_number_of_cycles, so that a value can be
    //! stored for each clock cycle.
    //! @param p_number_of_cycles The number of clock cycles that occurred
    //! during the target programs execution.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Processor_register
    explicit Execution(const std::size_t p_number_of_cycles)
        : m_pipeline(p_number_of_cycles), m_registers(p_number_of_cycles)
    {
    }

    //! @brief This allows for adding an entire pre recorded pipeline stage
    //! at once. This pre recorded pipeline stage should be provided by the
    //! simulator and contain the per clock cycle information related to the
    //! state that this pipeline stage was in throughout every clock cycle
    //! during the execution of the target program. This function makes use
    //! of templates as the data type in which one pipeline stage is stored
    //! may differ from another. Additionally this allows of setting the State
    //! of individual clock cycles to other values such as Stalled.
    //! @throws std::range_error TODO: This is thrown when the length of the
    //! new pipeline stage does not match the length of the existing
    //! pipeline stages.
    //! @param p_pipeline_stage_name The name of the pipeline stage to be
    //! added. e.g. "Fetch".
    //! @param p_pipeline_stage A vector containing all of the per clock
    //! cycle states of that pipeline stage. These are indexed by the number
    //! of clock clock cycles that have passed.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    template <class T_Value_Type>
    void Add_Pipeline_Stage(const std::string& p_pipeline_stage_name,
                            const std::vector<T_Value_Type>& p_pipeline_stage)
    {
        // Ensure all pipeline stages are the same length
        // TODO: Try not to enforce all values to be assigned/known
        // by the time the constructor is called as different simulators may
        // call for a more dynamic approach.
        // TODO: Make this work with Andres' simulator.
        /*
         *if (p_pipeline_stage.size() != m_pipeline.size())
         *{
         *    throw std::range_error(
         *        "All pipeline stages must be of the same length");
         *}
         */

        // std::min to account for different length pipeline stages.
        // Pipeline stages should all be the same length but may not be in the
        // case of errors.
        const std::size_t size{
            std::min(p_pipeline_stage.size(), m_pipeline.size())};

        // Transpose the vector into m_pipeline.
        for (std::size_t cycle = 0; cycle < size; ++cycle)
        {
            // Add it to m_pipeline.
            m_pipeline[cycle][p_pipeline_stage_name] = p_pipeline_stage[cycle];
        }
    }

    //! @brief This allows for storing an individual value representing a
    //! pipeline stage during a specific clock cycle. This pre recorded pipeline
    //! stage should be provided by the simulator and contain the per clock
    //! cycle information related to the state that this pipeline stage was in
    //! throughout the clock cycle given by p_cycle during the execution of the
    //! target program. This function makes use of templates as the data type in
    //! which one pipeline stage is stored may differ from another. Additionally
    //! this allows of setting the State of individual clock cycles to other
    //! values such as Stalled.
    //! @param p_cycle The clock cycle at which to add the value.
    //! @param p_pipeline_stage_name The name of the pipeline stage to be
    //! added. e.g. "Fetch".
    //! @param p_value A the state of that pipeline stage during the given clock
    //! cycle.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    template <class T_Value_Type>
    void Add_Value(const std::size_t p_cycle,
                   const std::string& p_pipeline_stage_name,
                   const T_Value_Type p_value)
    {
        // Add it to m_pipeline.
        m_pipeline[p_cycle][p_pipeline_stage_name] = p_value;
    }

    //! @brief Retrieves the state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle. This is a
    //! template function. The template type specifies which type you which
    //! to receive. The type stored will be dynamically cast into the
    //! requested type where possible. When this is not possible, an
    //! exception will be thrown.
    //! @throws std::bad_any_cast This is thrown when dynamically casting the
    //! requested value to the requested type fails.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns The requested state in the type given by the template
    //! T_Value_Type.
    //! @todo Is it worth having a function to retrieve the entire pipeline
    //! stage in one go?
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    template <typename T_Value_Type>
    const T_Value_Type Get_Value(const uint32_t p_cycle,
                                 const std::string& p_pipeline_stage_name) const
    {
        try
        {
            return std::any_cast<T_Value_Type>(
                m_pipeline.at(p_cycle).at(p_pipeline_stage_name));
        }
        catch (const std::bad_any_cast&)
        {
            throw std::invalid_argument("The requested pipeline state is "
                                        "not stored as the requested type");
        }
    }

    //! @brief Retrieves the type of state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle. This is
    //! different from retrieving the value as this will return an enum
    //! representing the type of value. If the pipeline stage was behaving in a
    //! regular fashion at the given clock cycle, 'Normal' will be returned.
    //! Other states such as 'Stalled' may also be returned.
    //! In the case that there is nothing in the pipeline stage at the given
    //! clock cycle, then the stage "Stalled" will be returned. This is marked
    //! as unsafe as it will hide out of bounds access.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns The requested type of state using the State enum.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    State Get_State_Unsafe(const uint32_t p_cycle,
                           const std::string& p_pipeline_stage_name) const
    {
        try
        {
            return get_state(p_cycle, p_pipeline_stage_name);
        }
        catch (const std::out_of_range&)
        {
            return State::Stalled;
        }
    }

    //! @brief Retrieves the type of state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle. This is
    //! different from retrieving the value as this will return an enum
    //! representing the type of value. If the pipeline stage was behaving in a
    //! regular fashion at the given clock cycle, 'Normal' will be returned.
    //! Other states such as 'Stalled' may also be returned.
    //! In the case that there is nothing in the pipeline stage at the given
    //! clock cycle, then an out of bounds error message will be printed and the
    //! program will exit.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns The requested type of state using the State enum.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    State Get_State(const uint32_t p_cycle,
                    const std::string& p_pipeline_stage_name) const
    {
        try
        {
            return get_state(p_cycle, p_pipeline_stage_name);
        }
        catch (const std::out_of_range&)
        {
            GILES::Internal::Error::Report_Error(
                "Could not find a value in the pipeline stage \"{}\" "
                "during clock cycle {}",
                p_pipeline_stage_name,
                p_cycle);
        }
    }

    //! @brief Checks to see if the type of state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle is Normal.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns Returns true if the state is normal, false if not.
    //! @returns The requested type of state using the State enum.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    bool Is_Normal_State(const std::uint32_t p_cycle,
                         const std::string& p_pipeline_stage_name) const
    {
        return GILES::Internal::Execution::State::Normal ==
               Get_State(p_cycle, p_pipeline_stage_name);
    }

    //! @brief Checks to see if the type of state of the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle is Normal.
    //! This is marked an unsafe because it will not report an error in the case
    //! of out of bounds access, it will return false instead.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns Returns true if the state is normal, false if not.
    //! @returns The requested type of state using the State enum.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @see https://en.wikipedia.org/wiki/Pipeline_stall
    bool Is_Normal_State_Unsafe(const std::uint32_t p_cycle,
                                const std::string& p_pipeline_stage_name) const
    {
        return GILES::Internal::Execution::State::Normal ==
               Get_State_Unsafe(p_cycle, p_pipeline_stage_name);
    }

    //! @brief Retrieves the instruction in the pipeline stage given by
    //! p_pipeline_stage_name at the clock cycle given by p_cycle.
    //! @warning This function does not check the type of the Value before
    //! attempting to turn it into an assembly instruction. This should be
    //! done separately; Get_State() can help with this.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_pipeline_stage_name The pipeline stage from which to
    //! retrieve the state.
    //! @returns The requested instruction.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    // TODO: Settle on using boost for all string functions or not using
    // boost.
    const GILES::Internal::Assembly_Instruction
    Get_Instruction(const uint32_t p_cycle,
                    const std::string& p_pipeline_stage_name) const
    {
        // Get the instruction as a string
        std::string instruction =
            Get_Value<std::string>(p_cycle, p_pipeline_stage_name);

        // Remove the opcode from the instruction and store it in a separate
        // variable.
        // TODO: Some instructions don't have operands, this fails with those.
        std::string opcode =
            GILES::Internal::Utility::string_split_head_pop(&instruction, " ");

        // Remove white space
        boost::algorithm::trim(instruction);

        // Convert the rest of the instruction into a list of operands
        std::vector<std::string> operands =
            GILES::Internal::Utility::string_split(instruction, ",");

        /*
         *std::vector<std::uint32_t> operand_values;
         *std::transform(operands.begin(),
         *               operands.end(),
         *               operand_values.begin(),
         *               [&](const auto p_operand) {
         *                   return Get_Operand_Value(p_cycle, p_operand);
         *               });
         */

        // Create a new instruction and return it.
        return Assembly_Instruction(opcode, operands /*, operand_values*/);
    }

    //! @brief Adds the state of all registers as they were during every clock
    //! cycle.
    //! @param p_registers A vector of a map of registers. The vector indicates
    //! he current clock cycle number and the map contains all registers for one
    //! cycle. The value in those registers is stored indexed by the register's
    //! name.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @todo Validate that the same set of registers are given for each cycle?
    void Add_Registers_All(
        const std::vector<std::map<std::string, std::size_t>> p_registers)
    {
        m_registers = p_registers;
    }

    //! @brief Adds the state of all registers as they were during the clock
    //! cycle indicated by p_cycle.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @param p_registers A map of registers. The value in those registers is
    //! stored indexed by the register's name.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @todo Validate that the same set of registers are given for each cycle?
    void
    Add_Registers_Cycle(const std::size_t p_cycle,
                        const std::map<std::string, std::size_t>& p_registers)
    {
        m_registers[p_cycle] = p_registers;
    }

    //! @brief Checks whether or not a value is the name of a register by
    //! checking if that register is present during the first clock cycle. This
    //! is used to check whether or not operands are registers.
    //! @param p_value The value to be checked.
    //! @returns Returns true if p_value is the name of a register. Returns
    //! false if it is not.
    bool Is_Register(const std::string& p_value) const
    {
        return m_registers[0].end() != m_registers[0].find(p_value);
    }

    //! @brief Get the state of the registers as they were after the number
    //! of clock cycles given by p_cycle have passed.
    //! @param p_cycle The cycle number at which to retrieve the registers
    //! from.
    //! @returns The registers as they were during that cycle.
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    const std::map<std::string, std::size_t>&
    Get_Registers(const std::size_t p_cycle) const
    {
        return m_registers.at(p_cycle);
    }

    //! @todo document
    //! @note Offsets do not need to be considered as they offset the loaded
    //! value, not the address.
    std::size_t Get_Register_Value(const std::size_t p_cycle,
                                   const std::string& p_register_name) const
    {
        return m_registers.at(p_cycle).at(p_register_name);
    }

    //! @brief Retrieves the value of an operand in numerical form. If that
    //! operand is a register then the value contained within that register is
    //! retrieved instead
    //! @param p_cycle The cycle number at which to retrieve the operand
    //! from.
    //! @param p_operand The operand to retrieve the value from.
    //! @ returns The value stored in the operand, or the value  in the register
    //! pointed to by the operand in numeric form.
    //! @note Offsets do not need to be considered as they offset the loaded
    //! value, not the address.
    std::size_t Get_Operand_Value(const std::size_t p_cycle,
                                  const std::string& p_operand) const
    {
        try
        {
            return Is_Register(p_operand)
                       ? Get_Register_Value(p_cycle, p_operand)
                       : static_cast<std::size_t>(std::stoi(p_operand));
        }
        // If p_operand is not numeric, i.e. corrupted data, try to recover by
        // clearing that value.
        catch (const std::invalid_argument&)
        {
            return 0;
        }
    }

    //! @brief Retrieves the value of an operand in numerical form. If that
    //! operand is a register then the value contained within that register is
    //! retrieved instead. If that operand does not exist, 0 is returned.
    //! @param p_cycle The cycle number at which to retrieve the operand
    //! from.
    //! @param p_instruction The instruction to retrieve the operand from.
    //! @param p_operand_number The index of the operand to retrieve the value
    //! from.
    //! @returns The value stored in the operand, or the value in the register
    //! pointed to by the operand in numeric form or, if there is not operand at
    //! that index, 0.
    //! @note This function is not zero indexed. Get_Operand_Value(x, y, 1) will
    //! retrieve the first operand.
    //! @note Offsets do not need to be considered as they offset the loaded
    //! value, not the address.
    std::size_t Get_Operand_Value(
        const std::size_t p_cycle,
        const GILES::Internal::Assembly_Instruction& p_instruction,
        const std::uint8_t p_operand_number) const
    {
        try
        {
            return Get_Operand_Value(
                p_cycle, p_instruction.Get_Operand(p_operand_number));
        }
        catch (const std::runtime_error&)
        {
            return 0;
        }
    }

    //! @brief Retrieves the total number of clock cycles that occurred
    //! during the running of the target program.
    //! @returns The total number of clock cycles.
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    std::size_t Get_Cycle_Count() const { return m_pipeline.size(); }

    //! TODO: Future: Make use of this.
    //! TODO: Maybe move this to be under Execution instead.
    //! @brief Gets a list of registers that were changed by this
    //! instruction and their new values.
    //! @returns A vector of Registers.
    /*
     *const std::vector<GILES::Internal::Register>& get_changed_registers()
     *const
     *{
     *    return m_changed_registers;
     *}
     */
};
}  // namespace Internal
}  // namespace GILES

#endif  // EXECUTION_HPP
