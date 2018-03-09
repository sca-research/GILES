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

option(BUILD_EXECUTABLE
    "Build ELMO-2 as a command line executable. (libELMO-2 will be built regardless)"
    ON
)

# C++17 is required for std::optional
# C++14 is required for unique pointers
# C++11 is required for nlohmann_json
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${PROJECT_SOURCE_DIR}/cmake/Third_Party.cmake")

# Add the models dynamically
# ******************************************************************************
include("${PROJECT_SOURCE_DIR}/cmake/Models.cmake")

#add_model("*NAME OF THE MODEL*", "*DESCRIPTION OF THE MODEL*", "ENABLED BY DEFAULT")
add_model("TEMPLATE" "Description of the TEMPLATE model" OFF)

configure_models()
# ******************************************************************************

# Add library called "libELMO-2" that is built from the source files
# ******************************************************************************
add_library(libELMO-2
    ELMO2.cpp
    Coefficients.cpp
    IO.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/Model_Factory.cpp
    ${MODEL_SOURCES} # all of the files required to compile the models
    Validator_Coefficients.cpp
)

get_external_header("cxxopts.hpp" "https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp")

target_include_directories(libELMO-2 SYSTEM PRIVATE ${THIRD_PARTY_DIR})

# Removes lib prefix that is auto added, to stop it being called liblibEMLO-2
SET_TARGET_PROPERTIES(libELMO-2 PROPERTIES PREFIX "")
# ******************************************************************************

if(BUILD_EXECUTABLE)
    get_external_header("nlohmann/json.hpp" "https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp")
    # Add executable "ELMO-2" that is a command line wrapper around libELMO-2
    add_executable(ELMO-2 Main.cpp)

    target_link_libraries(ELMO-2 libELMO-2)

    target_include_directories(ELMO-2 SYSTEM PRIVATE ${THIRD_PARTY_DIR})
endif(BUILD_EXECUTABLE)
