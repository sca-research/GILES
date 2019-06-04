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

#ifndef EMULATOR_UNICORN_HPP
#define EMULATOR_UNICORN_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Abstract_Factory_Register.hpp"  // for Emulator_Factory_Register
#include "Emulator_Interface.hpp"         // for Emulator_Interface
#include "Execution.hpp"                  // for Execution

namespace GILES
{
namespace Internal
{
// Doxygen comments are inherited from Emulator_Interface.hpp
class Emulator_Unicorn : public virtual GILES::Internal::Emulator_Interface,
                         public GILES::Internal::Emulator_Factory_Register<
                             GILES::Internal::Emulator_Unicorn>
{
private:
    uc_engine* uc;  // TODO: use smart pointers

    std::uint64_t elf_loader(const std::string& p_file);

    void map_space(const std::uint64_t p_start, const std::uint64_t p_end);

    void mem_write(const LIEF::ELF::Section& p_section);
    void mem_write(const LIEF::ELF::Segment& p_section);

public:
    //! @brief Constructs an Emulator that will simulate the program given by
    //! p_program_path.
    //! @param p_program_path The path to the program to be loaded into the
    //! simulator.
    explicit Emulator_Unicorn(const std::string& p_program_path)
        : GILES::Internal::Emulator_Interface(p_program_path)
    {
    }

    const GILES::Internal::Execution Run_Code() override;

    const std::string& Get_Extra_Data() override;

    //! @brief Retrieves the name of this Emulator.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory works.
    //! The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Unicorn"; }
};
}  // namespace Internal
}  // namespace GILES

#endif  // EMULATOR_UNICORN_HPP
