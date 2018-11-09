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
    @file Model_Power.hpp
    @brief Contains a mathematical model for calculating the power usage of the
    target program.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_POWER_HPP
#define MODEL_POWER_HPP

#include "Coefficients.hpp"
#include "Execution.hpp"
#include "Model.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Model_Power
//! @brief This derived class contains a specific implementation of a
//! mathematical model to calculate the power usage of a target program.
//! TODO: Find out more about this to be more descriptive.
class Model_Power : public virtual ELMO2::Internal::Model
{
public:
    //! @brief The constructor makes use of the base Model constructor to assist
    //! with initialisation of private member variables.
    Model_Power(const ELMO2::Internal::Execution& p_execution,
                const ELMO2::Internal::Coefficients& p_coefficients)
        : ELMO2::Internal::Model(p_execution, p_coefficients)
    {
        // This statement registers this class in the factory, allowing access
        // from elsewhere. Do not delete this or else this class will not appear
        // in the factory. If you wish to make this class inaccessible, a better
        // method would be to remove the corresponding cpp file from the build
        // script.
        // This is required to be "used" somewhere in order to prevent
        // the compiler from optimising it away, thus preventing self
        // registration.
        // Section 6.6.4.1, point 2 of the linked document states that this
        // statement will not be optimised away.
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
        // The void cast does nothing functionally but prevents the compiler
        // warning about an unused result.
        (void)m_is_registered;

        // TODO: This can be moved up into Model if CTRP is used. CTRP can work
        // if a non template interface class is introduced.
        if (!this->Check_Interaction_Terms())
        {
            throw std::logic_error(
                "Model was not provided with correct "
                "interaction terms by the Coefficients file.");
        }
    }

    //! @brief This function contains the mathematical calculations that
    //! generate the power Traces.
    //! TODO: Find out more in order to improve this description.
    //! @returns The generated Traces for the target program.
    const ELMO2::Internal::Traces& Generate_Traces() override {}
};
}  // namespace Internal
}  // namespace ELMO2

#endif
