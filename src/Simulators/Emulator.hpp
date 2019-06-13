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
    @file Emulator_Interface.hpp
    @brief Contains the Emulator_Interface class which serves as a base case for
    the interface to a specific emulator.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef EMULATOR_INTERFACE_HPP
#define EMULATOR_INTERFACE_HPP

#include <cstdio>  // for popen
#include <string>  // for string
#include <vector>  // for vector

#include "Abstract_Factory_Register.hpp"  // for Emulator_Factory_Register
#include "Assembly_Instruction.hpp"
#include "Execution.hpp"

namespace GILES
{
namespace Internal
{
//! @class Emulator
//! @brief An abstract class that serves as a base class for the interface
//! to a specific emulator. The emulator will record the Execution of the
//! target program. This class theoretically allows multiple emulators to be
//! utilised as per the users choice.
//! This class is not designed to be inherited from directly; instead
//! Emulator_Interface should be inherited from. This class provides a
//! non-templated base class to allow handling of derived objects.
class Emulator
{
protected:
    //! The path to the target program.
    const std::string m_program_path;

    //! @brief This constructor is marked as protected as it should only be
    //! called by derived classes to assist with initialisation.
    //! @param p_program_path The path where the program is.
    explicit Emulator(const std::string& p_program_path)
        : m_program_path(p_program_path)
    {
    }

    //! @brief Runs the command provided and returns the stdout output contained
    //! within a string.
    //! @todo CHANGE THIS AND DOCUMENT
    //! @todo Change this to not return a string for speed?
    //! @see https://stackoverflow.com/q/478898
    const std::string invoke_emulator(const std::string& p_emulator_command,
                                      const std::string& p_emulator_path = "")
    {
        // A temporary buffer to read the output into.
        std::array<char, 128> buffer;

        // The result of running p_emulator_command.
        std::string result;

        // Run the command, and capture the output within a FILE
        std::shared_ptr<std::FILE> command_output(
            popen((p_emulator_path + p_emulator_command).c_str(), "r"), pclose);

        if (!command_output)
            throw std::runtime_error("popen() failed!");

        // While not the end of file, read more data.
        while (!feof(command_output.get()))
        {
            // Read data into the buffer and then append it to result.
            if (nullptr != fgets(buffer.data(), 128, command_output.get()))
                result += buffer.data();
        }
        return result;
    }

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Emulator() = default;

    //! @brief A function to start the process of invoking the emulator and
    //! recording the results.
    //! @returns The recorded Execution of the target program as an Execution
    //! object.
    virtual const Execution Run_Code() = 0;

    //! @brief A function to request to inject a fault in the simulator.
    //! @param p_cycle_to_fault The clock cycle indicating when to inject the
    //! fault.
    //! @param p_register_to_fault The name of the register to inject the fault
    //! into.
    //! @param p_bit_to_fault The index of the bit to be faulted.
    //! @see https://en.wikipedia.org/wiki/Fault_injection
    virtual void Inject_Fault(const std::uint32_t p_cycle_to_fault,
                              const std::string& p_register_to_fault,
                              const std::uint8_t p_bit_to_fault) = 0;

    //! @todo Document
    virtual const std::string& Get_Extra_Data() = 0;
};

//! @class Emulator_Interface
//! @brief This adds self registering factory code to the derived class, given
//! by derived_t and delegates construction of a base class to the Emulator
//! class.
//! @tparam derived_t This should be the same as the derived type. This will add
//! the self registering factory code automatically, allowing use of the
//! derived class.
template <typename derived_t>
class Emulator_Interface : public Emulator,
                           public Emulator_Factory_Register<derived_t>
{
protected:
    //! @brief This constructor is marked as protected as it should only be
    //! called by derived classes to assist with initialisation.
    //! @param p_program_path The path where the program is.
    explicit Emulator_Interface(const std::string& p_program_path)
        : Emulator(p_program_path)
    {
    }

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Emulator_Interface() = default;
};
}  // namespace Internal
}  // namespace GILES

#endif
