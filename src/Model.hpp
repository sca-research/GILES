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
    @file Model.hpp
    @brief Contains the Model class which serves as a base case for mathematical
    models to be implemented.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_HPP
#define MODEL_HPP

#include "Coefficients.hpp"
#include "Execution.hpp"
#include "Traces.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Model
//! @brief An abstract class that serves as a base class for mathematical models
//! to be implemented, These models will generate the traces for the given
//! program by making use of the Coefficients and the recorded Execution of the
//! program.
class Model
{
protected:
    //! The execution of the target program as recorded by the Emulator.
    const ELMO2::Internal::Execution m_execution;

    //! The Coefficients created by measuring real hardware traces.
    const ELMO2::Internal::Coefficients& m_coefficients;

    //! @brief This function will add noise to the traces to simulate the
    //! background noise that would be picked up in real traces.
    //! @param p_traces The Traces to have the noise added to.
    //! @returns The Traces with the added noise.
    const ELMO2::Internal::Traces& addNoise(ELMO2::Internal::Traces& p_traces);

    //! @brief The constructor needs to be provided with the recorded
    //! Execution of the target program and the details from real world
    //! traces, the Coefficients, to be able to calculate the Traces for the
    //! target program. This constructor is marked as protected as it should
    //! only be called by derived classes to assist with initialisation.
    //! @param p_execution The recorded Execution of the target program,
    //! provided by the Emulator.
    //! @param p_coefficients The loaded Coefficients from real hardware
    //! traces.
    Model(const ELMO2::Internal::Execution& p_execution,
          const ELMO2::Internal::Coefficients& p_coefficients)
        : m_execution(p_execution), m_coefficients(p_coefficients)
    {
    } // TODO: Should this read Model() =
    // 0??? And have a generate_traces()
    // method to do this instead?

    //! @brief Retrieves the interaction terms used within the model. This is
    //! required to ensure the Coefficients file provided all of the required
    //! interaction terms.
    //! @returns A list of required interaction terms as defined in the
    //! derived class.
    virtual const std::unordered_set<std::string>&
    Get_Interaction_Terms() const = 0;

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Model() = default; // TODO: Should this = default? Rule of
    // ZERO? http://en.cppreference.com/w/cpp/language/rule_of_three

    //! @brief In derived classes, this function should contain the mathematical
    //! calculations that generate the Traces.
    //! @returns The generated Traces for the target program.
    virtual const ELMO2::Internal::Traces& Generate_Traces() const = 0;

    //! @brief Ensures that all the interaction terms used within the model
    //! are provided by the Coefficients.
    //! @returns True if the all the interaction terms required by the model
    //! are contained within the Coefficients.
    bool Find_Interaction_Terms() const
    {
        const auto& models_terms = Get_Interaction_Terms();
        const auto& coefficients_terms = m_coefficients.Get_Interaction_Terms();

        return std::includes(coefficients_terms.begin(),
                             coefficients_terms.end(),
                             models_terms.begin(),
                             models_terms.end());
    }
};
} // namespace Internal
} // namespace ELMO2

#endif
