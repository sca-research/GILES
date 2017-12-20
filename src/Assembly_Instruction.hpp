/*
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file Assembly_Instruction.hpp
    @brief The internal representation of an individual assembly instrction.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#ifndef ASSEMBLY_INSTRUCTION_HPP
#define ASSEMBLY_INSTRUCTION_HPP

#include <string>
#include <vector>

#include "Register.hpp"

namespace ELMO2
{
//! @class Assembly_Instruction
//! @brief The internal representation of an individual assembly instruction.
//! This contains the opcode and operands of the instruction as well as a list
//! of registers that have been changed making use of the Register class.
//! @see https://en.wikipedia.org/wiki/Assembly_language
class Assembly_Instruction
{
private:
    //! @see https://en.wikipedia.org/wiki/Opcode
    const std::string m_opcode;

    //! @see https://en.wikipedia.org/wiki/Operand#Computer_science
    std::vector<std::string> m_operands;
    const std::vector<Register> m_changed_registers;

public:
    //! @brief The constructor must be given all details about the assembly
    //! instruction as these cannot be set or changed later.
    //! @param p_opcode The instructions opcode.
    //! @param p_changed_registers A list of registers that the instrction has
    //! changed and their values.
    //! TODO: Should this be changed to instead a string and parse the opcode
    //! and operands from that? Or deduce the operands from the changed
    //! registers?
    Assembly_Instruction(
        const std::string& p_opcode,
        const std::vector<ELMO2::Register>& p_changed_registers)
        : m_opcode(p_opcode), m_changed_registers(p_changed_registers)
    {
    }

    //! @brief Gets the instructions opcode.
    //! @return The opcode.
    const std::string& get_opcode() const { return m_opcode; }

    //! @brief Gets a list of the instructions operands.
    //! @return A vector of operands.
    // TODO: Should this get the operand at a specified index i.e.
    // get_operand(1);
    const std::vector<std::string>& get_operands() const { return m_operands; }

    //! @brief Gets a list of registers that were changed by this instrcution
    //! and their new values.
    //! @returns A vector of Registers.
    const std::vector<ELMO2::Register>& get_changed_registers() const
    {
        return m_changed_registers;
    }
};
} // namespace ELMO2
#endif
