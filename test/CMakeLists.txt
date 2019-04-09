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

cmake_minimum_required(VERSION 2.6)

include("${PROJECT_SOURCE_DIR}/cmake/External.cmake")

# C++11 is required for nlohmann_json and catch
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(${PROJECT_NAME}_CALCULATE_COVERAGE "Setup ready to calculate the code coverage of the unit tests. This adds profiling compiler flags, disables optimisations and adds the 'coverage' target. Requires Gcovr." OFF)

if(${PROJECT_NAME}_CALCULATE_COVERAGE)
    # Enable profiling for code coverage
    # PARENT_SCOPE is needed for these flags to persist outside this directory
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage" PARENT_SCOPE)
    # Second call is needed to set these in the current directory
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

    # A custom command to generate a html code coverage report using gcovr.
    # (https://gcovr.com/)
    # This will create a directory, "coverage", compile and run the tests and then
    # generate the coverage information for those tests.
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/coverage
        COMMAND ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}-tests
        COMMAND gcovr ${PROJECT_BINARY_DIR} -r ${PROJECT_SOURCE_DIR} --exclude-unreachable-branches --exclude-directories=test --html --html-details -o ${PROJECT_BINARY_DIR}/coverage/coverage.html
            DEPENDS ${PROJECT_NAME}-tests
            COMMENT "Generating HTML coverage report"
    )
endif(${PROJECT_NAME}_CALCULATE_COVERAGE)

# Make the test executable
add_executable(${PROJECT_NAME}-tests
    #Test_Validator_Coefficients.cpp
    #Test_Factory.cpp
    Tests.cpp
)


# Link to required external projects
target_link_libraries(${PROJECT_NAME}-tests PUBLIC nlohmann_json::nlohmann_json)

# Download and include required files from external projects
target_include_external_project(${PROJECT_NAME}-tests Catch2 single_include/catch2)

# Include the src being tested
target_include_directories(${PROJECT_NAME}-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${EXECUTABLE_OUTPUT_PATH})

target_link_libraries(${PROJECT_NAME}-tests PUBLIC lib${PROJECT_NAME})
#target_include_directories(${PROJECT_NAME}-tests SYSTEM PRIVATE ${THIRD_PARTY_DIR})

enable_testing()
add_test(NAME Run_Tests COMMAND ${PROJECT_NAME}-tests)
