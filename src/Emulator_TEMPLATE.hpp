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

#ifndef EMULATOR_TEMPLATE_HPP
#define EMULATOR_TEMPLATE_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Emulator_Factory_Register.hpp"  // for Emulator_Factory_Register
#include "Emulator_Interface.hpp"         // for Emulator_Interface
#include "Execution.hpp"                  // for Execution

namespace ELMO2
{
namespace Internal
{
// Doxygen comments are inherited from Emulator_Interface.hpp
class Emulator_TEMPLATE : public virtual ELMO2::Internal::Emulator_Interface,
                          public ELMO2::Internal::Emulator_Factory_Register<
                              ELMO2::Internal::Emulator_TEMPLATE>
{
public:
    //! @brief Constructs an Emulator that will simulate the program given by
    //! p_program_path.
    //! @param p_program_path The path to the program to be loaded into the
    //! simulator.
    explicit Emulator_TEMPLATE(const std::string& p_program_path)
        : ELMO2::Internal::Emulator_Interface(p_program_path)
    {
        // This is required to be "used" somewhere in order to prevent the
        // compiler from optimising it away, thus preventing self registration.
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
        // Section 6.6.4.1, point 2 states that this statement will not be
        // optimised away.
        m_is_registered;
    }

    const ELMO2::Internal::Execution Run_Code() override;

    //! @brief Retrieves the name of this Emulator.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory works.
    //! The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "TEMPLATE"; }
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // EMULATOR_TEMPLATE_HPP
