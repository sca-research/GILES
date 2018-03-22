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
    @file Test_Validator_Coefficients.cpp
    @brief Contains the tests for the Validator_Coefficients class.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#define CATCH_CONFIG_MAIN

#include <catch.hpp> // for catch

#include "Tests.cpp"

#include <nlohmann/json.hpp> // for json

#include "Validator_Coefficients.hpp"

TEST_CASE("JSON is not blank"
          "[!throws][coefficients][validation]")
{
    nlohmann::json json;

    SECTION("Empty JSON throws an exception")
    {
        // Ensures JSON object is valid JSON
        REQUIRE_NOTHROW(json = {});

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Coefficients file must not be empty."));
    }

    SECTION("Invalid as Coefficients, non empty JSON string")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
            "Invalid Coefficients")"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON object, found: "));
    }

    SECTION("Invalid as Coefficients, non empty JSON object")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "Invalid" : "Invalid"
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON object, found: "));
    }

    SECTION("Invalid as Coefficients, non empty JSON object containing "
            "'Coefficients' string")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" : "Coeffcients"
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON object, found: "));
    }

    SECTION("The JSON contains at empty object")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" : {}
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Coefficients file must not contain "
                                      "empty coefficient categories."));
    }

    SECTION("Invalid non empty Coefficients category")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Hello" : "World"
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each category in the Coefficients "
                                      "file must contain a Constant value."));
    }

    SECTION("Coefficients category missing Constant value")
    {
        json = R"({"ALU" : {"Coefficients" : {"Hello" : "World"}}})"_json;

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each category in the Coefficients "
                                      "file must contain a Constant value."));
    }

    SECTION("Coefficients category missing Coefficients")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each category in the Coefficients "
                                      "file must contain a set of "
                                      "Coefficients."));
    }

    SECTION("Constant value is not a number")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" : 
                    {
                        "Constant" : "Invalid",
                        "Coefficients" :
                        {
                            "Not tested" : "here"
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON number, found: "));
    }

    SECTION("Coefficients heading is not a JSON object")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" : "Invalid"
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON object, found: "));
    }

    SECTION("Interaction terms are not JSON arrays")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Invalid" : "String"
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON array, found: "));
    }

    SECTION("Interaction term is an empty array")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : []
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each interaction term in the "
                                      "Coefficients file must contain at least "
                                      "one value."));
    }

    SECTION("Interaction term contains invalid data")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, "Two", 3]
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON number, found: "));
    }

    SECTION("Empty instructions heading")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : []
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Categories in the Coefficients file "
                                      "must not contain an empty list of "
                                      "instructions"));
    }

    SECTION("Instructions heading not an array")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : "Invalid"
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON array, found: "));
    }

    SECTION("Instruction not a string")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["add", "mul", 0, "str"]
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Expected a JSON string, found: "));
    }

    SECTION("Instruction has same name as same heading")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["ALU"]
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each instruction in the Coefficients "
                                      "file must have only one set of "
                                      "Coefficients associated with it."));
    }

    SECTION("Instruction has same name as different heading - first category "
            "instruction, second category heading")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["Shifts"]
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each instruction in the Coefficients "
                                      "file must have only one set of "
                                      "Coefficients associated with it."));
    }

    SECTION("Instruction has same name as different heading - first category "
            "heading, second category instruction")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["ALU"]
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each instruction in the Coefficients "
                                      "file must have only one set of "
                                      "Coefficients associated with it."));
    }

    SECTION("Instruction in multiple categories")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["Twice"]
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["Twice"]
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("Each instruction in the Coefficients "
                                      "file must have only one set of "
                                      "Coefficients associated with it."));
    }

    SECTION(
        "Inconsistent number of interaction terms. Second category has more.")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3],
                            "Operand2" : [3, 2, 1, 0]
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("The same interaction terms must be "
                                      "provided for all categories in the "
                                      "Coefficients file."));
    }

    SECTION(
        "Inconsistent number of interaction terms. First category has more.")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3],
                            "Operand2" : [3, 2, 1, 0]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("The same interaction terms must be "
                                      "provided for all categories in the "
                                      "Coefficients file."));
    }

    SECTION("Inconsistent naming of interaction terms")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Invalid" : [0, 1, 2, 3]
                        }
                    }
                })"));

        REQUIRE_THROWS_WITH(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json),
            Catch::Matchers::Contains("The same interaction terms must be "
                                      "provided for all categories in the "
                                      "Coefficients file."));
    }

    // TODO: Do valid tests (this point on) need to be in a seperate TEST_CASE
    // macro?

    SECTION("Valid Coefficients, no categories")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    }
                })"));

        REQUIRE_NOTHROW(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json));
    }

    SECTION("Valid Coefficients, one category")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["add", "odd", "edd"]
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    }
                })"));

        REQUIRE_NOTHROW(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json));
    }

    SECTION("Valid Coefficients, second has category")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        }
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["lsls", "lsrs"]
                    }
                })"));

        REQUIRE_NOTHROW(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json));
    }

    SECTION("Valid Coefficients, two categories")
    {
        REQUIRE_NOTHROW(json = nlohmann::json::parse(R"(
                {
                    "ALU" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["add", "odd", "edd"]
                    },
                    "Shifts" :
                    {
                        "Constant" : 0,
                        "Coefficients" :
                        {
                            "Operand1" : [0, 1, 2, 3]
                        },
                        "Instructions" : ["lsls", "lsrs"]
                    }
                })"));

        REQUIRE_NOTHROW(
            ELMO2::Internal::Validator_Coefficients::Validate_Json(json));
    }
}
