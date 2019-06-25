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

#include <boost/program_options.hpp>  // for options_description, value...
#include <fmt/format.h>               // for format
#include <fmt/ostream.h>              // for operator<<

#include "Error.hpp"  // for Report_Exit
#include "GILES.cpp"  // for GILES

//! Anonymous namespace is used as this functionality is only required when
//! building not as a library.
namespace
{
// TODO: Put this all in a class? - Probably should
std::string m_program_path;
std::string m_coefficients_path;
std::string m_model_name;
std::string m_simulator_name;
std::optional<std::string> m_traces_path;
std::uint32_t m_number_of_runs;

// These options are related to fault injection.
bool m_fault{false};
std::uint32_t m_fault_cycle;
std::string m_fault_register;
std::uint8_t m_fault_bit;

std::optional<std::uint32_t> m_timeout;

//! @brief Prints an error message and exits. This is to be called when the
//! program cannot run given the supplied command line arguments.
//! @note This function is marked as noreturn as it is guaranteed to always
//! halt the program. (Through std::exit())
//! @param p_message An error message can optionally be provided. This will
//! be printed first on a separate line if provided.
template <typename... args_t>
[[noreturn]] void bad_options(const args_t&... p_message) {
    fmt::print(p_message...);
    GILES::Internal::Error::Report_Exit(
        "\nPlease use option --help or -h to see proper usage");
}

//! @brief Interprets the command line flags.
//! @param p_options The options as contained within a string.
// TODO: Returns tag?
void parse_command_line_flags(int argc, char* argv[])
{
    boost::program_options::options_description options_description{fmt::format(
        "General instruction leakage simulator\n"
        "Usage: {} [--input] EXECUTABLE [--coefficients] COEFFICIENTS\n",
        argv[0])};

    std::vector<std::string> fault_options{};

    // clang-format off

    // Adds the command line options.
    options_description.add_options()
        ("help,h", "Print help")
        ("runs,r",
            boost::program_options::value<std::uint32_t>()->default_value(1),
            "Number of traces to generate")
        ("coefficients,c",
            boost::program_options::value<std::string>()->default_value(
            "./coeffs.json"),
            "Coefficients file")
        ("input,i",
            boost::program_options::value<std::string>(),
            "Executable to be ran in the simulator")
        ("output,o",
            boost::program_options::value<std::string>(),
            "Generated traces output file")
        ("simulator,s",
            boost::program_options::value<std::string>()->default_value(
            "Thumb Sim"),
            "The name of the simulator that should be used")
        ("model,m",
            boost::program_options::value<std::string>()->default_value(
            "Hamming Weight"),
            "The name of the mathematical model that should be used to "
            "generate traces")
        ("fault,f",
             boost::program_options::value<std::vector<std::string>>(
             &fault_options)
             ->multitoken(),
            "Where to inject a fault. e.g. \"--fault 10 R0 2\" is inject a "
            "fault "
            "before the 10th clock cycle, by flipping the second least "
            "significant bit in the register R0");
    // clang-format on


    boost::program_options::variables_map options;
    try
    {
        // Parse the provided arguments.
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(options_description)
                .style(boost::program_options::command_line_style::unix_style ^
                       boost::program_options::command_line_style::allow_short)
                .run(),
            options);
        boost::program_options::notify(options);
    }
    catch (const std::exception& exception)
    {
        bad_options(exception.what());
    }

    if (options.count("help"))  // if help flag is passed
    {
        fmt::print("{}\n", options_description);
        std::exit(EXIT_SUCCESS);
    }

    if (options.count("output"))  // if output flag is passed
    {
        m_traces_path = options["output"].as<std::string>();
    }

    if (options.count("input"))  // if input flag is passed
    {
        m_program_path = options["input"].as<std::string>();
    }
    else
    {
        bad_options("Input option is required.(-i / --input \"Path to "
                    "Executable\")");
    }

    if (options.count("output"))  // if output flag is passed
    {
        m_traces_path = options["output"].as<std::string>();
    }

    // Fault injection options
    if (options.count("fault"))
    {
        try
        {
            constexpr std::uint8_t number_of_fault_options{3};
            if (const std::size_t size{fault_options.size()};
                size != number_of_fault_options)
            {
                bad_options("Incorrect number of fault injection options "
                            "provided.\nExpected: {}\nGot: {}",
                            number_of_fault_options,
                            size);
            }
            m_fault_cycle    = std::stoi(fault_options[0]);
            m_fault_register = fault_options[1];
            m_fault_bit      = std::stoi(fault_options[2]);
        }
        catch (const std::exception&)
        {
            bad_options("Fault injection options could not be interpreted");
        }

        m_fault = true;
    }

    if (options.count("timeout"))
    {
        m_timeout = options["timeout"].as<std::uint32_t>();
    }

    // default "./coeffs.json" is used if flag is not passed
    m_coefficients_path = options["coefficients"].as<std::string>();

    // default "Thumb Sim" is used if flag is not passed
    m_simulator_name = options["simulator"].as<std::string>();

    // default "Hamming Weight" is used if flag is not passed
    m_model_name = options["model"].as<std::string>();

    // default 1 is used if flag is not passed
    // TODO: Remove this default?
    m_number_of_runs = options["runs"].as<std::uint32_t>();
}
}  // namespace

//! @brief The entry point of the program.
int main(int argc, char* argv[])
{
    parse_command_line_flags(argc, argv);

    GILES::GILES giles = GILES::GILES(m_program_path,
                                      m_coefficients_path,
                                      m_traces_path,
                                      m_number_of_runs,
                                      m_model_name);

    // If fault inject options are provided then send them to GILES,
    if (m_fault)
    {
        giles.Inject_Fault(m_fault_cycle, m_fault_register, m_fault_bit);
    }

    // If the timeout option is provided then send it to GILES,
    if (m_timeout)
    {
        giles.Set_Timeout(m_timeout.value());
    }

    giles.Run();
    return 0;
}
