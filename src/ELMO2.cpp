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
    @file ELMO2.cpp
    @brief This file contains the entry point for ELMO2 and invokes other
    operations.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <memory>         // for make_unique, unique_ptr
#include <optional>       // for optional
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <unordered_set>  // for unordered_set
#include <utility>        // for pair, move

#include <Traces_Serialiser.hpp>
#include <fmt/format.h>  // for print

#include "Coefficients.hpp"        // for Coefficients
#include "Emulator_Factory.hpp"    // for Emulator_Factory
#include "Emulator_Interface.hpp"  // for Emulator_Interface
#include "Error.hpp"               // for Report_Error
#include "Execution.hpp"           // for Execution
#include "IO.hpp"                  // for IO
#include "Model_Factory.hpp"       // for Model_Factory
namespace ELMO2
{
// Forward Declarations
namespace Internal
{
class Model;
}  // namespace Internal

//! @class ELMO_2
//! @brief This contains the operations that control the rest of the program
//! passing data between them.
class ELMO_2
{
private:
    const ELMO2::Internal::Coefficients m_coefficients;
    const std::string m_program_path;
    const std::string m_model_name;
    const std::string m_simulator_name;
    const std::optional<std::string>& m_traces_path;
    const std::uint32_t m_number_of_runs;

    // Future: This data is stored here as well as in Traces_Serialiser as it
    // should be able to be accessed programmatically in the future.
    // TODO: Add getter.
    std::vector<std::vector<float>> m_traces;
    std::vector<std::string> m_extra_data;

    Traces_Serialiser::Serialiser<float> m_serialiser;

    // TODO: Future: This has been left in as it will be used in future versions
    // when running multiple models at once is supported.
    //! @todo Optimise this using std methods. Can be reduced down to
    //! std::remove_if or something similar.
    //! @todo This is not needed. Using a lazy approach we can only check the
    //! constructed model.
    //! @todo maybe move to Model_Factory.hpp
    /*const std::unordered_set<std::string> get_models_in_use()
    {
        std::unordered_set<std::string> models_in_use;
        // Create a list of models to use from a list of all models compiled
        for (const auto& model : ELMO2::Internal::Model_Factory::Get_All())
        {
            std::cout << "Found Model: " << model.first << std::endl;
            // if this model is enabled.
            tor_TE
            if (model.second)  // TODO: model.second is currently a function
                               // pointer to the constructor, not is_enabled
                               // bool.
            {
            // Add it to the list of models in use.
            models_in_use.insert(model.first);
            //}
        }
        return models_in_use;
    }*/

    //! @brief Prints a warning if the target program does not run in a constant
    //! number of clock cycles each time it is executed.
    //! @returns true if a warning was printed, false if not.
    //! @param p_trace_index The index of the current trace to have its size
    //! checked.
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    //! @todo: Future: This should only be checked if TRS files are being used.
    bool warn_if_not_constant_time() const
    {
        const auto current_size      = m_traces.back().size();
        static const auto first_size = m_traces.front().size();

        // If there is no size difference then return false.
        if (first_size == current_size)
        {
            return false;
        }

        ELMO2::Internal::Error::Report_Warning(
            "The target program did not run in a constant number of cycles.\n"
            "If this was not an intentional countermeasure to timing attacks "
            "then this is considered insecure.\n"
            "Trace number 0 took {} clock cycles.\n"
            "Trace number {} took {} clock cycles.\n",
            first_size,
            m_traces.size() - 1,  // Trace index
            current_size);
        return true;
    }

public:
    // TODO: Separate out some of the functionality in here into an API
    //! @brief The main entry point to the ELMO2 library. This controls the
    //! running of ELMO2 and invokes other components.
    //! @param p_program_path The path to the target executable to be ran in
    //! the emulator.
    //! @param p_coefficients_path The path to the Coefficients file.
    //! @param p_traces_path The path to save the Traces to. This is an
    //! optional parameter and omitting it will cause the traces to not be
    //! saved to a file.
    ELMO_2(const std::string& p_program_path,
           const std::string& p_coefficients_path,
           const std::optional<std::string>& p_traces_path,
           const std::uint32_t p_number_of_runs,
           const std::string& p_model_name =
               "Hamming Weight")  // TODO: Set the default using cmake
                                  // configuring a static var in an external
                                  // file.
    : m_coefficients{ELMO2::Internal::IO().Load_Coefficients(
          p_coefficients_path)},
      m_program_path{p_program_path}, m_model_name{std::move(p_model_name)},
      m_traces_path{p_traces_path}, m_number_of_runs{p_number_of_runs},
      m_serialiser{}
    {
        // Check the supplied model name is valid
        ELMO2::Internal::Model_Factory::Find(p_model_name);
    }

