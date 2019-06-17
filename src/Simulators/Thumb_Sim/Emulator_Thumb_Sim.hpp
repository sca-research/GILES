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

#ifndef EMULATOR_THUMB_SIM_HPP
#define EMULATOR_THUMB_SIM_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Emulator.hpp"   // for Emulator_Interface
#include "Execution.hpp"  // for Execution

#include "simulator.cpp"  // for Simulator

namespace GILES
{
namespace Internal
{
class Emulator_Thumb_Sim : public virtual Emulator_Interface<Emulator_Thumb_Sim>
{
private:
    Simulator m_simulator;
    Thumb_Simulator::Debug m_execution_recording;

public:
    //! @brief Constructs an Emulator that will simulate the program given by
    //! p_program_path.
    //! @param p_program_path The path to the program to be loaded into the
    //! simulator.
    explicit Emulator_Thumb_Sim(const std::string& p_program_path)
        : Emulator_Interface{p_program_path}, m_simulator{}
    {
    }

    const GILES::Internal::Execution Run_Code() override;

    const std::string& Get_Extra_Data() override;

    void Inject_Fault(const std::uint32_t p_cycle_to_fault,
                      const std::string& p_register_to_fault,
                      const std::uint8_t p_bit_to_fault) override;

    void Add_Timeout(const std::uint32_t p_number_of_cyclest) override;

    //! @brief Retrieves the name of this Emulator.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory
    //! works. The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Thumb Sim"; }
};
}  // namespace Internal
}  // namespace GILES

#endif  // EMULATOR_THUMB_SIM_HPP
