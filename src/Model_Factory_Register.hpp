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
    @file Model_Factory_Register.hpp
    @brief Contains the Model_Factory_Register class, which is used
    to assist with registering classes within the Factory class. This file
    exists only to simplify the synatx of using the Abstract_Factory_Register
    class.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_FACTORY_REGISTER_HPP
#define MODEL_FACTORY_REGISTER_HPP

#include "Abstract_Factory_Register.hpp" // for Abstract_Factory_Register
#include "Model.hpp"                     // for Model

namespace ELMO2
{
namespace Internal
{

//! @class Model_Factory_Register
//! @brief This class exists only to simply the usage of the
//! Abstract_Factory_Register class. By providing an intermidiate class, the
//! possibility of accidentally initialisating a seperate template is
//! eliminated. To see what is actually going on behind the scenes, refer to the
//! Abstract_Factory_Register class.
template <class Derived>
class Model_Factory_Register
    : public ELMO2::Internal::Abstract_Factory_Register<
          ELMO2::Internal::Model,
          Derived,
          const ELMO2::Internal::Execution&,
          const ELMO2::Internal::Coefficients&> // TODO: public virtal instead?
{

protected:
    //! @brief This has been made protected to ensure the constructor cannot be
    //! called directly as this class is designed to be inherited from and not
    //! instantiated directly.
    Model_Factory_Register() = default;
};
} // namespace Internal
} // namespace ELMO2

#endif
