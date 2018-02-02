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
    @file IO.hpp
    @brief This file contains all input and output functions for ELMO2.
    @author Scott Egerton
    @date 2017
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "Coefficients.hpp"
#include "Traces.hpp"

namespace ELMO2
{
namespace Internal
{
//! @class IO
//! @brief This contains all Input and Output functions for ELMO2. This includes
//! loading the Coefficients file and saving generated Traces.
class IO
{
public:
    //! All possible output formats for the generated traces to be saved as.
    enum class Output_Format : uint8_t
    {
        Riscure
    };

    //! @brief Loads the Coefficients from a file as specified by
    //! p_coefficients_path.
    //! @param p_coefficients_path The path where the Coefficients should be
    //! loaded from.
    //! @returns The Coefficients using the internal representation.
    const ELMO2::Internal::Coefficients&
    Load_Coefficients(const std::string& p_coefficients_path);

    //! @brief Saves the generated Traces to the file as specified by
    //! p_traces_path. The p_format parameter specifies which format the Traces
    //! should be saved in.
    //! @param p_traces_path The destination to save the Traces to.
    //! @param p_traces The Traces to be saved.
    //! @param p_format The format in which the Traces are to be saved.
    bool Output_Traces(const std::string& p_traces_path,
                       const ELMO2::Internal::Traces& p_traces,
                       const Output_Format p_format) const;
};
} // namespace Internal
} // namespace ELMO2

#endif
