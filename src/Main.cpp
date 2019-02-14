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

#include <algorithm>  // for move
#include <cstdlib>    // for exit, EXIT_FAILURE
#include <iostream>   // for operator<<, cout, endl, ostream, basic_ostream
#include <memory>     // for __shared_ptr_access
#include <optional>   // for optional
#include <stdexcept>  // for invalid_argument
#include <string>     // for string, operator<<
#include <vector>     // for vector

#include <cxxopts.hpp>  // for Options, value, OptionAdder, OptionDetails

#include "ELMO2.cpp"  // for ELMO_2

//! Anonymous namespace is used as this functionality is only required when
//! building not as a library.
namespace
{
// TODO: Put this all in a class
std::string m_program_path;
std::string m_coefficients_path;
std::string m_model_name;
std::string m_simulator_name;
std::optional<std::string> m_traces_path;
std::uint32_t m_number_of_runs;

//! @brief Interprets the command line flags.
//! @param p_options The options as contained within a string.
// TODO: Returns tag?
void parse_command_line_flags(int& argc, char**& argv)
{
    cxxopts::Options options(argv[0], "Side channel leakage emulation tool");

    // clang-format off
    options.positional_help("[--input] EXECUTABLE "
                            "[--file] COEFFICIENTS");

    // Adds the command line options.
    options.add_options()
        ("h,help", "Print help")
        ("r,runs", "Number of traces to generate",
             cxxopts::value<std::uint32_t>()->default_value("1"),
             "N")
        ("f,file", "Coefficients file",
             cxxopts::value<std::string>()->default_value("./coeffs.json"),
             "COEFFICIENTS")
        ("i,input", "Executable to be ran in the simulator",
             cxxopts::value<std::string>(),
             "EXECUTABLE")
        ("o,output", "Generated traces output file",
             cxxopts::value<std::string>(),
             "FILE")
        ("s,simulator", "The name of the simulator that should be used",
             cxxopts::value<std::string>(),
             "SIMULATOR NAME")
        ("m,model", "The name of the mathematical model that should be used to "
                    "generate traces",
             cxxopts::value<std::string>(),
             "MODEL NAME");
    // clang-format on

    // Input can be specified without -i/--input flag
    // Coefficients File can be specified without -f/--file flag
    const auto& result = [&] {
        try
        {
            options.parse_positional(std::vector<std::string>{"input", "file"});

            return options.parse(argc, argv);
        }
        catch (const cxxopts::OptionParseException& exception)
        {
            std::cout << exception.what() << std::endl
                      << "Please use option --help or -h to see proper usage"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }();

    if (0 != result.count("help"))  // if help flag is passed
    {
        std::cout << options.help() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (0 != result.count("output"))  // if output flag is passed
    {
        m_traces_path = result["output"].as<std::string>();
    }

    if (0 != result.count("input"))  // if input flag is passed
    {
        m_program_path = result["input"].as<std::string>();
    }
    else
    {
        std::cout << "Input option is required. "
                     "(-i / --input \"Path to Executable\")"
                  << std::endl
                  << "Please use option --help or -h to see proper usage"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (0 != result.count("output"))  // if output flag is passed
    {
        m_traces_path = result["output"].as<std::string>();
    }

    // default "./coeffs.json" is used if flag is not passed
    m_coefficients_path = result["file"].as<std::string>();

    // default "Hamming Weight" is used if flag is not passed
    m_model_name = result["Hamming Weight"].as<std::string>();

    // default "Andres" is used if flag is not passed
    m_simulator_name = result["Andres"].as<std::string>();

    // default 1 is used if flag is not passed
    // TODO: Remove this default?
    m_number_of_runs = result["runs"].as<std::uint32_t>();
}
}  // namespace

//! @brief The entry point of the program.
int main(int argc, char* argv[])
{
    parse_command_line_flags(argc, argv);

    ELMO2::ELMO_2 elmo2 = ELMO2::ELMO_2(
        m_program_path, m_coefficients_path, m_traces_path, m_number_of_runs);

    elmo2.run();
    return 0;
}
