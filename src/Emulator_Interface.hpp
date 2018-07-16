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
    @file Emulator_Interface.hpp
    @brief Contains the Emulator_Interface class which serves as a base case for
    the interface to a specific emulator.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef EMULATOR_INTERFACE_HPP
#define EMULATOR_INTERFACE_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Assembly_Instruction.hpp"
#include "Execution.hpp"
#include "Register.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Emulator_Interface
//! @brief An abstract class that serves as a base class for the interface to a
//! specific emulator. The emulator will record the Execution of the target
//! program. This class theoretically allows multiple emulators to be utilised
//! as per the users choice.
// TODO: Look into gem5.
class Emulator_Interface
{
private:
    const std::string m_program_path;  // TODO: This should be optional ideally:
                                       // Don't pass in path instead pass in
                                       // binary loaded from IO. Can this simply
                                       // be loaded as a string or is there a
                                       // better way?

    // TODO: ****** GO THROUGH EVERY CLASS WITH RULE OF FIVE *******************
protected:
    //! @brief This constructor is marked as protected as it should only be
    //! called by derived classes to assist with initialisation.
    //! @param p_program_path The path where the program is. TODO: Change this
    //! to hold the binary instead of path.
    explicit Emulator_Interface(const std::string& p_program_path)
        : m_program_path(p_program_path)
    {
    }

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Emulator_Interface() =
        default;  // TODO: Should this = default? Rule of
    // ZERO? http://en.cppreference.com/w/cpp/language/rule_of_three
    // TODO: Should many functions everywhere be noexcept?

    //! @brief A function to start the process of invoking the emulator and
    //! recording the results.
    //! @returns The recorded Execution of the target program.
    virtual const ELMO2::Internal::Execution Run_Code() = 0;
};
}  // namespace Internal
}  // namespace ELMO2

#endif
