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

#ifndef EMULATOR_ANDRES_HPP
#define EMULATOR_ANDRES_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Emulator_Factory_Register.hpp"  // for Emulator_Factory_Register
#include "Emulator_Interface.hpp"         // for Emulator_Interface
#include "Execution.hpp"                  // for Execution

#include "simulator.cpp"  // for Simulator

namespace ELMO2
{
namespace Internal
{
// Doxygen comments are inherited from Emulator_Interface.hpp
class Emulator_Andres : public virtual ELMO2::Internal::Emulator_Interface,
                        public ELMO2::Internal::Emulator_Factory_Register<
                            ELMO2::Internal::Emulator_Andres>
{
private:
    Thumb_Simulator::Debug m_execution_recording;

public:
    //! @brief Constructs an Emulator that will simulate the program given by
    //! p_program_path.
    //! @param p_program_path The path to the program to be loaded into the
    //! simulator.
    explicit Emulator_Andres(const std::string& p_program_path)
        : ELMO2::Internal::Emulator_Interface(p_program_path)
    {
        // This statement registers this class in the factory, allowing access
        // from elsewhere. Do not delete this or else this class will not appear
        // in the factory. If you wish to make this class inaccessible, a better
        // method would be to remove the corresponding cpp file from the build
        // script.
        // This is required to be "used" somewhere in order to prevent
        // the compiler from optimising it away, thus preventing self
        // registration.
        // Section 6.6.4.1, point 2 of the linked document states that this
        // statement will not be optimised away.
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
        // The void cast does nothing functionally but prevents the compiler
        // warning about an unused result.
        (void)m_is_registered;
    }

    const ELMO2::Internal::Execution Run_Code() override;

    const std::string& Get_Extra_Data() override;

    //! @brief Retrieves the name of this Emulator.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory
    //! works. The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Andres"; }
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // EMULATOR_ANDRES_HPP
