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
    @file Abstract_Factory_Register.hpp
    @brief This file contains the Abstract_Factory_Register class, which is used
    to assist with registering classes within the Abstract_Factory class.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ABSTRACT_FACTORY_REGISTER_HPP
#define ABSTRACT_FACTORY_REGISTER_HPP

#include <memory>  // for unique_ptr, make_unique
#include <string>  // for string

#include "Abstract_Factory.hpp"

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
// These are need to instantiate the templates at the bottom of the file.
class Model;
class Execution;
class Coefficients;
class Emulator;

//! @class Abstract_Factory_Register
//! @brief This class is used to assist with the self registration of classes
//! within the factory.
//! The idea is that a class would inherit from this, using itself as the
//! template typename. This will add the bool m_is_registered to it. At run
//! time, m_is_registered will be evaluated due to the fact that it is static.
//! The evaulation of this will call the Register function in the Factory class,
//! registering the type given by T. i.e. The class that derived from this.
//! @see https://www.bfilipek.com/2018/02/factory-selfregister.html
template <typename Base, typename derived_t, typename... args_t>
class Abstract_Factory_Register
{
private:
    //! @brief This calls the constructor of the Derived class. A pointer to
    //! this function is passed to the factory class so that it can create
    //! instances of the self registering derived classes.
    //! @param p_execution The recorded Execution of the simulation of the
    //! target program.
    //! @param p_coefficients The coefficients as loaded from the Coefficients
    //! file.
    //! @returns A unique_ptr to an object of the type given by the derived_t
    //! template.
    static std::unique_ptr<Base> create(args_t... p_args)
    {
        return std::make_unique<derived_t>(p_args...);
    }

    //! @brief Retrieves the name of the Derived class.
    //! @returns The name as a string.
    //! This is needed to ensure self registration in the factory works. As
    //! such, this implementation ensures that all classes that wish to self
    //! register must provide an implementation for Get_Name() because the
    //! factory registration requires this as unique identifier.
    //! @note This idiom is known as the curiously recurring template pattern.
    //! @see https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    static const std::string get_name() { return derived_t::Get_Name(); }

public:  // TODO: Should this be protected?
    //! This static variable is evaluated before main() is called. The
    //! evaluation of this requires the Register function to be called,
    //! automatically registering the class defined by the template T in the
    //! factory before main() is called.
    static bool m_is_registered;

protected:
    //! @brief This has been made protected to ensure the constructor and
    //! the copy constructor cannot be called directly as this class is
    //! designed to be inherited from and not instantiated directly.
    Abstract_Factory_Register()
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
        (void)derived_t::m_is_registered;
    }

    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Abstract_Factory_Register() =
        default;  // TODO: Should this = default? Rule of
    // ZERO? http://en.cppreference.com/w/cpp/language/rule_of_three
};

//! @todo Does this inherit the doxygen comments from m_is_registered? - if not,
//! add doxygen comments.
//! @todo Add comment explaining why this is outside the class.
template <typename Base, typename derived_t, typename... args_t>
bool ELMO2::Internal::Abstract_Factory_Register<Base,
                                                derived_t,
                                                args_t...>::m_is_registered =
    Abstract_Factory<Base, args_t...>::Register(derived_t::Get_Name(), create);

//! @brief This exists only to simply the usage of the
//! Abstract_Factory_Register class. By providing an intermediate, the
//! possibility of accidentally initialising a separate template is
//! eliminated. To see what is actually going on behind the scenes, refer to the
//! Abstract_Factory_Register class.
template <typename derived_t>
using Model_Factory_Register = Abstract_Factory_Register<Model,
                                                         derived_t,
                                                         const Execution&,
                                                         const Coefficients&>;

//! @brief This exists only to simply the usage of the
//! Abstract_Factory_Register class. By providing an intermediate, the
//! possibility of accidentally initialising a separate template is
//! eliminated. To see what is actually going on behind the scenes, refer to the
//! Abstract_Factory_Register class.
template <typename derived_t>
using Emulator_Factory_Register =
    Abstract_Factory_Register<Emulator, derived_t, const std::string&>;
}  // namespace Internal
}  // namespace ELMO2

#endif
