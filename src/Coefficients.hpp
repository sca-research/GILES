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
    @file Coefficients.hpp
    @brief Contains the internal representation of the Coefficients file.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef COEFFICIENTS_HPP
#define COEFFICIENTS_HPP

#include <string>         // for string
#include <typeinfo>       // for typeid
#include <unordered_set>  // for unordered_set
#include <vector>         // for vector

#include <boost/core/demangle.hpp>  // for demangle
#include <nlohmann/json.hpp>        // for json

#include "Error.hpp"  // for Report_Error

#include <iostream>

namespace GILES
{
namespace Internal
{
//! @class Coefficients
//! @brief The internal representation of the Coefficients file.
//! The Coefficients file contains the coefficient values that were created by
//! measuring real hardware traces. Each instruction will have a set of terms
//! with a list of corresponding values that will be used to calculate
//! predicted traces by the Model class.
//! @see https://eprint.iacr.org/2016/517
class Coefficients
{
private:
    const nlohmann::json m_coefficients;

    //! @brief Retrieves a value from the Coefficients. This is a generic
    //! function that can retrieve anything, dependent on its parameters. The
    //! parameters in p_categories are all evaluated in the order they are
    //! provided, each representing a sub category within the last, all within
    //! the coefficients. Once all the sub categories have been evaluated, what
    //! is found within that final category is retrieved as the type given by
    //! T_return.
    //! @tparam T_return The return type.
    //! @tparam T_categories The type of the names of the sub categories. This
    //! is only a template in order to be variadic; this is enforced as a string
    //! or something that can be static_cast into a string.
    //! @param p_categories This is a variadic parameter where each value
    //! represents a sub level within the Coefficients.
    //! @returns The value at the end of all the sub levels as the type given by
    //! T_return.
    template <typename T_return,
              typename T_head_category,
              typename... T_categories>
    T_return static get_value(const nlohmann::json& p_coefficients,
                              const T_head_category& p_head,
                              const T_categories&... p_categories)
    {
        // If there are still more than 1 level left to recurse into.
        if constexpr (1 <= sizeof...(T_categories))
        {
            // Go one level deeper and then recurse.
            return get_value<T_return>(p_coefficients.at(p_head),
                                       p_categories...);
        }
        else
        {
            // This is the last sub level, stop recursing and start returning.
            try
            {
                return p_coefficients.at(p_head);
            }
            // If retrieval fails then throw an error that is slightly easier to
            // understand than a generic json type error.
            catch (const nlohmann::detail::type_error&)
            {
                GILES::Internal::Error::Report_Error(
                    "Cannot retrieve value: {} from Coefficients as the chosen "
                    "type: {}",
                    p_coefficients.dump(4),
                    boost::core::demangle(typeid(T_return).name()));
            }
            // If one of the categories doesn't exist then throw an error that
            // is slightly easier to understand than a generic json type error.
            catch (const nlohmann::detail::exception& exception)
            {
                // TODO: Is this a valid alternative ? It works
                // return T_return{};
                // Make sure this is the type of exception we thought it
                // was. If "cannot use .at() with array" exception or "key not
                // found"
                if (304 == exception.id || 403 == exception.id)
                {
                    GILES::Internal::Error::Report_Error(
                        "Could not find category at the current place in "
                        "the Coefficients with the given name: \"{}\".\n"
                        "Current place in coefficients:\n{}\n ",
                        p_head,
                        p_coefficients.dump(4));
                }
                throw;  // Re-throw if this is not the expected exception.
            }
        }
    }

    //! @brief Retrieves a value from the Coefficients. This is mostly a wrapper
    //! function around the other get_value function that exists to simplify
    //! exception handling.
    //! @tparam T_return The return type.
    //! @tparam T_categories The type of the names of the sub categories. This
    //! is only a template in order to be variadic; this is enforced as a string
    //! or something that can be static_cast into a string.
    //! @param p_categories This is a variadic parameter where each value
    //! represents a sub level within the Coefficients.
    //! @returns The value at the end of all the sub levels as the type given by
    //! T_return.
    //! @exception std::invalid_argument If the resulting value cannot be cast
    //! to the chosen return type, this exception is thrown.
    //! @exception nlohmann::json::out_of_range This is thrown when any of the
    //! sub categories given by p_categories is not found.
    //! @see https://en.cppreference.com/w/cpp/language/fold
    template <typename T_return, typename... T_categories>
    T_return get_value(const T_categories&... p_categories) const
    {
        return get_value<T_return>(m_coefficients, p_categories...);
    }

