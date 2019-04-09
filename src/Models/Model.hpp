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
    @file Model.hpp
    @brief Contains the Model class which serves as a base case for mathematical
    models to be implemented.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>      // for all_of
#include <string>         // for string
#include <unordered_set>  // for unordered_set
#include <vector>         // for vector

#include "Coefficients.hpp"
#include "Execution.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class Model
//! @brief An abstract class that serves as a base class for mathematical models
//! to be implemented, These models will generate the traces for the given
//! program by making use of the Coefficients and the recorded Execution of the
//! program.
class Model
{
protected:
    //! The execution of the target program as recorded by the Emulator.
    const ELMO2::Internal::Execution m_execution;

    //! The Coefficients created by measuring real hardware traces.
    const ELMO2::Internal::Coefficients& m_coefficients;

    //! @brief This function will add noise to the traces to simulate the
    //! background noise that would be picked up in real traces.
    //! @param p_traces The Traces to have the noise added to.
    //! @returns The Traces with the added noise.
    // const ELMO2::Internal::Traces& addNoise(ELMO2::Internal::Traces*
    // p_traces);

    //! @brief The constructor needs to be provided with the recorded
    //! Execution of the target program and the details from real world
    //! traces, the Coefficients, to be able to calculate the Traces for the
    //! target program. This constructor is marked as protected as it should
    //! only be called by derived classes to assist with initialisation.
    //! @param p_execution The recorded Execution of the target program,
    //! provided by the Emulator.
    //! @param p_coefficients The loaded Coefficients from real hardware
    //! traces.
    Model(const ELMO2::Internal::Execution& p_execution,
          const ELMO2::Internal::Coefficients& p_coefficients)
        : m_execution(p_execution), m_coefficients(p_coefficients)
    {
    }

    //! @brief Retrieves the interaction terms used within the model. This is
    //! required to ensure the Coefficients file provided all of the required
    //! interaction terms.
    //! @returns A list of required interaction terms as defined in the
    //! derived class.
    virtual const std::unordered_set<std::string>&
    Get_Interaction_Terms() const = 0;
    /*
     *static const std::unordered_set<std::string>& Get_Interaction_Terms()
     *{
     *    return Derived::get_interaction_terms();
     *};
     */

public:
    //! @brief Virtual destructor to ensure proper memory cleanup.
    //! @see https://stackoverflow.com/a/461224
    virtual ~Model() = default;  // TODO: Should this = default? Rule of
    // ZERO? http://en.cppreference.com/w/cpp/language/rule_of_three

    //! @brief In derived classes, this function should contain the mathematical
    //! calculations that generate the Traces.
    //! @returns The generated Traces for the target program.
    virtual const std::vector<float> Generate_Traces() const = 0;

    //! @todo Move this to a Model_Maths class
    //! @brief Calculates the hamming weight of the given input.
    //! @param p_input The value to find the hamming weight of.
    //! @tparam T This is a template in order to support multiple integer based
    //! types in one function.
    //! @returns The hamming weight of p_input.
    //! @see https://en.wikipedia.org/wiki/Hamming_weight
    /*
     *    template <typename T> constexpr std::size_t hamming_weight(T p_input)
     *const
     *    {
     * // Use non standard accelerated function if it is available.
     *#ifdef __GNUC_
     *        return __builtin_popcount(p_input);
     *#else
     *        std::size_t count = p_input ? 1 : 0;
     *        while (p_input &= (p_input - 1))
     *        {
     *            ++count;
     *        }
     *        return count;
     *#endif
     *    }
     */

    template <std::uint32_t N> struct Hamming
    {
        constexpr Hamming() : Weights()
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                Weights[i] = hamming_weight(i);
            }
        }
        std::size_t Weights[N];

    private:
        template <typename T>
        constexpr std::size_t hamming_weight(T p_input) const
        {
// Use non standard accelerated function if it is available.
#ifdef __GNUC_
            __builtin_popcount(p_input);
#else
            std::size_t count = p_input ? 1 : 0;
            while (p_input &= (p_input - 1))
            {
                ++count;
            }
            return count;
#endif
        }
    };

    // TODO: Work out why this optimisation had no performance impact...
    static constexpr std::size_t hamming_weight(const std::uint32_t p_input)
    {
        constexpr auto hammings = Hamming<255>();

        union {
            std::uint32_t original;
            std::uint8_t bytes[4];
        } split_input({p_input});

        // Split the 32 bit number into an array of 4 8 bit numbers
        // const auto split_input = (std::uint8_t*)&p_input;

        return hammings.Weights[split_input.bytes[0]] +
               hammings.Weights[split_input.bytes[1]] +
               hammings.Weights[split_input.bytes[2]] +
               hammings.Weights[split_input.bytes[3]];
    }

    //! @todo Move this to a Model_Maths class
    //! @brief Calculates the hamming distance between the two given inputs.
    //! @param p_input_1 The value to find the hamming distance from.
    //! @param p_input_2 The value to find the hamming distance to.
    //! @tparam T This is a template in order to support multiple integer
    //! based types in one function.
    //! @returns The hamming distance between the inputs.
    //! @see https://en.wikipedia.org/wiki/Hamming_distance
    template <typename T>
    constexpr std::size_t hamming_distance(T p_input_1, T p_input_2) const
    {
        return hamming_weight(p_input_1 ^ p_input_2);
    }

    //! @brief Ensures that all the interaction terms used within the model
    //! are provided by the Coefficients.
    //! @returns True if the all the interaction terms required by the model
    //! are contained within the Coefficients and false if not.
    bool Check_Interaction_Terms() const
    {
        const auto& models_terms       = Get_Interaction_Terms();
        const auto& coefficients_terms = m_coefficients.Get_Interaction_Terms();

        // If there are less terms in the Coefficients then there are
        // missing terms.
        return models_terms.size() <= coefficients_terms.size() &&
               // For each term the model uses, check it is contained within
               // the set of terms provided by the Coefficients.
               std::all_of(models_terms.begin(),
                           models_terms.end(),
                           // A lambda function for checking if an
                           // individual element is in coefficients_terms.
                           [&coefficients_terms](const auto& term) {
                               return coefficients_terms.end() !=
                                      coefficients_terms.find(term);
                           });
    }
};
}  // namespace Internal
}  // namespace ELMO2

#endif
