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
    @todo Change this
    @file Model_Hamming_Weight.hpp
    @brief This file is a template. This can be used as a base for implementing
    a mathematical model for generating traces of a given target program.
    @note When implementing a model there is no need to change much in this
    file. Only the word "Template" needs to be changed in various places as
    well as the comments. The code that needs to be changed is mainly contained
    to the corresponding .cpp file.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_HAMMING_WEIGHT_HPP
#define MODEL_HAMMING_WEIGHT_HPP

#include <string>         // for string
#include <unordered_set>  // for unordered_set

#include "Model.hpp"                   // for Model
#include "Model_Factory_Register.hpp"  // for Model_Factory_Register
#include "Traces.hpp"                  // for Traces

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
class Coefficients;
class Execution;

//! @class Model_Hamming_Weight
//! @brief This derived class contains a specific implementation of a
//! mathematical model to calculate the traces for a target program. It is
//! designed as a template allowing new models to be added with ease.
//! Deriving from Model_Factory_Register as well will automatically register
//! this class within the factory class.
class Model_Hamming_Weight : public virtual ELMO2::Internal::Model,
                             public ELMO2::Internal::Model_Factory_Register<
                                 ELMO2::Internal::Model_Hamming_Weight>
{
private:
    static const std::unordered_set<std::string> m_required_interaction_terms;

    //! @brief Retrieves a list of the interaction terms that are used within
    //! the model. These must be provided by the Coefficients in order for
    //! the model to function.
    //! @returns The list of interaction terms used within the model.
    const std::unordered_set<std::string>&
    Get_Interaction_Terms() const override
    {
        return m_required_interaction_terms;
    }

public:
    //! @brief The constructor makes use of the base Model constructor to assist
    //! with initialisation of private member variables.
    Model_Hamming_Weight(const ELMO2::Internal::Execution& p_execution,
                         const ELMO2::Internal::Coefficients& p_coefficients)
        : ELMO2::Internal::Model(p_execution, p_coefficients)
    {
        // This is required to be "used" somewhere in order to prevent the
        // compiler from optimising it away, thus preventing self registration.
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
        // Section 6.6.4.1, point 2 states that this statment will not be
        // optimised away.
        m_is_registered;

        // TODO: This can be moved up into Model if CTRP is used. CTRP can work
        // if a non template interface class is introduced.
        if (!this->Check_Interaction_Terms())
        {
            throw std::logic_error(
                "Model was not provided with correct "
                "interaction terms by the Coefficients file.");
        }
    }

    const ELMO2::Internal::Traces Generate_Traces() const override;

    //! @brief Retrieves the name of this Model.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory works.
    //! The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Hamming Weight"; }
};
}  // namespace Internal
}  // namespace ELMO2

#endif
