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

#include "Execution.hpp"

TEST_CASE("Execution class testing"
          "[execution]")
{
    GILES::Internal::Execution execution{3};

    SECTION("Add_Pipeline_Stage uint8_t")
    {
        REQUIRE_NOTHROW(execution.Add_Pipeline_Stage(
            "Execute", std::vector<std::uint8_t>{5, 1}));
    }

    SECTION("Add_Pipeline_Stage bool")
    {
        // Test adding different types
        REQUIRE_NOTHROW(execution.Add_Pipeline_Stage(
            "Fetch", std::vector<bool>{true, false}));
    }

    SECTION("Add_Registers_All correct size")
    {
        std::vector<std::map<std::string, std::size_t>> registers{
            {{"1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}},
            {{"1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}},
            {{"1", 4}, {"2", 10}, {"SP", 0xB}, {"A0", 7}}};

        REQUIRE_NOTHROW(execution.Add_Registers_All(registers));
    }

    SECTION("Add_Registers_Cycle & Check Registers")
    {
        REQUIRE_NOTHROW(execution.Add_Registers_Cycle(
            0, {{"1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}}));

        REQUIRE(execution.Is_Register("A0"));

        REQUIRE_FALSE(execution.Is_Register("false"));
    }

    SECTION("Add_Value & Get_Value")
    {
        REQUIRE_NOTHROW(
            execution.Add_Value<std::uint8_t>(0, "Execute", 0b0101101));

        REQUIRE_NOTHROW(execution.Get_Value<std::uint8_t>(0, "Execute"));

        REQUIRE_THROWS_WITH(
            execution.Get_Value<std::string>(0, "Execute"),
            "The requested pipeline state is not stored as the requested type");
    }

    SECTION("Get_Cycle_Count")
    {
        REQUIRE_NOTHROW(3 == execution.Get_Cycle_Count());
    }

    SECTION("Get_Instruction")
    {
        REQUIRE_NOTHROW(
            execution.Add_Value<std::string>(0, "Execute", "add r0, 10"));

        REQUIRE_NOTHROW(execution.Get_Instruction(0, "Execute"));
        REQUIRE_THROWS(execution.Get_Instruction(-1, "Execute"));
        REQUIRE_THROWS(execution.Get_Instruction(1, "Execute"));
        REQUIRE_THROWS(execution.Get_Instruction(0, "Fetch"));
    }

    SECTION("Get_Operand_Value - Operand")
    {
        REQUIRE_NOTHROW(execution.Add_Registers_Cycle(
            0, {{"r1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}}));

        // Register lookups.
        REQUIRE(9 == execution.Get_Operand_Value(0, "SP"));
        REQUIRE(0 == execution.Get_Operand_Value(0, "2"));

        // Not a register.
        REQUIRE(5 == execution.Get_Operand_Value(0, "5"));
    }

    SECTION("Get_Operand_Value - Instruction")
    {
        const GILES::Internal::Assembly_Instruction instruction{"add",
                                                                {"r1", "10"}};

        REQUIRE_NOTHROW(execution.Add_Registers_Cycle(
            0, {{"r1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}}));

        // Get register r1
        REQUIRE(1 == execution.Get_Operand_Value(0, instruction, 1));

        // Get value 10
        REQUIRE(10 == execution.Get_Operand_Value(0, instruction, 2));
    }

    SECTION("Get_Registers")
    {
        std::map<std::string, std::size_t> registers{
            {"r1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}};

        REQUIRE_NOTHROW(execution.Add_Registers_Cycle(0, registers));

        REQUIRE(registers == execution.Get_Registers(0));

        // Cycle 2 is within range but should be blank as nothing has been
        // added for this yet.
        REQUIRE_NOTHROW(execution.Get_Registers(2));

        REQUIRE_THROWS(execution.Get_Registers(7));
        REQUIRE_THROWS(execution.Get_Registers(-1));
    }

    SECTION("Get_Register_Value")
    {
        REQUIRE_NOTHROW(execution.Add_Registers_Cycle(
            0, {{"r1", 1}, {"2", 0}, {"SP", 9}, {"A0", 1}}));

        REQUIRE(1 == execution.Get_Register_Value(0, "r1"));

        REQUIRE_THROWS(execution.Get_Register_Value(2, "r1"));
        REQUIRE_THROWS(execution.Get_Register_Value(-1, "r1"));
        REQUIRE_THROWS(execution.Get_Register_Value(100, "r1"));
        REQUIRE_THROWS(execution.Get_Register_Value(0, "invalid"));
    }

    SECTION("Cycle state tests")
    {
        // Add a few test states.
        REQUIRE_NOTHROW(
            execution.Add_Value<std::uint8_t>(0, "Execute", 0b0101101));
        REQUIRE_NOTHROW(execution.Add_Value(
            0, "Stalled", GILES::Internal::Execution::State::Stalled));
        REQUIRE_NOTHROW(execution.Add_Value(
            1, "Flush", GILES::Internal::Execution::State::Flushing));

        // Perform the tests.

        // Test Get_State with valid parameters.
        REQUIRE(GILES::Internal::Execution::State::Normal ==
                execution.Get_State(0, "Execute"));

        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State(0, "Stalled"));

        REQUIRE(GILES::Internal::Execution::State::Flushing ==
                execution.Get_State(1, "Flush"));

        // TODO: Find a way of testing for report_error.

        // Test Get_State_Unsafe with valid parameters.
        REQUIRE(GILES::Internal::Execution::State::Normal ==
                execution.Get_State_Unsafe(0, "Execute"));

        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State_Unsafe(0, "Stalled"));

        REQUIRE(GILES::Internal::Execution::State::Flushing ==
                execution.Get_State_Unsafe(1, "Flush"));

        // Test Get_State_Unsafe with invalid parameters.
        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State_Unsafe(0, "Invalid"));

        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State_Unsafe(-1, "Execute"));

        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State_Unsafe(1, "Execute"));

        REQUIRE(GILES::Internal::Execution::State::Stalled ==
                execution.Get_State_Unsafe(100, "Execute"));

        // Test Is_Normal_State with valid parameters.
        REQUIRE(execution.Is_Normal_State(0, "Execute"));

        REQUIRE_FALSE(execution.Is_Normal_State(0, "Stalled"));

        REQUIRE_FALSE(execution.Is_Normal_State(1, "Flush"));

        // Test Is_Normal_State_Unsafe with valid parameters.
        REQUIRE(execution.Is_Normal_State_Unsafe(0, "Execute"));

        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(0, "Stalled"));

        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(1, "Flush"));

        // Test Is_Normal_State_Unsafe with invalid parameters.
        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(0, "Invalid"));

        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(-1, "Execute"));

        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(1, "Execute"));

        REQUIRE_FALSE(execution.Is_Normal_State_Unsafe(100, "Execute"));
    }
}
