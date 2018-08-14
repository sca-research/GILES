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
    @file IO.cpp
    @brief This file contains all input and output functions for ELMO2.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include "IO.hpp"

#include <fstream>                     // for ifstream
#include <iostream>                    // for operator<<, endl
#include <stdexcept>                   // for invalid_argument

#include <json.hpp>           // for json, basic_json<>::exception

#include "Coefficients.hpp"            // for Coefficients
#include "Validator_Coefficients.hpp"  // for Validator_Coefficients

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
class Traces;

//! @brief Loads the Coefficients from a file as specified by
//! p_coefficients_path.
//! @param p_coefficients_path The path where the Coefficients should be
//! loaded from.
//! @returns The Coefficients using the internal representation.
const ELMO2::Internal::Coefficients ELMO2::Internal::IO::Load_Coefficients(
    const std::string& p_coefficients_path) const
{
    // read the Coefficients file into a JSON object
    std::ifstream file(p_coefficients_path);
    nlohmann::json json;

    // Ensure the file contains valid JSON
    try
    {
        json = nlohmann::json::parse(file);
    }
    catch (nlohmann::json::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::invalid_argument("Coefficients file '" +
                                    p_coefficients_path +
                                    "' is not a valid JSON file");
    }

    // This will throw an exception if validation fails.
    ELMO2::Internal::Validator_Coefficients::Validate_Json(json);

    return ELMO2::Internal::Coefficients(json);
}

//! @brief Saves the generated Traces to the file as specified by
//! p_traces_path. The p_format parameter specifies which format the Traces
//! should be saved in.
//! @param p_traces_path The destination to save the Traces to.
//! @param p_traces The Traces to be saved.
//! @param p_format The format in which the Traces are to be saved.
bool ELMO2::Internal::IO::Output_Traces(const std::string& p_traces_path,
                                        const ELMO2::Internal::Traces& p_traces,
                                        const Output_Format& p_format) const
{
    return false; // TODO: Implement this
}
} // namespace Internal
} // namespace ELMO2
