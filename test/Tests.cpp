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
    @file Tests.cpp
    @brief Contains the entry point for the tests
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

//! Required when using Catch testing framework - Tells Catch to provide a
//! main()
//! @see https://github.com/catchorg/Catch2
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif  // CATCH_CONFIG_MAIN

#include <catch.hpp>  // for catch

// The actual tests
#include "Test_Coefficients.cpp"
#include "Test_Execution.cpp"
#include "Test_Factory.cpp"
#include "Test_Validator_Coefficients.cpp"
