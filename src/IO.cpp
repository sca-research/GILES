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
    @file IO.cpp
    @brief This file contains all input and output functions for GILES.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include "IO.hpp"

#include <cstdlib>    // for exit, EXIT_FAILURE
#include <fstream>    // for ifstream
#include <stdexcept>  // for invalid_argument

#include <nlohmann/json.hpp>  // for json, basic_json<>::exception

#include "Coefficients.hpp"            // for Coefficients
#include "Error.hpp"                   // for Report_Error
#include "Validator_Coefficients.hpp"  // for Validator_Coefficients

namespace GILES
{
namespace Internal
{
//! @brief Loads the Coefficients from a file as specified by
//! p_coefficients_path.
//! @param p_coefficients_path The path where the Coefficients should be
//! loaded from.
//! @returns The Coefficients using the internal representation.
const GILES::Internal::Coefficients GILES::Internal::IO::Load_Coefficients(
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
    catch (nlohmann::detail::parse_error&)
    {
        GILES::Internal::Error::Report_Error(
            "Coefficients file '{}' is not a valid JSON file",
            p_coefficients_path);
    }

    // This will throw an exception if validation fails.
    GILES::Internal::Validator_Coefficients::Validate_Json(json);

    return GILES::Internal::Coefficients(json);
}
}  // namespace Internal
}  // namespace GILES
