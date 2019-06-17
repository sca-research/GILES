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
    @file Emulator_TEMPLATE.hpp
    @todo add brief
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include "Emulator_TEMPLATE.hpp"
#include "Error.hpp"
#include "Execution.hpp"

const GILES::Internal::Execution GILES::Internal::Emulator_TEMPLATE::Run_Code()
{
    // *** Place your code here ***
    //! @note m_program_path Should contain the path to the target program.
    //! @note invoke_emulator(...) will return the output of a binary as a
    //! string. This can be used if the emulator is a binary.
    Error::Report_Error("Not yet implemented");
}

void GILES::Internal::Emulator_TEMPLATE::Inject_Fault(
    const std::uint32_t p_cycle_to_fault,
    const std::string& p_register_to_fault,
    const std::uint8_t p_bit_to_fault)
{
    Error::Report_Error("This feature is not supported by this simulator");
}

void GILES::Internal::Emulator_TEMPLATE::Add_Timeout(
    const std::uint32_t p_number_of_cyclest)
{
    Error::Report_Error("This feature is not supported by this simulator");
}
