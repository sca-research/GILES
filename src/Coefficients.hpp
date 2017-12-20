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
    @file Coefficients.hpp
    @brief Contains the internal representation of the Coefficients file.
    @author Scott Egerton
    @date 2017
    @copyright GNU General Public License Version 2+
*/

#ifndef COEFFICIENTS_HPP
#define COEFFICIENTS_HPP

namespace ELMO2
{
//! @class Coefficients
//! @brief The internal representation of the Coefficients file.
//! The Coefficients file contains the coefficients that were created by
//! measuring real hardware traces. Each instruction will have a corresponding
//! coefficient that will be used to calculate predicted traces by the Model
//! class.
//! TODO: Learn more about traces to fill in a detailed description of what the
//! coefficients file is.
//! TODO: add @see link to ELMO paper or something to further describe what the
//! Coefficients are.
class Coefficients
{
};
} // namespace ELMO2

#endif
