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
    @file Main.cpp
    @brief This file contains a command line entry executable that invokes GILES
    with the correct options.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <algorithm>  // for move
#include <cstdlib>    // for exit, EXIT_SUCCESS
#include <memory>     // for __shared_ptr_access
#include <optional>   // for optional
#include <stdexcept>  // for invalid_argument
#include <string>     // for string, operator<<
#include <vector>     // for vector

#include <cxxopts.hpp>   // for Options, value, OptionAdder, OptionDetails
#include <fmt/format.h>  // for print

#include "GILES.cpp"  // for GILES
#include "Error.hpp"  // for Report_Exit

//! Anonymous namespace is used as this functionality is only required when
//! building not as a library.
namespace
{
// TODO: Put this all in a class?
std::string m_program_path;
std::string m_coefficients_path;
std::string m_model_name;
std::string m_simulator_name;
std::optional<std::string> m_traces_path;
std::uint32_t m_number_of_runs;

//! @brief Prints an error message and exits. This is to be called when the
//! program cannot run given the supplied command line arguments.
//! @note This function is marked as noreturn as it is guaranteed to always halt
//! the program. (Through std::exit())
//! @param p_message An error message can optionally be provided. This will be
//! printed first on a separate line if provided.
[[noreturn]] void bad_options(const std::string& p_message = "") {
    if (!p_message.empty())
    {
        fmt::print("{}\n", p_message);
    }
    GILES::Internal::Error::Report_Exit(
        "Please use option --help or -h to see proper usage");
}

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
             cxxopts::value<std::string>()->default_value("Thumb Sim"),
             "SIMULATOR NAME")
        ("m,model", "The name of the mathematical model that should be used to "
                    "generate traces",
             cxxopts::value<std::string>()->default_value("Hamming Weight"),
             "MODEL NAME");
    // clang-format on

    const auto& result = [&] {
        try
        {
            // Input can be specified without -i/--input flag
            // Coefficients File can be specified without -f/--file flag
            options.parse_positional(std::vector<std::string>{"input", "file"});

            return options.parse(argc, argv);
        }
        catch (const cxxopts::OptionParseException& exception)
        {
            bad_options(exception.what());
        }
    }();

    if (0 != result.count("help"))  // if help flag is passed
    {
        fmt::print("{}\n", options.help());
        std::exit(EXIT_SUCCESS);
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
        bad_options(
            "Input option is required.(-i / --input \"Path to Executable\")");
    }

    if (0 != result.count("output"))  // if output flag is passed
    {
        m_traces_path = result["output"].as<std::string>();
    }

    // default "./coeffs.json" is used if flag is not passed
    m_coefficients_path = result["file"].as<std::string>();

    // default "Thumb Sim" is used if flag is not passed
    m_simulator_name = result["simulator"].as<std::string>();

    // default "Hamming Weight" is used if flag is not passed
    m_model_name = result["model"].as<std::string>();

    // default 1 is used if flag is not passed
    // TODO: Remove this default?
    m_number_of_runs = result["runs"].as<std::uint32_t>();
}
}  // namespace

//! @brief The entry point of the program.
int main(int argc, char* argv[])
{
    parse_command_line_flags(argc, argv);

    GILES::GILES elmo2 = GILES::GILES(m_program_path,
                                        m_coefficients_path,
                                        m_traces_path,
                                        m_number_of_runs,
                                        m_model_name);

    elmo2.Run();
    return 0;
}
