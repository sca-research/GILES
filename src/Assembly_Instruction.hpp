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
    @file Assembly_Instruction.hpp
    @brief The internal representation of an individual assembly instruction.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ASSEMBLY_INSTRUCTION_HPP
#define ASSEMBLY_INSTRUCTION_HPP

#include <cstdint>    // for uint32_t
#include <stdexcept>  // for overflow_error, underflow_error
#include <string>     // for string
#include <utility>    // for pair
#include <vector>     // for vector

#include <boost/algorithm/string.hpp>  // TODO: Convert Uility.h over to boost algorithms (or the other way around?)

namespace ELMO2
{
namespace Internal
{
//! @class Assembly_Instruction
//! @brief The internal representation of an individual assembly instruction.
//! This contains the opcode and operands of the instruction as well as a list
//! of registers that have been changed making use of the Register class.
//! @see https://en.wikipedia.org/wiki/Assembly_language
class Assembly_Instruction
{
protected:
    //! A pair of the assembly and the binary representation of the opcode.
    //! @see https://en.wikipedia.org/wiki/Opcode
    //! @todo Future improvement: Store whole encoded instruction as well.
    // std::pair<std::string, boost::dynamic_bitset<>> m_opcode;
    // This is commented as the encoded opcode does not leak in any way.

    //! @see https://en.wikipedia.org/wiki/Opcode
    std::string m_opcode;

    //! The instruction operands stored in human readable form.
    //! @see https://en.wikipedia.org/wiki/Operand#Computer_science
    //! @todo Does this need to be stored in binary as well? If
    //! so then std::pair could be used with boost::dynamic_bitset.
    //! @todo Future improvement: Store whole encoded instruction
    std::vector<std::string> m_operands;

    //! The numerical values stored within the operands in m_operands. If the
    //! operand is a register, then the value within that register is stored.
    //! @see https://en.wikipedia.org/wiki/Operand#Computer_science
    //! @todo Does this need to be stored in binary as well? If
    //! so then std::pair could be used with boost::dynamic_bitset.
    //! @todo Future improvement: Store whole encoded instruction
    // std::vector<std::uint32_t> m_operand_values;

    // TODO: Future: Should this be stored in Assembly_Instruction or Execution?
    // const std::vector<Register> m_changed_registers;

public:
    //! @todo Change brief.
    //! @brief The constructor must be given all details about the assembly
    //! instruction as these cannot be set or changed later.
    //! @param p_opcode The instructions opcode.
    //! @param p_opcode_binary The binary representation of the opcode as it
    //! would be encoded on real hardware.
    //! @param p_operands The binary representation of the operands as they
    //! would be encoded on real hardware.
    //! @todo Remove temp constructors
    //! @todo A disassembler is needed to get opcode as well as binary form of
    //! full instruction and binary form of opcode and separate operands
    Assembly_Instruction(const std::string& p_opcode,  // TODO: Maybe refactor?
                         std::vector<std::string> p_operands)
        : m_opcode(p_opcode)
    {
        for (std::string& operand : p_operands)
        {
            boost::algorithm::trim(operand);  // Remove whitespace
            m_operands.push_back(operand);
        }
    }

    // Copy Constructor
    Assembly_Instruction(const Assembly_Instruction& other) noexcept
        : m_opcode(other.m_opcode), m_operands(other.m_operands)
    {
    }

    // Move constructor
    Assembly_Instruction(Assembly_Instruction&& other) noexcept
        : m_opcode(std::move(other.m_opcode)), m_operands(other.m_operands)
    {
    }

    //! @todo document
    //! Move constructor
    //// TODO: should move be exchange?
    /*
     *Assembly_Instruction(const Assembly_Instruction&& other)
     *    : m_opcode(std::move(other.m_opcode)),
     *      m_operands(std::move(other.m_operands))
     *{
     *}
     */

    //! @brief Gets the instructions opcode in human readable form e.g.
    //! "add".
    //! @return The opcode as a string.
    const std::string& Get_Opcode() const noexcept { return m_opcode; }

    //! @brief Gets a list of the instructions operands.
    //! @return A vector of operands.
    // TODO: Should this get the operand at a specified index i.e.
    // get_operand(1); //TODO: Implement both after converting OPERANDS to
    // ORDERED map. Operands != Registers. A list of registers is better
    // stored as a map.
    const std::vector<std::string>& Get_Operands() const { return m_operands; }

    //! @brief Gets one of the instructions operands.
    //! @return A string containing the operand.
    //! @note get_operand(1) will return
    //! @exception std::out_of_range This is thrown when the index given by
    //! p_operand_index - 1 is out of the array bounds as no range checking
    //! is performed in this function.
    //! @throws overflow_error This is thrown when the instruction does not
    //! contain enough operands. E.g. The fifth operand is requested when the
    //! instruction only has one operand.
    //! @throws underflow_error When the number given by p_operand_index is too
    //! low to refer to any operand.
    //! @note This function is not zero indexed. Get_Operand(1) will retrieve
    //! the first operand.
    const std::string& Get_Operand(const uint8_t p_operand_index) const
    {
        if (p_operand_index > m_operands.size())
        {
            throw std::overflow_error(
                "Cannot retrieve operand. This " +
                static_cast<std::string>(m_opcode) +
                "instruction does not have that many operands");
        }
        else if (0 == p_operand_index)
        {
            throw std::underflow_error(
                "Cannot retrieve operand. Operand index cannot be 0. This may "
                "because this function is not zero indexed. Get_Operand(1) "
                "will retrieve the first operand.");
        }
        return m_operands.at(p_operand_index - 1);
    }

    std::size_t Get_Number_of_Operands() const { return m_operands.size(); }

    /*
     *const std::size_t Get_Operand_Value(const uint8_t p_operand_index) const
     *{
     *    try
     *    {
     *        return Get_Operand(p_operand_index);
     *    }
     *    catch (const std::runtime_error& exception)
     *    {
     *        return 0;
     *    }
     *};
     */
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // EXECUTION_HPP
