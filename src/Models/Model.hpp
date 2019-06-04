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
    @file Model.hpp
    @brief Contains the Model class which serves as a base case for mathematical
    models to be implemented.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>      // for all_of
#include <string>         // for string
#include <unordered_set>  // for unordered_set
#include <vector>         // for vector

#include "Abstract_Factory_Register.hpp"  // for Model_Factory_Register
#include "Coefficients.hpp"
#include "Execution.hpp"
#include "Model_Math.hpp"

namespace GILES
{
namespace Internal
{
class Model
{
protected:
    //! The execution of the target program as recorded by the Emulator.
    const GILES::Internal::Execution m_execution;

    //! The Coefficients created by measuring real hardware traces.
    const GILES::Internal::Coefficients& m_coefficients;

    Model(const GILES::Internal::Execution& p_execution,
          const GILES::Internal::Coefficients& p_coefficients)
        : m_execution(p_execution), m_coefficients(p_coefficients)
    {
    }

public:
    //! @brief In derived classes, this function should contain the
    //! mathematical calculations that generate the Traces.
    //! @returns The generated Traces for the target program.
    virtual const std::vector<float> Generate_Traces() const = 0;

    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Model() = default;
};

//! @class Model
//! @brief An abstract class that serves as a base class for mathematical models
//! to be implemented, These models will generate the traces for the given
//! program by making use of the Coefficients and the recorded Execution of the
//! program.
template <typename derived_t>
class Model_Interface : public Model, public Model_Factory_Register<derived_t>
{
protected:
    //! @brief The constructor needs to be provided with the recorded
    //! Execution of the target program and the details from real world
    //! traces, the Coefficients, to be able to calculate the Traces for the
    //! target program. This constructor is marked as protected as it should
    //! only be called by derived classes to assist with initialisation.
    //! @param p_execution The recorded Execution of the target program,
    //! provided by the Emulator.
    //! @param p_coefficients The loaded Coefficients from real hardware
    //! traces.
    Model_Interface(const GILES::Internal::Execution& p_execution,
                    const GILES::Internal::Coefficients& p_coefficients)
        : Model(p_execution, p_coefficients)
    {
        if (!Check_Interaction_Terms())
        {
            throw std::logic_error(
                "Model was not provided with correct "
                "interaction terms by the Coefficients file.");
        }
    }

    //! @brief Retrieves the interaction terms used within the model. This is
    //! required to ensure the Coefficients file provided all of the required
    //! interaction terms.
    //! @returns A list of required interaction terms as defined in the
    //! derived class.
    static const std::unordered_set<std::string>& Get_Interaction_Terms()
    {
        return derived_t::Get_Interaction_Terms();
    };

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Model_Interface() = default;

    //! @brief Ensures that all the interaction terms used within the model
    //! are provided by the Coefficients.
    //! @returns True if the all the interaction terms required by the model
    //! are contained within the Coefficients and false if not.
    bool Check_Interaction_Terms() const
    {
        const auto& models_terms       = Get_Interaction_Terms();
        const auto& coefficients_terms = m_coefficients.Get_Interaction_Terms();

        // If there are less terms in the Coefficients then there are
        // missing terms.
        return models_terms.size() <= coefficients_terms.size() &&
               // For each term the model uses, check it is contained within
               // the set of terms provided by the Coefficients.
               std::all_of(models_terms.begin(),
                           models_terms.end(),
                           // A lambda function for checking if an
                           // individual element is in coefficients_terms.
                           [&coefficients_terms](const auto& term) {
                               return coefficients_terms.end() !=
                                      coefficients_terms.find(term);
                           });
    }
};
}  // namespace Internal
}  // namespace GILES

#endif
