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
    @file Test_Validator_Coefficients.cpp
    @brief Contains the tests for the Validator_Coefficients class.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <catch.hpp>  // for catch

#include <nlohmann/json.hpp>  // for json

#include "Coefficients.hpp"
#include "Validator_Coefficients.hpp"

TEST_CASE("Coefficients"
          "[!throws][coefficients]")
{
    // A test Coefficients class is used for testing purposes.
    // This is its contents.
    const nlohmann::json json = R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3],
                            "Operand2" : [4, 5, 6],
                            "Hello" :
                            {
                                "World" : 4.0001,
                                "Hi" : 3.9999
                            }
                        },
                        "Instructions" : ["add", "odd", "edd"]
                    },
                    "Shifts" :
                    {
                        "Constant" : 1,
                        "Coefficients" :
                        {
                            "Operand1" : [7, 8, 9, 10],
                            "Operand2" : [11, 12, 13],
                            "Hello" :
                            {
                                "World" : 0,
                                "Hi" : -1
                            }
                        },
                        "Instructions" : ["lsls", "lsrs"]
                    },
                    "eors" :
                    {
                        "Constant" : 2.01,
                        "Coefficients" :
                        {
                            "Operand1" : [14, 15, 16, 17],
                            "Operand2" : [18, 19, 20],
                            "Hello" :
                            {
                                "World" : 14,
                                "Hi" : 5.0
                            }
                        }
                    }
                })"_json;

    // Ensure that this is valid JSON before using it to test the Coefficients
    // class.
    REQUIRE_NOTHROW(
        GILES::Internal::Validator_Coefficients::Validate_Json(json));

    GILES::Internal::Coefficients coefficients{json};

    SECTION("Get_Instruction_Category")
    {
        REQUIRE("ALU" == coefficients.Get_Instruction_Category("add"));
        REQUIRE("ALU" == coefficients.Get_Instruction_Category("odd"));
        REQUIRE("ALU" == coefficients.Get_Instruction_Category("edd"));
    }

    SECTION("Get_Coefficients")
    {
        REQUIRE(std::vector<double>{0, 1, 2, 3} ==
                coefficients.Get_Coefficients("add", "Operand1"));
        REQUIRE(std::vector<double>{0, 1, 2, 3} ==
                coefficients.Get_Coefficients("odd", "Operand1"));
        REQUIRE(std::vector<double>{4, 5, 6} ==
                coefficients.Get_Coefficients("odd", "Operand2"));
    }

    SECTION("Get_Coefficient")
    {
        REQUIRE(4.0001 ==
                coefficients.Get_Coefficient("add", "Hello", "World"));
        REQUIRE(3.9999 == coefficients.Get_Coefficient("odd", "Hello", "Hi"));

        REQUIRE(0 == coefficients.Get_Coefficient("lsls", "Hello", "World"));
        REQUIRE(-1 == coefficients.Get_Coefficient("lsrs", "Hello", "Hi"));

        REQUIRE(14 == coefficients.Get_Coefficient("eors", "Hello", "World"));
        REQUIRE(5.0 == coefficients.Get_Coefficient("eors", "Hello", "Hi"));
        /*
         *REQUIRE(std::vector<double>{0, 1, 2, 3} ==
         *        coefficients.Get_Coefficients("odd", "Operand1"));
         *REQUIRE(std::vector<double>{4, 5, 6} ==
         *        coefficients.Get_Coefficient("odd", "Operand2"));
         */
    }

    SECTION("Get_Constant")
    {
        REQUIRE(0 == coefficients.Get_Constant("add"));
        REQUIRE(0 == coefficients.Get_Constant("odd"));
        REQUIRE(0 == coefficients.Get_Constant("odd"));
        REQUIRE(1 == coefficients.Get_Constant("lsls"));
        REQUIRE(1 == coefficients.Get_Constant("lsrs"));
        REQUIRE(2.01 == coefficients.Get_Constant("eors"));

        REQUIRE_THROWS_WITH(coefficients.Get_Constant("Invalid"),
                            "This instruction (Invalid) was not found "
                            "within the Coefficients");
    }

    SECTION("Get_Interaction_Terms")
    {
        REQUIRE(
            std::unordered_set<std::string>{"Operand1", "Operand2", "Hello"} ==
            coefficients.Get_Interaction_Terms());
    }

    // TODO: Find a way of testing these invalid results that don't return.
    /*
     *SECTION("Get_Coefficients Invalid")
     *{
     *  REQUIRE_THROWS(coefficients.Get_Coefficients("add", "Operand2"));
     *            REQUIRE_THROWS(coefficients.Get_Coefficients("Invalid",
     *     "Operand1"));
     *
     *            REQUIRE_THROWS(coefficients.Get_Coefficients("add",
     *     "Invalid"));
     *
     *            REQUIRE_THROWS(coefficients.Get_Coefficients("Invalid",
     *     "Invalid"));
     *}
     */
}
