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
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef IO_HPP
#define IO_HPP

#include <string> // for string

#include "Coefficients.hpp"

namespace ELMO2
{
namespace Internal
{
// Forward Declarations
class Traces;

//! @class IO
//! @brief This contains all Input and Output functions for ELMO2. This includes
//! loading the Coefficients file and saving generated Traces.
class IO // TODO: Maybe have an option to output just one trace (or a few) to a
         // seperate file?
{
public:
    //! All possible output formats for the generated traces to be saved as.
    enum class Output_Format : uint8_t // TODO: Should this be size_t instead?
    {
        Riscure
    };

    const ELMO2::Internal::Coefficients
    Load_Coefficients(const std::string& p_coefficients_path) const;

    bool Output_Traces(const std::string& p_traces_path,
                       const ELMO2::Internal::Traces& p_traces,
                       const Output_Format& p_format) const;
};
} // namespace Internal
} // namespace ELMO2

#endif
