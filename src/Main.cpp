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

#include <algorithm>     // for move
#include <iostream>      // for operator<<, cout, endl, ostream, basic_ostream
#include <memory>        // for __shared_ptr_access
#include <optional>      // for optional
#include <stdexcept>     // for invalid_argument
#include <stdlib.h>      // for exit
#include <string>        // for string, operator<<

#include <cxxopts.hpp> // for Options, value, OptionAdder, OptionDetails

#include "ELMO2.cpp" // for ELMO_2

//! Anonymous namespace is used as this functionality is only required when
//! building not as a library.
namespace
{
std::string m_program_path;
std::string m_coefficients_path;
std::optional<std::string> m_traces_path;

//! @brief Interprets the command line flags.
//! @param p_options The options as contained within a string.
void parse_command_line_flags(int& argc, char**& argv)
{
    cxxopts::Options options(argv[0], "Side channel leakage emulation tool");

    options.positional_help("[--input] EXECUTABLE [--file] "
                                    "COEFFICIENTS");

    // Adds the command line options.
    options.add_options()("h,help", "Print help")
            ("r,runs", "Number of traces to generate", cxxopts::value<int>(),
             "N")
            ("f,file", "Coefficients file",
             cxxopts::value<std::string>()->default_value("./coeffs.json"),
             "COEFFICIENTS")
            ("i,input", "Executable to be ran in the emulator",
             cxxopts::value<std::string>(), "EXECUTABLE")
            ("o,output", "Generated traces output file",
             cxxopts::value<std::string>(), "FILE");

    // Input can be specified without -i/--input flag
    // Coefficients File can be specified without -f/--file flag
    options.parse_positional(std::vector<std::string>{"input", "file"});

    const auto& result = options.parse(argc, argv);

    if (result.count("help")) // if help flag is passed
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("input")) // if input flag is passed
    {
        m_program_path = result["input"].as<std::string>();
    }
    else
    {
        throw std::invalid_argument("Input option is required. (-i / --input "
                                    "\"Path to Executable\"). See "
                                    "--help for more.");
    }

    if (result.count("output")) // if output flag is passed
    {
        m_traces_path = result["output"].as<std::string>();
    }

    // default "./coeffs.json" is used if flag is not passed
    m_coefficients_path = result["file"].as<std::string>();
}
} // namespace

//! @brief The entry point of the program.
int main(int argc, char* argv[])
{
    parse_command_line_flags(argc, argv);

    ELMO2::ELMO_2 elmo2 =
        ELMO2::ELMO_2(m_program_path, m_coefficients_path, m_traces_path);
    return 0;
}
