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
    @file IO.hpp
    @brief This file contains all input and output functions for GILES.
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#ifndef IO_HPP
#define IO_HPP

#include <string>  // for string

#include "Coefficients.hpp"  // for Coefficients

namespace GILES
{
namespace Internal
{
//! @class IO
//! @brief This contains all Input and Output functions for GILES. This includes
//! loading the Coefficients file.
struct IO
{
    const GILES::Internal::Coefficients
    Load_Coefficients(const std::string& p_coefficients_path) const;
};
}  // namespace Internal
}  // namespace GILES

#endif