    //! @brief This is a helper function that does nothing other than lookup
    //! the instruction category of the instruction given by p_opcode, and
    //! pass the results onto the get_value function.
    //! @param p_opcode The opcode of the instruction to lookup the category
    //! for.
    //! @param p_categories This is a variadic parameter where each value
    //! represents a sub level within the Coefficients.
    //! @tparam T_return The return type.
    //! @tparam T_categories The type of the names of the sub categories.
    //! This is only a template in order to be variadic; this is enforced as
    //! a string or something that can be static_cast into a string.
    //! @returns The value at the end of all the sub levels as the type
    //! given by T_return.
    template <typename T_return, typename... T_categories>
    T_return get_value_opcode(const std::string& p_opcode,
                              const T_categories&... p_categories) const
    {
        return get_value<T_return>(Get_Instruction_Category(p_opcode),
                                   p_categories...);
    }

    //! @todo Change to be style of get_value_opcode comment
    //! @brief Retrieves an individual coefficient value by name, under the
    //! instruction category that contains the instruction given by
    //! p_opcode. p_categories is a variadic parameter that represents a
    //! list of sub categories within the instruction category to retrieve
    //! from.
    //! @tparam T_return The return type.
    //! @tparam T_categories The type of the names of the sub categories.
    //! This is only a template in order to be variadic; this is enforced as
    //! a string or something that can be static_cast into a string.
    //! @param p_opcode  The opcode of the instruction that the Coefficients
    //! are required for.
    //! @param p_categories This is a variadic parameter where each value
    //! represents a sub level within the Coefficients.
    //! @returns The value at the end of all the sub levels as the type
    //! given by T_return.
    template <typename T_return, typename... T_categories>
    T_return get_coefficient(const std::string& p_opcode,
                             const T_categories&... p_categories) const
    {
        return get_value_opcode<T_return>(
            p_opcode, "Coefficients", p_categories...);
    }

public:
    //! @brief Constructors an instance from the json as loaded from the
    //! Coefficients file.
    //! @param p_coefficients The validated coefficients, contained within a
    //! json object, as they are stored in the file.
    //! @warning Validation of the json p_coefficients should have already
    //! occurred, using the Validator_Coefficients class, before calling the
    //! constructor.
    explicit Coefficients(const nlohmann::json& p_coefficients)
        : m_coefficients(p_coefficients)
    {
    }

    const std::string&
    Get_Instruction_Category(const std::string& p_opcode) const;

    const std::unordered_set<std::string> Get_Interaction_Terms() const;

    const std::vector<double>
    Get_Coefficients(const std::string& p_opcode,
                     const std::string& p_interaction_term) const;

    //! @brief Retrieves an individual coefficient value by name, under the
    //! instruction category that contains the instruction given by
    //! p_opcode. p_categories is a variadic parameter that represents a
    //! list of sub categories within the instruction category to retrieve
    //! from.
    //! @tparam T_categories The type of the names of the sub categories.
    //! This is only a template in order to be variadic; this is enforced as
    //! a string or something that can be static_cast into a string.
    //! @param p_opcode  The opcode of the instruction that the Coefficients
    //! are required for.
    //! @param p_categories This is a variadic parameter where each value
    //! represents a sub level within the Coefficients.
    //! @returns The value at the end of all the sub levels as type double.
    template <typename... T_categories>
    double Get_Coefficient(const std::string& p_opcode,
                           const T_categories&... p_categories) const
    {
        return get_coefficient<double>(p_opcode, p_categories...);
    }

    double Get_Constant(const std::string& p_opcode) const;
};
}  // namespace Internal
}  // namespace GILES

#endif
