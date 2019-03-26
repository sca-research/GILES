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
    @file Model_Power.hpp
    @brief Contains a mathematical model for calculating the power usage of the
    target program.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef MODEL_POWER_HPP
#define MODEL_POWER_HPP

#include <bitset>       // for bitset
#include <cstdint>      // for size_t
#include <string>       // for string
#include <string_view>  // for string_view
#include <type_traits>  // for is_same
#include <utility>      // for pair
#include <vector>       // for vector

#include "Assembly_Instruction.hpp"
#include "Coefficients.hpp"
#include "Execution.hpp"

#include "Model.hpp"                   // for Model
#include "Model_Factory_Register.hpp"  // for Model_Factory_Register

namespace ELMO2
{
namespace Internal
{
//! @class Model_Power
//! @class Model_Hamming_Weight
//! @brief This derived class contains a specific implementation of a
//! mathematical model to calculate the traces for a target program. It is
//! designed as a template allowing new models to be added with ease.
//! Deriving from Model_Factory_Register as well will automatically register
//! this class within the factory class.
class Model_Power : public virtual ELMO2::Internal::Model,
                    public ELMO2::Internal::Model_Factory_Register<
                        ELMO2::Internal::Model_Power>
{
private:
    class Instruction_Terms_Helper
    {
    protected:
        static std::size_t calculate_interactions(const std::bitset<32>& p_term)
        {
            std::size_t result{0};

            // TODO: This is just dot product, change to use std function inner
            // product.
            // Loop through all possible combinations of i and j where i
            // and j are never the same.
            for (std::size_t term_1{0}; term_1 < 32; ++term_1)
            {
                for (std::size_t term_2{term_1 + 1}; term_2 < 32; ++term_2)
                {
                    result += p_term[term_1] *
                              p_term[term_2];  // TODO: What is this doing?
                                               // - Logical AND but why?
                }
            }
            return result;
        }

        //! @brief Ensures this is a pure virtual class, only able to be
        //! inherited from.
        // virtual ~Instruction_Terms_Helper() = 0;
    };

    //! @todo document
    // Used to store intermediate terms needed in leakage calculations, that are
    // related to a specific instruction. This exists for the simple reason of
    // saving the time recalculating the data.
    // TODO: This should inherit from Assembly_Instruction
    struct Assembly_Instruction_Power : ELMO2::Internal::Assembly_Instruction,
                                        Instruction_Terms_Helper
    {
        Assembly_Instruction_Power(
            const ELMO2::Internal::Assembly_Instruction& p_instruction,
            const std::size_t p_operand_1,
            const std::size_t p_operand_2)  // TODO: Encode Operand value into
                                            // Assembly_Instruction
            : ELMO2::Internal::Assembly_Instruction(p_instruction),
              Operand_1(p_operand_1), Operand_2(p_operand_2),
              Operand_1_Bit_Interactions(calculate_interactions(p_operand_1)),
              Operand_2_Bit_Interactions(calculate_interactions(p_operand_2))
        {
        }

        // Does this need to be stored? -
        // Saves recalculating it
        //
        // TODO: MOVE OPERANDS UP TO
        // ASSEMBLY_INSTRUCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        const std::uint32_t Operand_1;
        const std::uint32_t Operand_2;
        const std::size_t Operand_1_Bit_Interactions;
        const std::size_t Operand_2_Bit_Interactions;
    };

    //! @todo: document
    // This stores intermediate terms that are related to the interactions
    // between two different instructions. This exists for the simple reason of
    // saving the time recalculating the data.
    struct Instruction_Terms_Interactions : public Instruction_Terms_Helper
    {
        Instruction_Terms_Interactions(
            const ELMO2::Internal::Model_Power::Assembly_Instruction_Power&
                p_instruction_1,
            const ELMO2::Internal::Model_Power::Assembly_Instruction_Power&
                p_instruction_2)
            : Operand_1_Bit_Flip(calculate_bitflips(p_instruction_1.Operand_1,
                                                    p_instruction_2.Operand_2)),
              Operand_2_Bit_Flip(calculate_bitflips(p_instruction_1.Operand_1,
                                                    p_instruction_2.Operand_2)),
              Bit_Flip1_Bit_Interactions(
                  calculate_interactions(Operand_1_Bit_Flip)),
              Bit_Flip2_Bit_Interactions(
                  calculate_interactions(Operand_2_Bit_Flip))
        {
        }
        // TODO: I think only the hw and hd need to be stored?
        // BitFlip is 32 bools indicating whether a bitflip has occurred between
        // current and previous instruction. BitFlip
        // TODO: This makes more sense to be stored as a bit set, maybe make a
        // wrapper get function that does the to_ulong() conversion?
        const std::bitset<32> Operand_1_Bit_Flip;
        const std::bitset<32> Operand_2_Bit_Flip;
        const std::size_t Bit_Flip1_Bit_Interactions;
        const std::size_t Bit_Flip2_Bit_Interactions;

    private:
        static const std::bitset<32>
        calculate_bitflips(const std::bitset<32>& p_instruction_1_operand,
                           const std::bitset<32>& p_instruction_2_operand)
        {
            std::bitset<32> result;
            for (std::size_t i{0}; i < result.size(); ++i)
            {
                result[i] =
                    p_instruction_1_operand[i] != p_instruction_2_operand[i];
            }
            return result;
        }
    };

    static const std::unordered_set<std::string> m_required_interaction_terms;

    //! @brief Retrieves a list of the interaction terms that are used within
    //! the model. These must be provided by the Coefficients in order for
    //! the model to function.
    //! @returns The list of interaction terms used within the model.
    const std::unordered_set<std::string>&
    Get_Interaction_Terms() const override
    {
        return m_required_interaction_terms;
    }

    //! @brief A wrapper around the Get_Coefficient function that will return 0
    //! if an instruction is not found.
    template <typename... T_categories>
    std::vector<double>
    Get_Coefficients(const std::string& p_opcode,
                     const T_categories&... p_categories) const
    {
        try
        {
            return m_coefficients.Get_Coefficients(p_opcode, p_categories...);
        }
        catch (const std::out_of_range& exception_not_found)
        {
            // 32 0s is large enough to be accessed without segfaulting.
            return std::vector<double>(32, 0);
        }
    }

    //! @brief A wrapper around the Get_Coefficients function that will return 0
    //! if an instruction is not found and retrieve the instruction category of
    //! p_target_category before calling Get_Coefficient.
    //! @param p_opcode  The opcode of the instruction that the Coefficients are
    //! required for.
    //! @param p_instruction_term The term that the coefficients should be
    //! retrieved for.
    //! @param p_target_category The instruction category of the target
    //! instruction representing another sub level within the Coefficients.
    //! @returns The value at the end of all the sub levels as type double or 0
    //! if the instruction was not found.
    double Get_Coefficient(const std::string& p_opcode,
                           const std::string& p_instruction_term,
                           const std::string& p_target_category) const
    {
        try
        {
            return m_coefficients.Get_Coefficient(
                p_opcode,
                p_instruction_term,
                Get_Instruction_Category(p_target_category));
        }
        catch (const std::out_of_range& exception_not_found)
        {
            return 0;
        }
    }

    //! @brief A wrapper around the Get_Constant function that will return 0
    //! if an instruction is not found.
    //! @param p_opcode The opcode of the instruction that the Constant is
    //! required for.
    //! @returns The value of the constant for the category that the
    //! instruction belongs to or 0 if the instruction was not found.
    double Get_Constant(const std::string& p_opcode) const
    {
        try
        {
            return m_coefficients.Get_Constant(p_opcode);
        }
        catch (const std::out_of_range& exception_not_found)
        {
            return 0;
        }
    }

    //! @brief A wrapper around the Get_Instruction_Category function that
    //! will return Shifts if an instruction is not found.
    //! @param p_opcode The opcode of the instruction for which the category
    //! will be retrieved.
    //! @returns The name of the category that the instruction is contained
    //! within. If the coefficients are not categorised then the instruction
    //! opcode is returned or "Shifts" if the instruction is not found.
    const std::string
    Get_Instruction_Category(const std::string& p_opcode) const
    {
        try
        {
            return m_coefficients.Get_Instruction_Category(p_opcode);
        }
        // Instruction was not profiled so return any value that prevents a
        // crash. This be be zeroed out later on in calculations.
        catch (const std::out_of_range& exception_not_found)
        {
            return "Shifts";
            // Linear regression means that nothing is done for ALU and this is
            // an invalid value so Shifts is used as the default value
        }
    }

    //! @brief Retrieves the Assembly_Instruction that is in the execute
    //! pipeline stage in the current cycle, given by p_cycle.
    //! If the execution stage of the pipeline for the current cycle is not in a
    //! normal state then a fake instruction with all terms set to be 0 is
    //! returned. This is to prevent crashing whilst still not adding erroneous
    //! data to any calculations using its terms.
    //! @param p_cycle The clock cycle number from which to retrieve the
    //! pipeline state.
    //! @returns The requested instruction as an instance of
    //! Assembly_Instruction_Power.
    //! @see https://en.wikipedia.org/wiki/Instruction_pipelining
    //! @see https://en.wikipedia.org/wiki/Clock_cycle
    const ELMO2::Internal::Model_Power::Assembly_Instruction_Power
    get_instruction_terms(const std::size_t& p_cycle) const
    {
        // Prevents trying to calculate the hamming weight of stalls and
        // flushes.
        if (!m_execution.Is_Normal_State(p_cycle, "Execute"))
        {
            // Return a fake instruction to prevent crashing
            // Currently stalls and flushes are stored as zeros in
            // calculations.
            return ELMO2::Internal::Model_Power::Assembly_Instruction_Power(
                Assembly_Instruction("Abnormal State", {"0", "0"}), 0, 0);
        }

        // Retrieves what is in the "Execute" pipeline stage at clock cycle
        // "i".
        const auto& instruction =
            m_execution.Get_Instruction(p_cycle, "Execute");

        // Add the next set of operands.
        return ELMO2::Internal::Model_Power::Assembly_Instruction_Power(
            m_execution.Get_Instruction(p_cycle, "Execute"),
            m_execution.Get_Operand_Value(p_cycle, instruction, 1),
            m_execution.Get_Operand_Value(p_cycle, instruction, 2));
    }

    //! @todo document
    //! @brief Multiplies all of the values in p_vector by p_scalar and returns
    //! the total of all of these multiplications added up.
    //! @tparam T The type of the values to be multiplied. Any type that
    //! implements the * and + operators should work.
    //! @param p_scalar The value to multiply each of the vector values by.
    //! @param p_vector The vector containing the values to multiply with the
    //! scalar value.
    //! @returns The sum of all of the multiplications.
    //! @see https://en.wikipedia.org/wiki/Scalar_multiplication
    /*
     *template <typename T>
     *const T sum_of_scalar_multiply(const T p_scalar,
     *                               const std::vector<T>& p_vector) const
     *{
     *    auto total = 0;
     *    for (const auto value : p_vector)
     *    {
     *        total += p_scalar * value;
     *    }
     *    return total;
     *}
     */
    /*
     *    template <std::size_t N>
     *    const double calculate_term(const std::string& p_opcode,
     *                                const std::string& p_term_name,
     *                                const std::bitset<N> p_instruction_term)
     * const
     *    {
     *        // This is based off of what original elmo does to calculate an
     *        // individual term
     *        const auto coefficients = Get_Coefficients(p_opcode, p_term_name);
     *
     *        return std::inner_product(std::begin(p_instruction_term),
     *                                  std::end(p_instruction_term),
     *                                  std::begin(coefficients),
     *                                  std::end(coefficients));
     *        return sum_of_scalar_multiply(p_instruction_term,
     *                                      Get_Coefficients(p_opcode,
     *        p_term_name));
     *    }
     */

    template <typename T>
    const double dot_product(const std::vector<T>& p_vector_1,
                             const std::vector<T>& p_vector_2)
    {
        T total{0};
        for (std::size_t i{0}; i < p_vector_1.size(); ++i)
        {
            total += p_vector_1[i] * p_vector_2[i];
        }
        return total;
    }

    template <std::size_t N>
    const double calculate_term(const std::string& p_opcode,
                                const std::string& p_term_name,
                                const std::bitset<N>& p_instruction_term) const
    // TODO: Replace bitset with vector of bools everywhere?
    {
        // This is based off of what original elmo does to calculate an
        // individual term
        const auto coefficients = Get_Coefficients(p_opcode, p_term_name);
        double total{0};
        for (std::size_t i{0}; i < N; ++i)
        {
            // bit = (p_instruction_term >> i) && bool(1);
            total += p_instruction_term[i] * coefficients[i];
        }
        /*
         *return sum_of_scalar_multiply(p_instruction_term,
         *                              Get_Coefficients(p_opcode,
         *p_term_name));
         */
        return total;
    }

    //! @brief Used only when calling calculate_hamming_x functions.
    //! Increases readability for a simple boolean value.
    enum class Instruction
    {
        Previous,
        Subsequent
    };

    const double calculate_hamming_weight(
        const ELMO2::Internal::Model_Power::Assembly_Instruction_Power&
            p_current_instruction,
        const std::size_t p_operand_index,
        const Instruction p_previous_or_next_instruction,
        const std::string& p_opcode_target)
        const  // TODO: Why is this type
               // double?? What should it be?
    {
        std::string instruction{Instruction::Previous ==
                                        p_previous_or_next_instruction
                                    ? "Previous"
                                    : "Subsequent"};
        // This is based off of what original elmo does to calculate an
        // individual term
        return Get_Coefficient(p_current_instruction.Get_Opcode(),
                               "Hamming_Weight_Operand" +
                                   std::to_string(p_operand_index) + "_" +
                                   instruction + "_Instruction",
                               p_opcode_target) *
               // hamming_weight(current_instruction.Get_Operand(p_operand_index));
               hamming_weight(p_current_instruction.Operand_1);
    }

public:
    //! @brief The constructor makes use of the base Model constructor to
    //! assist with initialisation of private member variables.
    Model_Power(const ELMO2::Internal::Execution& p_execution,
                const ELMO2::Internal::Coefficients& p_coefficients)
        : ELMO2::Internal::Model(p_execution, p_coefficients)
    {
        // This statement registers this class in the factory, allowing
        // access from elsewhere. Do not delete this or else this class will
        // not appear in the factory. If you wish to make this class
        // inaccessible, a better method would be to remove the
        // corresponding cpp file from the build script. This is required to
        // be "used" somewhere in order to prevent the compiler from
        // optimising it away, thus preventing self registration.
        // Section 6.6.4.1, point 2 of the linked document states that this
        // statement will not be optimised away.
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
        // The void cast does nothing functionally but prevents the compiler
        // warning about an unused result.
        (void)m_is_registered;

        // TODO: This can be moved up into Model if CTRP is used. CTRP can
        // work if a non template interface class is introduced.
        if (!this->Check_Interaction_Terms())
        {
            throw std::logic_error(
                "Model was not provided with correct "
                "interaction terms by the Coefficients file.");
        }
    }

    //! @brief This function contains the mathematical calculations that
    //! generate the power Traces.
    //! TODO: Improve this description with details of how elmo power model
    //! works.
    //! @returns The generated Traces for the target program
    const std::vector<float> Generate_Traces() const override;

    //! @brief Retrieves the name of this Model.
    //! @returns The name as a string.
    //! @note This is needed to ensure self registration in the factory
    //! works. The factory registration requires this as unique identifier.
    static const std::string Get_Name() { return "Power"; }
};  // namespace Internal
}  // namespace Internal
}  // namespace ELMO2

#endif
