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

#ifndef EMULATOR_THUMB_SIM_HPP
#define EMULATOR_THUMB_SIM_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Emulator.hpp"   // for Emulator_Interface
#include "Execution.hpp"  // for Execution

#include "simulator.cpp"  // for Simulator

namespace ELMO2
{
namespace Internal
{
class Emulator_Thumb_Sim : public virtual Emulator_Interface<Emulator_Thumb_Sim>
{
private:
    Thumb_Simulator::Debug m_execution_recording;

public:
    //! @brief Constructs an Emulator that will simulate the program given by
    //! p_program_path.
    //! @param p_program_path The path to the program to be loaded into the
    //! simulator.
    explicit Emulator_Thumb_Sim(const std::string& p_program_path)
        : Emulator_Interface(p_program_path)
    {
    }

    const ELMO2::Internal::Execution Run_Code() override;

    const std::string& Get_Extra_Data() override;

    //! @brief Retrieves the name of this Emulator.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory
    //! works. The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Thumb Sim"; }
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // EMULATOR_THUMB_SIM_HPP
