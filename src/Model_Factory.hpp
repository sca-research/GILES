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
    @file Model_Factory.hpp
    @brief Contains the Model_Factory class; a factory class for initialisation
    of models. This file exists only to simplify the syntax of using the
    Abstract_Factory class and ensure that all models are included in the
    compilation.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include "Abstract_Factory.hpp"  // for Abstract_Factory
#include "Model.hpp"             // for Model

namespace ELMO2
{
namespace Internal
{

//! @class Model_Factory
//! @brief This class exists only to simply the usage of the Abstract_Factory
//! class. By providing an intermediate class, the possibility of accidentally
//! initialising a separate template is eliminated. Additionally, this
//! provides for a more meaningful name. To see what is actually going on behind
//! the scenes, refer to the Abstract_Factory class.
class Model_Factory
    : public ELMO2::Internal::Abstract_Factory<
          ELMO2::Internal::Model,
          const ELMO2::Internal::Execution&,
          const ELMO2::Internal::Coefficients&>  // TODO: public virtual
                                                 // instead?
{
    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_constructor
    Model_Factory(const Model_Factory&) = delete;

    //! @brief This has been deleted to ensure the copy
    //! assignment operator cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_assignment
    Model_Factory& operator=(const Model_Factory&) = delete;
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // MODEL_FACTORY_HPP
