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

option(${PROJECT_NAME}_BUILD_EXECUTABLE
    "Build ${PROJECT_NAME} as a command line executable. (The library will be built regardless)"
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
#add_model("TEMPLATE" "Description of the TEMPLATE model" OFF)
#add_model("Hamming_Weight" "Description of the TEMPLATE model" ON)

#configure_models()
# ******************************************************************************

# Add library called that is built from the source files
# ******************************************************************************
add_library(lib${PROJECT_NAME}
    ELMO2.cpp
    Coefficients.cpp
    IO.cpp
    Model_Factory.cpp
    Model_TEMPLATE.cpp
    ${MODEL_SOURCES} # all of the files required to compile the models
    Validator_Coefficients.cpp
)

get_external_header("cxxopts.hpp" "https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp")

get_external_header("nlohmann/json.hpp" "https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp")

target_include_directories(lib${PROJECT_NAME} SYSTEM PRIVATE ${THIRD_PARTY_DIR})

# Removes lib prefix that is auto added, to stop it being called liblibEMLO-2
SET_TARGET_PROPERTIES(lib${PROJECT_NAME} PROPERTIES PREFIX "")
# ******************************************************************************

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    # Add executable that is a command line wrapper around the library
    add_executable(${PROJECT_NAME} Main.cpp)

    target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${THIRD_PARTY_DIR})
endif(${PROJECT_NAME}_BUILD_EXECUTABLE)
