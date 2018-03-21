#[=[
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
#]=]

get_external_header("catch.hpp" "https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp")

# C++11 is required for nlohmann_json and catch
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Make test executable
set(TEST_SOURCES Test_Validator_Coefficients.cpp)
add_executable(tests ${TEST_SOURCES})

# Include the src being tested
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/src ${EXECUTABLE_OUTPUT_PATH})

target_link_libraries(tests libELMO-2)
target_include_directories(tests SYSTEM PRIVATE ${THIRD_PARTY_DIR})

enable_testing()
add_test(NAME Run_Tests COMMAND tests)
