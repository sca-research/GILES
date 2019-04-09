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
    @file Utility.hpp
    @brief This file contains the Utility class. A simple class with a few
    static helper functions that is otherwise empty.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>  // for string
#include <vector>  // for vector

#include "Assembly_Instruction.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Utility This is a simple class containing nothing but static function
//! that has a few functions to help with simple operations such as string
//! transformations that are not in the standard library.
class Utility
{
public:
    //! @brief Splits a string into a vector of strings using a string given by
    //! p_delimiter as a delimiter for splitting. // For example: Splitting the
    //! string "Hello!World" with the delimiter "!" would become "Hello",
    //! "World".
    //! @param p_input The string to be split.
    //! @param p_delimiter The delimiting string to spilt on. This will not be
    //! present in the output.
    //! @ returns a vector containing each split part of the input string.
    static const std::vector<std::string>
    string_split(std::string p_input, const std::string& p_delimiter)
    {
        // A temporary structure for storing the result.
        std::vector<std::string> split_string;

        // Loop indefinitely until the end of the string if reached.
        // This is basically a while loop with a variable declared within it.
        std::size_t position;
        while (std::string::npos != (position = p_input.find(p_delimiter)))
        {
            // Push back everything from the beginning until this point as one
            // split.
            split_string.push_back(p_input.substr(0, position));

            // Remove what was just pushed back from the input string.
            p_input.erase(0, position + p_delimiter.length());
        }

        split_string.push_back(p_input);

        /*
         *if (0 == split_string.size())
         *{
         *    throw std::invalid_argument(
         *        "Could not split string, the delimiting value was not
         *found.");
         *}
         */
        return split_string;
    }

    //! @brief Splits a string into into two using a string given by
    //! p_delimiter as a delimiter for splitting. The left hand side of the
    //! split is removed from the input and returned.
    //! If the delimiter is not found, the entire string is popped and returned,
    //! clearing p_input.
    //! @warning This function can potentially remove characters from the
    //! original input as p_input is a pointer.
    //! @param p_input The string to be split. This is a passed by pointer and
    //! will also contain the right hand side of the split after the function
    //! returns. This has been declared as a const pointer instead of a
    //! reference to indicate that it contains the original data.
    //! @param p_delimiter The delimiting string to spilt on. This will not be
    //! present in the output.
    //! @returns The left hand side of the first occurrence of p_delimiter in
    //! p_input or the entire string if p_delimiter is not found.
    static const std::string
    string_split_head_pop(std::string* const p_input,
                          const std::string& p_delimiter)
    {
        std::string head;
        auto position = p_input->find(p_delimiter);

        // Push back everything from the beginning until this point as one
        // split.
        head = p_input->substr(0, position);

        // Remove what was just pushed back from the input string.
        p_input->erase(0, position + p_delimiter.length());

        if (std::string::npos == position)
        {
            // If the delimiting value was not found, then pop the whole string
            // as the head
            p_input->clear();
        }
        return head;
    }

private:
    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class containing
    //! nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_constructor
    Utility(const Utility&) = delete;

    //! @brief This has been deleted to ensure the copy
    //! assignment operator cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_assignment
    Utility& operator=(const Utility&) = delete;
};
}  // namespace Internal
}  // namespace ELMO2

#endif  // UTILITY_HPP
