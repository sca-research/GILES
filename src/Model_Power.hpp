/*
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file Model_Power.hpp
    @brief Contains a mathematical model for calculating the power usage of the
    target program.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#ifndef MODEL_POWER_HPP
#define MODEL_POWER_HPP

#include "Coefficients.hpp"
#include "Execution.hpp"
#include "Model.hpp"

namespace ELMO2
{
//! @class Model_Power
//! @brief This derived class conatins a specific implementation of a
//! mathematical model to calculate the power usage of a target program.
//! TODO: Find out more about this to be more descriptive.
class Model_Power : public virtual ELMO2::Model
{
public:
    //! @brief The constructor makes use of the base Model constructor to assist
    //! with initilisation of private memeber variables.
    Model_Power(const ELMO2::Execution& p_execution,
                const ELMO2::Coefficients& p_coefficients)
        : ELMO2::Model(p_execution, p_coefficients)
    {
    }

    //! @brief This function contains the mathematical calculations that
    //! generate the power Traces.
    //! TODO: Find out more in order to improve this description.
    //! @returns The generted Traces for the target program.
    const ELMO2::Traces& GenerateTraces() override;
};
} // namespace ELMO2

#endif
