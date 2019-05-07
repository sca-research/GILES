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
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ABSTRACT_FACTORY_HPP
#define ABSTRACT_FACTORY_HPP

#include <memory>         // for unique_ptr
#include <stdexcept>      // for invalid_argument
#include <string>         // for string
#include <unordered_map>  // for unordered_map

#include "Error.hpp"  // for Report_Error

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
// These are need to instantiate the templates at the bottom of the file.
class Coefficients;
class Emulator_Interface;
class Execution;
class Model;

//! @class Abstract_Factory
//! @brief This is a static factory class that assists in the construction of
//! objects. By providing abstraction for the caller, it hides the exact type of
//! the object from the caller and allows the caller to make use of the object
//! through the Base class interface regardless of the derived type.
//! @note The exact classes that this can construct are not stored here and are
//! added at runtime using self-registering techniques. This simplifies the
//! process of adding and removing classes and allows objects to be selectively
//! compiled as a way to completely remove those as options. This is also a
//! template class allowing it to construct any object and not just those of a
//! fixed type.
//! @see https://en.wikipedia.org/wiki/Factory_method_pattern
//! @see https://www.bfilipek.com/2018/02/factory-selfregister.html
template <typename base_t, typename... args_t> class Abstract_Factory
{
public:
    //! A function pointer used to point to the function that calls the
    //! constructor for objects in the factory.
    using Create_Function = std::unique_ptr<base_t> (*)(args_t...);

    //! @brief Retrieves an object's constructor from the factory by name.
    //! If an object with the given name is not found then an error message will
    //! be reported and execution will halt.
    //! @param p_type The name of the object to be retrieved.
    //! @returns A function pointer, pointing at the constructor of the class
    //! corresponding to the name given by p_type.
    static Create_Function Find(const std::string& p_type)
    {
        // If p_type is registered.
        if (auto& all = Get_All(); all.end() != all.find(p_type))
        {
            // Construct and return an instance of p_type.
            return all[p_type];
        }
        Error::Report_Error("Could not find '{}'.\n", p_type);
    }

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
    static std::unique_ptr<base_t> Construct(const std::string& p_type,
                                             args_t... p_args)
    {
        return Find(p_type)(p_args...);
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
        // Don't register if this is already registered.
        if (auto& all = Get_All(); all.end() == all.find(p_type))
        {
            all[p_type] = p_constructor;
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
    //! "Construct members on first use idiom". This is needed to guarantee that
    //! the unordered_map is initialised whilst the factory contents are
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

    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class containing
    //! nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_constructor
    Abstract_Factory(const Abstract_Factory&) = delete;

    //! @brief This has been deleted to ensure the copy
    //! assignment operator cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_assignment
    Abstract_Factory& operator=(const Abstract_Factory&) = delete;
};

//! @brief This exists only to simply the usage of the Abstract_Factory
//! class. By providing an intermediate, the possibility of accidentally
//! initialising a separate template is eliminated. Additionally, this
//! provides for a more meaningful name. To see what is actually going on behind
//! the scenes, refer to the Abstract_Factory class.
using Model_Factory =
    Abstract_Factory<Model, const Execution&, const Coefficients&>;

//! @brief This exists only to simply the usage of the Abstract_Factory
//! class. By providing an intermediate, the possibility of accidentally
//! initialising a separate template is eliminated. Additionally, this
//! provides for a more meaningful name. To see what is actually going on behind
//! the scenes, refer to the Abstract_Factory class.
using Emulator_Factory =
    Abstract_Factory<Emulator_Interface, const std::string&>;

}  // namespace Internal
}  // namespace ELMO2

#endif
