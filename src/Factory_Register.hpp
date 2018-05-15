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

#include <memory>        // for unique_ptr, make_unique
#include <string>        // for oper...
#include <unordered_map> // forunordered_map

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
template <typename Derived> class Factory_Register
{
private:
    //! @brief This calls the constructor of the derived class. A pointer to
    //! this funtion is passed to the factory class so that it can create
    //! instances of the self registering derived classes.
    //! @param p_execution The recored Execution of the simulation of the target
    //! program.
    //! @param p_coefficients The coefficients as loaded from the Coefficients
    //! file.
    //! @returns A unique_ptr to an object of the type given by the Derived
    //! template.
    static std::unique_ptr<Model>
    create(const ELMO2::Internal::Execution& p_execution,
           const ELMO2::Internal::Coefficients& p_coefficients)
    {
        return std::make_unique<Derived>(p_execution, p_coefficients);
    }

    //! @brief Retrieves the name of this Model.
    //! @returns The name as a string.
    //! This is needed to ensure self registration in the factory works. As
    //! such, this implementation ensures that all classes that wish to self
    //! register must provide an implementation for Get_Name() because the
    //! factory registration requires this as unique identifier.
    //! @note This idiom is known as the curiously recurring template pattern.
    //! @see https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    static const std::string get_name() { return Derived::Get_Name(); }

protected: // TODO: Should this be protected?
    //! This static variable is evaluated before main() is called. The
    //! evaluation of this requires the Register function to be called,
    //! automatically registering the class defined by the template T in the
    //! factory before main() is called.
    static bool m_is_registered;

    //! @brief This has been made protected to ensure the constructor and the
    //! copy constructor cannot be called directly as this class is designed to
    //! be inherited from and not instantiated directly.
    Factory_Register() = default;

    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Factory_Register() =
        default; // TODO: Should this = default? Rule of
    // ZERO? http://en.cppreference.com/w/cpp/language/rule_of_three
};

template <typename Derived>
bool ELMO2::Internal::Factory_Register<Derived>::m_is_registered =
    ELMO2::Internal::Model_Factory::Register(Derived::Get_Name(), create);
} // namespace Internal
} // namespace ELMO2

#endif
