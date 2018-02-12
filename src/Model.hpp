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
    @brief Contains the Model class which serves as a base case for mathmatical
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
//! @brief An abstract class that serves as a base class for mathmatical models
//! to be implemented, These models will generate the traces for the given
//! program by making use of the Coefficients and the recorded Execution of the
//! program.
class Model
{
private:
    //! The execution of the traget program as recorded by the Emulator.
    const ELMO2::Internal::Execution m_execution;

    //! The Coefficients created by measuring real hardware traces.
    const ELMO2::Internal::Coefficients m_coefficients;

    //! @brief This function will add noise to the traces to simulate the
    //! background noise that would be picked up in real traces.
    //! @ param The Traces to have the noise added to.
    //! @returns The Traces with the added noise.
    const ELMO2::Internal::Traces& addNoise(ELMO2::Internal::Traces& p_traces);

protected:
    //! @brief The constructor needs to be provided with the recorded Execution
    //! of the target program and the details from real world traces, the
    //! Coefficients to be able to calculate the Traces for the target program.
    //! This contructor is marked as protected as it should only be
    //! called by derived classes to assist with initilisation.
    //! @param p_execution The recorded Execution of the target program,
    //! provided by the Emulator.
    //! @param p_coefficients The loaded Coefficients from real hardware traces.
    Model(const ELMO2::Internal::Execution& p_execution,
          const ELMO2::Internal::Coefficients& p_coefficients)
        : m_execution(p_execution), m_coefficients(p_coefficients)
    {
    } // TODO: Should this read Model() =
    // 0??? And have a generate_traces()
    // method to do this instead?

public:
    //! @brief Virtual deconstructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Model(); // TODO: Should this = default? Rule of ZERO?
                      // http://en.cppreference.com/w/cpp/language/rule_of_three

    //! @brief In derived classes, this function should contain the mathematical
    //! calculations that generate the Traces.
    //! @returns The generted Traces for the target program.
    virtual const ELMO2::Internal::Traces& GenerateTraces() = 0;
};
} // namespace Internal
} // namespace ELMO2

#endif
