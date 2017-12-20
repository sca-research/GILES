/*
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file IO.hpp
    @brief This file contains all input and output functions for ELMO2.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "Coefficients.hpp"
#include "Traces.hpp"

namespace ELMO2
{
//! @class IO
//! @brief This contains all Input and Output functions for ELMO2. This includes
//! loading the Coefficients file and saving generated Traces.
class IO
{
public:
    //! @brief Loads the Coefficients from a file as specified by
    //! p_coefficients_path.
    //! @param p_coefficients_path The path where the Coefficients should be
    //! loaded from.
    //! @returns The Coefficients using the internal representation.
    const ELMO2::Coefficients&
    Load_Coefficients(const std::string& p_coefficients_path);

    //! @brief Saves the generated Traces to the file as specified by
    //! p_traces_path. The p_format parameter specifies which format the Traces
    //! should be saved in. TODO: Should the format be an enum instead of a
    //! string to prevent typos? - Yes
    //! @param p_traces_path The destination to save the Traces to.
    //! @param p_traces The Traces to be saved.
    //! @param p_format The format in which the Traces are to be saved.
    const bool Output_Traces(const std::string& p_traces_path,
                             const ELMO2::Traces& p_traces,
                             const std::string& p_format) const;
};
} // namespace ELMO2

#endif