    //! @todo Document
    void Run()
    {
        // Initialise all emulators.
        for (const auto& emulator_interface :
             ELMO2::Internal::Emulator_Factory::Get_All())
        {
            fmt::print("Using simulator: {}\n", emulator_interface.first);

            // Run the emulator and save the results to m_traces.
            Run_Simulator(emulator_interface.first);

            // If a path was provided then save.
            if (m_traces_path)
            {
                // Save to file.
                m_serialiser.Save(m_traces_path.value());
            }
        }
    }

    //! @brief Runs the simulator given by p_simulator_name and TODO:
    decltype(m_traces) Run_Simulator(const std::string& p_simulator_name)
    {
        fmt::print("Using model: {}\n", m_model_name);

        // Enusres that the constant time warning is not printed over and over.
        bool warning_printed{false};

        // Used to indicate progress to the user. 'i' is not used as it is not
        // thread safe. This is.
        uint32_t steps_completed{0};

#pragma omp parallel for
        for (std::size_t i = 0; i < m_number_of_runs; ++i)
        {
            // Construct the simulator, ready for use.
            const auto simulator = ELMO2::Internal::Emulator_Factory::Construct(
                p_simulator_name, m_program_path);

            const auto execution = simulator->Run_Code();

            // Any extra data to be included in the trace.
            const auto extra_data = simulator->Get_Extra_Data();

            // Initialise all models.
            // TODO: Future: Add support for using multiple models at once using
            // this code.
            /*for (const auto& model_interface :
                 ELMO2::Internal::Model_Factory::Get_All())
            {

            // Construct the model, ready for use.
            const auto model = ELMO2::Internal::Model_Factory::Construct(
                model_interface.first, execution, m_coefficients);*/

            // Construct the model, ready for use.
            const auto model = ELMO2::Internal::Model_Factory::Construct(
                m_model_name, execution, m_coefficients);

            // If this is not the first trace gathered then ensure that all
            // traces are the same length (Meaning the target algorithm runs
            // in constant time). This is a requirement for using the TRS
            // trace format.
            const auto trace = model->Generate_Traces();

            // Increment the counter of number of traces generated.
#pragma omp atomic
            ++steps_completed;

// This is marked critical to ensure everything gets added and
// locks are automatically handled.
#pragma omp critical
            {
                // Add the generated trace to the list of traces.
                m_traces.emplace_back(trace);

                // Add any extra information given by the simulator to the
                // traces.
                m_extra_data.emplace_back(extra_data);

                m_serialiser.Add_Trace(trace, extra_data);

                // If this warning hasn't been printed before.
                if (!warning_printed)
                {
                    // Will print a warning if the target program is not
                    // constant time.
                    warning_printed = warn_if_not_constant_time();
                }
            }

            fmt::print("\rGenerated: {} of {} traces. ({}%)",
                       steps_completed,
                       m_number_of_runs,
                       100.0 * steps_completed / m_number_of_runs);
            //}
        }
        fmt::print("\nDone!\n");
        return m_traces;
    }
};
}  // namespace ELMO2
