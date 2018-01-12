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
    @file Execution.hpp
    @brief This file contains the internal representation of the Execution of a
    program.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <string>
#include <vector>

#include "Assembly_Instruction.hpp"
#include "Register.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Execution
//! @brief The internal representation of the Execution of a program. This
//! contains a list of Assembly_Instruction as well as the state of every
//! Register before the Execution started
class Execution
{
private:
    //! The inital state of the registers in the emualator when the program
    //! execution started.
    const std::vector<ELMO2::Internal::Register> m_inital_registers;

    //! The full list of Assembly Instructions in the order they where executed.
    std::vector<ELMO2::Internal::Assembly_Instruction> m_assembly;

public:
    //! The constructor should be provided with the initial state of the
    //! registers and the first instruction executed as these should already be
    //! known. TODO: Should the first instruction be known? TODO: Pass in the
    //! correct objects as opposed to strings as the format should be
    //! interpreted by the specific emulator interface.
    //! TODO: Document params.
    Execution(const std::vector<ELMO2::Internal::Register>& p_inital_registers,
              const ELMO2::Internal::Assembly_Instruction& p_first_instruction)
        : m_inital_registers(p_inital_registers),
          m_assembly({p_first_instruction})
    {
    }

    //! Get the state of the registers as they were when the program
    //! started to run.
    const std::vector<ELMO2::Internal::Register>& get_inital_registers() const
    {
        return m_inital_registers;
    }

    //! Get the full list of Assembly Instructions in the order they where
    //! executed.
    const std::vector<ELMO2::Internal::Assembly_Instruction>&
    get_assembly() const
    {
        return m_assembly;
    }
};
} // namespace Internal
} // namespace ELMO2

#endif
