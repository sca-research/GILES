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

/*!
    @file ELMO2.cpp
    @brief This file contains the entry point for the program and invokes other
    operations. // TODO: Change this when main.cpp is created.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Coefficients.hpp"
#include "Emulator_Interface.hpp"
#include "Execution.hpp"
#include "IO.hpp"
#include "Model.hpp"
#include "Model_Power.hpp"
#include "Traces.hpp"
#include "Unicorn_Interface.hpp"

namespace ELMO2
{
//! @class ELMO2
//! @brief The starting point for the program. This reads the user specified
//! options and invokes the corresponding components within the program.
//! TODO: Should this be static?
class ELMO_2
{
private:
    std::unique_ptr<ELMO2::Emulator_Interface> emulator_Interface;
    std::vector<ELMO2::Model> models;
    const ELMO2::IO io;
    const ELMO2::Coefficients coefficients;

    //! @brief Re-orders the assembly instructions.
    //! Re-ordering of the instructions is necessary as the order they are
    //! executed in is not the order that they were written in/compiled into.
    //! TODO: Maybe look into using a dissasembler e.g. capstone to work around
    //! this issue?
    //! TODO: Should accept the target program (Using the decided upon format
    //! (binary/assembly))
    void re_order_assembly() { throw("Function not yet implemented"); }

    //! @brief Interprets options passed to ELMO2.
    //! @param p_options The options as contained within an unordered map. The
    //! key is the option and the value is what that option is set to.
    //! TODO: Change the return type? At least to bool i.e. successful or not?
    void
    parse_options(const std::unordered_map<std::string, std::string>& p_options)
    {
        throw("Function not yet implemented");
    }

public:
    ELMO_2(
        const std::unordered_map<std::string, std::string>& p_options,
        const std::string& p_coefficients_path,
        const std::string& p_progam_path,
        const std::string& p_traces_path) // TODO: Look into std::optional in
                                          // C++17 as saving should be optional.
    : emulator_Interface(std::make_unique<ELMO2::Unicorn_Interface>(
          ELMO2::Unicorn_Interface(p_progam_path))),
      io(ELMO2::IO()), coefficients(ELMO2::Coefficients())
    {
        parse_options(p_options);
    }
};
} // namespace ELMO2

namespace // TODO: Add a Doxygen Comment explaining why this is in the anonymous
          // namespace. (It's only needed for currently compilation. If ELMO2
          // was a lib then this isn't needed) Also read more about anonymous
          // namespace.
{
//! @brief Interprets the command line flags.
//! @param p_options The options as contained within a string.
//! @returns The options as contained within an unordered map. The
//! key is the option and the value is what that option is set to.
//! TODO: Change the return type? At least to bool i.e. successful or not?
const std::unordered_map<std::string,
                         std::string>& // TODO: Return by reference here?
parse_command_line_flags(const char* p_options[])
{
    throw("Function not yet implemented");
}
} // namespace

//! @brief The entry point of the program.
int main(const int argc, const char* argv[])
{
    std::cout << "Hello World!\n";
    std::string p_coefficients_path = "./"; // TODO: Change this
    std::string p_traces_path = "./";       // TODO: Change this
    std::string p_program_path = "./";      // TODO: Change this

    parse_command_line_flags(argv); // TODO: Look into ways of having the
                                    // program path passed correctly.

    ELMO2::ELMO_2 elmo2 =
        ELMO2::ELMO_2(parse_command_line_flags(argv), p_coefficients_path,
                      p_program_path, p_traces_path);
    return 0;
}
