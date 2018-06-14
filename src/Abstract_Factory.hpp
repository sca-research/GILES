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
    @file Abstract_Factory.hpp
    @brief Contains the Abstract_Factory class; a factory class for
    initialisation of objects.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ABSTRACT_FACTORY_HPP
#define ABSTRACT_FACTORY_HPP

#include <memory>         // for unique_ptr
#include <string>         // for string
#include <unordered_map>  // for unordered_map

namespace ELMO2
{
namespace Internal
{

//! @class Abstract_Factory
//! @brief This is a static factory class that assists in the construction of
//! objects. By providing abstraction for the caller, it hides the exact type of
//! the object from the caller and allows the caller to make use of the object
//! through the Base class interface regardless of the derived type.
//! @note The exact classes that this can construct are not stored here and are
//! added at runtime using self-registering techniques. This simplifies the
//! process of adding and removing classes and allows objects to be selectively
//! compiled as a way to completely remove those as options. This si also a
//! template class allowing it to construct any object and not just those of a
//! fixed type.
//! @see https://en.wikipedia.org/wiki/Factory_method_pattern
//! @see https://www.bfilipek.com/2018/02/factory-selfregister.html
template <class Base, typename... Args> class Abstract_Factory
{
public:
    //! A function pointer used to point to the function that calls the
    //! constructor for objects in the factory.
    using Create_Function = std::unique_ptr<Base> (*)(Args...);

    //! @brief This function assists with the initialisation of the objects in
    //! the factory. This will be passed the parameters to be passed on to the
    //! constructor. It will selectively construct an object based on the
    //! first parameter, p_type and return a unique pointer to it.
    //! @param p_type The type of the object to be constructed as a
    //! string.
    //! @param p_execution The Execution as received from the emulator.
    //! @param p_coefficients The Coefficients as loaded from the
    //! coefficients file.
    //! @returns A unique pointer to the constructed object. The type of this
    //! is to the base class, given by the template Base, allowing the caller to
    //! ignore the actual type of the object and make use of the Base class
    //! interface.
    static std::unique_ptr<Base> Construct(const std::string& p_type,
                                           Args... p_args)
    {
        if (Get_All().end() != Get_All().find(p_type))
        {
            return Get_All()[p_type](p_args...);
        }

        throw std::invalid_argument("Unknown type: " + p_type);
    }

    //! @brief This registers classes in the list available to the factory,
    //! allowing the factory to create an instance of anything within the list.
    //! This is needed to that classes can self register meaning that the list
    //! of classes available does not need to be hard coded.
    //! @see https://www.bfilipek.com/2018/02/factory-selfregister.html
    //! @param p_type A string representing the type of the class being
    //! registered. This is used to identify classes.
    //! @param p_create_function A pointer to a function that will return a
    //! constructed instance of the class.
    //! @returns A bool indicating whether or not the class given by p_type was
    //! already registered.
    static bool Register(const std::string& p_type,
                         const Create_Function& p_constructor)
    {
        if (Get_All().end() == Get_All().find(p_type))
        {
            Get_All()[p_type] = p_constructor;
            return true;
        }
        return false;
    }

    //! @brief Retrieves a list of all of the classes that have been compiled
    //! and whether or not they are enabled. TODO: Make the code match this
    //! brief.
    //! @returns An unordered_map, indexed by the name of the class, containing
    //! boolean values indicating as to whether that class is enabled by
    //! default. TODO: Change this
    //! @note This is a function wrapper around a static object using the
    //! "Construct members on first use idiom". This is needed to guarentee that
    //! the unordered_map is initalised whilst the factory contents are
    //! registering themselves. This is a work around for the well known static
    //! initialisation order fiasco.
    //! @see https://isocpp.org/wiki/faq/ctors#static-init-order
    //! @see
    //! https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use-members
    static std::unordered_map<std::string, Create_Function>& Get_All()
    {
        static std::unordered_map<std::string,
                                  Create_Function>
            map;  // TODO: Rename
        return map;
    }

private:
    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class containing
    //! nothing but static functions.
    Abstract_Factory(const Abstract_Factory&) = delete;
};
}  // namespace Internal
}  // namespace ELMO2

#endif
