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
    @file Coefficients.hpp
    @brief Contains the internal representation of the Coefficients file.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef COEFFICIENTS_HPP
#define COEFFICIENTS_HPP

#include <string>        // for string
#include <unordered_set> // for unordered_set
#include <vector>        // for vector

#include <json.hpp> // for json

namespace ELMO2
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

    const std::string
    get_instruction_category(const std::string& p_opcode) const;

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

    const std::unordered_set<std::string> Get_Interaction_Terms() const;

    const std::vector<double>
    Get_Coefficients(const std::string& p_opcode,
                     const std::string& p_interaction_term) const;

    double Get_Constant(const std::string& p_opcode) const;
};
} // namespace Internal
} // namespace ELMO2

#endif
