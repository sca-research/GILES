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
    @file Model_Math.hpp
    @brief Contains the Model_Math class which serves as a base case for
   mathematical Model_Maths to be implemented.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef Model_Math_MATH_HPP
#define Model_Math_MATH_HPP

#include <array>   // for array
#include <limits>  // for max
#include <vector>  // for vector

namespace GILES
{
namespace Internal
{
//! @class Model_Math
//! @brief An abstract class that serves as a base class for mathematical
//! Models to be implemented, These Models will generate the traces
//! for the given program by making use of the Coefficients and the recorded
//! Execution of the program.
class Model_Math
{
private:
    //! This controls the size of the hamming weight lookup table.
    using lookup_size_t = std::uint8_t;  // 8 bit lookup table

    //! The size of the hamming weight lookup table to generate,
    static constexpr std::size_t array_size =
        std::numeric_limits<lookup_size_t>::max() + 1;

    //! class Hamming
    //! @brief This class exists as a wrapper around a lookup table for
    //! Hamming weight values. The array 'weights' will be initialised to
    //! serve as a lookup table when an instance of Hamming is constructed.
    //! @see https://en.wikipedia.org/wiki/Hamming_weight
    struct Hamming
    {
        //! The hamming weight lookup table.
        std::array<std::uint8_t, array_size> Weights;

        //! @brief Constructs Hamming and populates the lookup table with
        //! hamming weight values.
        //! @see https://en.wikipedia.org/wiki/Hamming_weight
        constexpr Hamming() : Weights{}
        {
            for (std::uint32_t i{0}; i < array_size; ++i)
            {
                Weights[i] = Hamming_Weight(i);
            }
        }

        //! @brief Calculates the hamming weight of the given input.
        //! @param p_input The value to find the hamming weight of.
        //! @returns The hamming weight of p_input.
        //! @see https://en.wikipedia.org/wiki/Hamming_weight
        static constexpr std::uint8_t Hamming_Weight(const std::size_t p_input)
        {
// Use non standard accelerated function if it is available.
#ifdef __GNUC
            return __builtin_popcount(p_input);

// Otherwise manually calculate the hamming weights.
#else
            std::size_t input{p_input};
            std::uint8_t count = input ? 1 : 0;
            while (input &= (input - 1))
            {
                ++count;
            }
            return count;
#endif
        }
    };

public:
    //! @brief Retrieves the hamming weight of a given 32 bit value
    //! @param p_input The value to find the hamming weight of.
    //! @returns The hamming weight of p_input.
    //! @see https://en.wikipedia.org/wiki/Hamming_weight
    //! @todo Ensure T is an int type.
    template <typename T,
              typename = std::enable_if_t<std::is_integral<T>::value>>
    static constexpr std::size_t Hamming_Weight(const T p_input)
    {
        // Initialise the lookup.
        constexpr Hamming hamming{};

        // Split the input into a number of parts, each small enough to perform
        // a hamming weight lookup on.
        constexpr std::size_t number_of_parts{sizeof(T) /
                                              sizeof(lookup_size_t)};
        union {
            T original;
            lookup_size_t part[number_of_parts];
        } split_input{p_input};

        // Calculate the sum of the hamming weight of all the parts. This is
        // equal to the hamming weight of the input.
        std::size_t result{0};
        for (std::size_t i{0}; i < number_of_parts; ++i)
        {
            result += hamming.Weights[split_input.part[i]];
        }

        return result;
    }

public:
    //! @brief Calculates the hamming distance between the two given inputs.
    //! @param p_input_1 The value to find the hamming distance from.
    //! @param p_input_2 The value to find the hamming distance to.
    //! @tparam T This is a template in order to support multiple integer
    //! based types in one function.
    //! @returns The hamming distance between the inputs.
    //! @see https://en.wikipedia.org/wiki/Hamming_distance
    template <typename T>
    static constexpr std::size_t Hamming_Distance(T p_input_1, T p_input_2)
    {
        return Hamming_Weight(p_input_1 ^ p_input_2);
    }

    //! @brief This has been deleted to ensure the constructor and the copy
    //! constructor cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_constructor
    Model_Math(const Model_Math&) = delete;

    //! @brief This has been deleted to ensure the copy
    //! assignment operator cannot be called as this is just a utility class
    //! containing nothing but static functions.
    //! @see https://en.cppreference.com/w/cpp/language/rule_of_three
    //! @see https://en.cppreference.com/w/cpp/language/copy_assignment
    Model_Math& operator=(const Model_Math&) = delete;
};  // namespace Internal
}  // namespace Internal
}  // namespace GILES

#endif
