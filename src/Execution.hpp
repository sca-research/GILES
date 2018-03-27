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
    @file Execution.hpp
    @brief This file contains the internal representation of the Execution of a
    program.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
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
    //! The initial state of the registers in the emulator when the program
    //! execution started.
    const std::vector<ELMO2::Internal::Register> m_initial_registers;

    //! The full list of Assembly Instructions in the order they where executed.
    std::vector<ELMO2::Internal::Assembly_Instruction> m_assembly;

public:
    //! The constructor should be provided with the initial state of the
    //! registers and the first instruction executed as these should already be
    //! known. TODO: Should the first instruction be known? TODO: Pass in the
    //! correct objects as opposed to strings as the format should be
    //! interpreted by the specific emulator interface.
    //! TODO: Document params.
    Execution(const std::vector<ELMO2::Internal::Register>& p_initial_registers,
              const ELMO2::Internal::Assembly_Instruction& p_first_instruction)
        : m_initial_registers(p_initial_registers),
          m_assembly({p_first_instruction})
    {
    }

    Execution(){}

    //! Get the state of the registers as they were when the program
    //! started to run.
    const std::vector<ELMO2::Internal::Register>& get_initial_registers() const
    {
        return m_initial_registers;
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
