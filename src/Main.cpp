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
    @file Main.cpp
    @brief This file contains a command line entry executable that invokes ELMO2
    with the correct options.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <iostream>
#include <unordered_map>

#include "ELMO2.cpp"

//! Anonymous namespace is used as this functionality is only required when
//! building not as a library.
namespace
{
//! @brief Interprets the command line flags.
//! @param p_options The options as contained within a string.
//! @returns The options as contained within an unordered map. The
//! key is the option and the value is what that option is set to.
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

    parse_command_line_flags(
        argv); // TODO: Look into ways of having the
               // program path passed correctly. - Boost::program_options

    ELMO2::ELMO_2 elmo2 =
        ELMO2::ELMO_2(parse_command_line_flags(argv), p_coefficients_path,
                      p_program_path, p_traces_path);
    return 0;
}
