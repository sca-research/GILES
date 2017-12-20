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

// TODO: Add doxygen comments after modifying Emulator_Interface.hpp

#ifndef UNICORN_INTERFACE_HPP
#define UNICORN_INTERFACE_HPP

#include <string>
#include <vector>

#include "Emulator_Interface.hpp"

namespace ELMO2
{
class Unicorn_Interface : public virtual ELMO2::Emulator_Interface
{
private:
    const std::vector<ELMO2::Register>&
    parse_registers(const std::string& p_registers) override;

    const ELMO2::Assembly_Instruction&
    parse_assembly(const std::string& p_assembly) override;

public:
    explicit Unicorn_Interface(const std::string& p_program_path)
        : ELMO2::Emulator_Interface(p_program_path)
    {
    }

    const ELMO2::Execution& Run_Code() override;
};
} // namespace ELMO2

#endif
