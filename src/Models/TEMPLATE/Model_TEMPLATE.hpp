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
    @file Model_TEMPLATE.hpp
    @brief This file is a template. This can be used as a base for implementing
    a mathematical model for generating traces of a given target program.
    @note When implementing a model there is no need to change much in this
    file. Only the word "Template" needs to be changed in various places as
    well as the comments. The code that needs to be changed is mainly contained
    to the corresponding .cpp file.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_TEMPLATE_HPP
#define MODEL_TEMPLATE_HPP

#include <string>         // for string
#include <unordered_set>  // for unordered_set
#include <vector>         // for vector

#include "Model.hpp"  // for Model_Interface

namespace GILES
{
namespace Internal
{
// Forward Declarations
class Coefficients;
class Execution;

//! @class Model_TEMPLATE
//! @brief This derived class contains a specific implementation of a
//! mathematical model to calculate the traces for a target program. It is
//! designed as a template allowing new models to be added with ease.
//! Deriving from Model_Factory_Register as well will automatically register
//! this class within the factory class.
class Model_TEMPLATE : public virtual Model_Interface<Model_TEMPLATE>
{
private:
    static const std::unordered_set<std::string> m_required_interaction_terms;

public:
    //! @brief The constructor makes use of the base Model constructor to assist
    //! with initialisation of private member variables.
    Model_TEMPLATE(const GILES::Internal::Execution& p_execution,
                   const GILES::Internal::Coefficients& p_coefficients)
        : Model_Interface<Model_TEMPLATE>(p_execution, p_coefficients)
    {
    }

    const std::vector<float> Generate_Traces() const override;

    //! @brief Retrieves a list of the interaction terms that are used within
    //! the model. These must be provided by the Coefficients in order for
    //! the model to function.
    //! @returns The list of interaction terms used within the model.
    static const std::unordered_set<std::string>& get_interaction_terms()
    {
        return m_required_interaction_terms;
    }

    //! @brief Retrieves the name of this Model.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory works.
    //! The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "TEMPLATE"; }
};
}  // namespace Internal
}  // namespace GILES

#endif
