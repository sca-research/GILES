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
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ASSEMBLY_INSTRUCTION_HPP
#define ASSEMBLY_INSTRUCTION_HPP

#include <string>   // for string
#include <utility>  // for pair
#include <vector>   // for vector

#include <boost/dynamic_bitset.hpp>

#include "Register.hpp"

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
private:
    //! A pair of the assembly and the binary representation of the opcode.
    //! @see https://en.wikipedia.org/wiki/Opcode
    //! @todo Future improvement: Store whole encoded instruction as well.
    std::pair<std::string, boost::dynamic_bitset<>> m_opcode;

    //! The instruction operands stored as binary.
    //! @see https://en.wikipedia.org/wiki/Operand#Computer_science
    //! @todo Does this need to be stored in human readable form as well? If
    //! so then std::pair could be used.
    //! @todo Future improvement: Store whole encoded instruction
    std::vector<boost::dynamic_bitset<>> m_operands;

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
    //! full instruction and binary form of opcode and seperate operands
    //! @todo Use string splitter on input if needed
    //! @see
    //! https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
    Assembly_Instruction(const std::string& p_opcode,  // TODO: Maybe refactor?
                         const std::string& p_opcode_binary,
                         const std::vector<std::string>&
                             p_operands)  // TODO: Why is this a string?
        : m_opcode(std::make_pair(p_opcode,
                                  boost::dynamic_bitset<>(p_opcode_binary)))
    {
        for (const auto& operand : p_operands)
        {
            m_operands.push_back(boost::dynamic_bitset<>(operand));
        }
    }

    //! @brief Gets the instructions opcode in human readable form e.g. "add".
    //! @return The opcode as a string.
    const std::string& get_opcode() const { return m_opcode.first; }

    //! @brief Gets the binary representation of the instructions opcode.
    //! @return The opcode contained within a dynamic_bitset container from the
    //! boost libraries.
    //! @see
    //! https://www.boost.org/doc/libs/release/libs/dynamic_bitset/dynamic_bitset.html
    const boost::dynamic_bitset<>& get_opcode_binary() const
    {
        return m_opcode.second;
    }

    //! @brief Gets a list of the instructions operands.
    //! @return A vector of operands.
    // TODO: Should this get the operand at a specified index i.e.
    // get_operand(1); //TODO: Implement both after converting OPERANDS to
    // ORDERED map. Operands != Registers. A list of registers is better
    // stored as a map.
    const std::vector<boost::dynamic_bitset<>>& get_operands() const
    {
        return m_operands;
    }

    //! @brief Gets one of the instructions operands.
    //! @return A string containing the operand.
    //! @note get_operand(1) will return
    //! @exception std::out_of_range This is thrown when the index given by
    //! p_operand_index - 1 is out of the array bounds as no range checking
    //! is performed in this function.
    const boost::dynamic_bitset<>&
    get_operand(const uint8_t p_operand_index) const
    {
        return m_operands[p_operand_index - 1];
    }

    //! TODO: Future: Make use of this.
    //! TODO: Maybe move this to be under Execution instead.
    //! @brief Gets a list of registers that were changed by this
    //! instruction and their new values.
    //! @returns A vector of Registers.
    /*
     *const std::vector<ELMO2::Internal::Register>& get_changed_registers()
     *const
     *{
     *    return m_changed_registers;
     *}
     */
};
}  // namespace Internal
}  // namespace ELMO2
#endif
