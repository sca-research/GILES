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
    @file Error.hpp
    @brief This file contains a custom error handler.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdlib>  // for exit, EXIT_FAILURE

#include <fmt/format.h>  // for print, vprint, make_format_args.

namespace ELMO2
{
namespace Internal
{
//! @class IO
//! @brief This contains all Input and Output functions for ELMO2. This includes
//! loading the Coefficients file and saving generated Traces.
struct Error
{
private:
    //! @brief Prints "Error: " followed by a formatted error message and stops
    //! execution.
    //! @note This function is marked as noreturn as it is guaranteed to always
    //! halt the program. (Through std::exit())
    //! @param p_format The format string to be printed.
    //! @param p_args The arguments to be printed in the format string.
    [[noreturn]] static void vreport_error(const char* p_format,
                                           fmt::format_args p_args)
    {
        fmt::print("Error: ");
        vreport_exit(p_format, p_args);
    }

    //! @brief Prints formatted message and stops execution.
    //! @note This function is marked as noreturn as it is guaranteed to always
    //! halt the program. (Through std::exit())
    //! @param p_format The format string to be printed.
    //! @param p_args The arguments to be printed in the format string.
    [[noreturn]] static void vreport_exit(const char* p_format,
                                          fmt::format_args p_args)
    {
        fmt::vprint(p_format, p_args);
        fmt::print("\n");
        std::exit(EXIT_FAILURE);
    }

public:
    //! @brief Prints "Error: " followed by a formatted error message and stops
    //! execution.
    //! @note This function is marked as noreturn as it is guaranteed to always
    //! halt the program. (Through std::exit())
    //! @param p_format The format string to be printed.
    //! @param p_args The arguments to be printed in the format string.:
    template <typename... Args>
    [[noreturn]] static constexpr void Report_Error(const char* p_format,
                                                    const Args&... p_args)
    {
        vreport_error(p_format, fmt::make_format_args(p_args...));
    }

    //! @brief Prints formatted message and stops execution.
    //! @note This function is marked as noreturn as it is guaranteed to always
    //! halt the program. (Through std::exit())
    //! @param p_format The format string to be printed.
    //! @param p_args The arguments to be printed in the format string.
    template <typename... Args>
    [[noreturn]] static constexpr void Report_Exit(const char* p_format,
                                                   const Args&... p_args)
    {
        vreport_exit(p_format, fmt::make_format_args(p_args...));
    }
};
}  // namespace Internal
}  // namespace ELMO2

#endif
