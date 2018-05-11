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
    @brief This file contains the Factory_Register class, which is used to
    assist with registering classes within the Factory class
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef FACTORY_REGISTER_HPP
#define FACTORY_REGISTER_HPP

#include <map>       // for map
#include <memory>    // for make...
#include <stdexcept> // for inva...
#include <string>    // for oper...

#include "Model_Factory.hpp"

namespace ELMO2
{
namespace Internal
{
//! @brief This class is used to assist with the self registration of classes
//! within the factory.
//! The idea is that a class would inherit from this, using itself as the
//! template typename. This will add the bool m_is_registered to it. At run
//! time, m_is_registered will be evaluated due to the fact that it is staitic.
//! This evaulation will call the Register function in the Factory class,
//! registering the type given by T. i.e. The class that derived from this.
//! @see https://www.bfilipek.com/2018/02/factory-selfregister.html
template <typename T> class Factory_Register
{
protected: // TODO: Should this be protected??? It is protected in example
    //! This static variable is evaluated before main() is called. The
    //! evaluation of this requires the Register function to be called,
    //! automatically registering the class defined by the template T in the
    //! factory before main() is called.
    static bool m_is_registered;
};

template <typename T>
bool ELMO2::Internal::Factory_Register<T>::m_is_registered =
    ELMO2::Internal::Model_Factory::Register(
        T::Get_Name(),
        /*
         *[](const ELMO2::Internal::Execution& p_execution,
         *   const ELMO2::Internal::Coefficients& p_coefficients) {
         *    return std::make_unique<ELMO2::Internal::Model_TEMPLATE>(
         *        p_execution, p_coefficients);
         *});
         */
        T::Create_Function); // TODO: Replace T::Create_Function( with lamba
// function @see
// http://coliru.stacked-crooked.com/a/de4467d26dd02b42
} // namespace Internal
} // namespace ELMO2

#endif
