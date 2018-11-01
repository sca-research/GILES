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
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <memory>         // for make_unique, unique_ptr
#include <optional>       // for optional
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <unordered_set>  // for unordered_set
#include <utility>        // for pair

#include <Traces_Serialiser.hpp>

#include <iostream>  // for temp debugging TODO: Remove this

#include "Coefficients.hpp"        // for Coefficients
#include "Emulator_Factory.hpp"    // for Emulator_Factory
#include "Emulator_Interface.hpp"  // for Emulator_Interface
#include "Execution.hpp"           // for Execution
#include "IO.hpp"                  // for IO
#include "Model_Factory.hpp"       // for Model_Factory
#include "Unicorn_Interface.hpp"   // for Unicorn_Interface
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
    const ELMO2::Internal::IO io;

    //! @brief Re-orders the assembly instructions.
    //! Re-ordering of the instructions is necessary as the order they are
    //! executed in is not the order that they were written in/compiled into.
    //! @note Out of order execution is supported by certain simulators making
    //! this redundant.
    //! TODO: Maybe look into using a disassembler e.g. capstone to work around
    //! this issue?
    //! TODO: Should optionally accept the target program (Using the decided
    //! upon format (binary/assembly)) as overloaded function
    void re_order_assembly(const std::string& p_program_path)
    {
        throw("Function not yet implemented");
    }

    //! @brief Interprets options passed to ELMO2.
    //! @param p_options The options as contained within an unordered map. The
    //! key is the option and the value is what that option is set to.
    //! TODO: Change the return type? At least to bool i.e. successful or not?
    void
    parse_options(const std::unordered_map<std::string, std::string>& p_options)
    {
        // throw("Function not yet implemented");
    }

    //! @todo Optimise this using std methods. Can be reduced down to
    //! std::remove_if or something similar.
    //! @todo This is not needed. Using a lazy approach we can only check the
    //! constructed model.
    //! @todo maybe move to Model_Factory.hpp
    const std::unordered_set<std::string> get_models_in_use()
    {
        std::unordered_set<std::string> models_in_use;
        // Create a list of models to use from a list of all models compiled
        for (const auto& model : ELMO2::Internal::Model_Factory::Get_All())
        {
            std::cout << "Found Model: " << model.first << std::endl;
            // if this model is enabled.
            if (model.second)  // TODO: model.second is currently a function
                               // pointer to the constructor, not is_enabled
                               // bool.
            {
                // Add it to the list of models in use.
                models_in_use.insert(model.first);
            }
        }
        return models_in_use;
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
           const std::uint32_t p_number_of_runs)
        : io(ELMO2::Internal::IO())
    {
        // parse_options(p_options);

        // TODO: Move all of this out of the constructor?

        const ELMO2::Internal::Coefficients coefficients =
            io.Load_Coefficients(p_coefficients_path);

        // Initialise all emulators.
        for (const auto& emulator_interface :
             ELMO2::Internal::Emulator_Factory::Get_All())
        {
            std::cout << "Found Emulator: " << emulator_interface.first
                      << std::endl;

            std::vector<std::vector<float>> traces;

            for (std::size_t i = 0; i < p_number_of_runs; ++i)
            {
                // Construct the simulator, ready for use
                const auto simulator =
                    ELMO2::Internal::Emulator_Factory::Construct(
                        emulator_interface.first, p_program_path);

                const auto execution = simulator->Run_Code();

                // Initialise all models.
                for (const auto& model_interface :
                     ELMO2::Internal::Model_Factory::Get_All())
                {
                    std::cout << "Found Model: " << model_interface.first
                              << std::endl;

                    // Construct the model, ready for use
                    const auto model =
                        ELMO2::Internal::Model_Factory::Construct(
                            model_interface.first, execution, coefficients);

                    traces.emplace_back(model->Generate_Traces());
                }
            }

            // If a path was provided then save
            if (p_traces_path)
            {
                // Save to file.
                Traces_Serialiser::Serialiser serialiser({traces});
                serialiser.Save(p_traces_path.value());
            }
        }
    }
};
}  // namespace ELMO2
