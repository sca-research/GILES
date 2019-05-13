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

# C++17 is required for std::optional and init if statements
# C++14 is required for unique pointers
# C++11 is required for nlohmann_json
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add library that is built from the source files
add_library(lib${PROJECT_NAME} SHARED
    ELMO2.cpp
    Coefficients.cpp
    IO.cpp
    Validator_Coefficients.cpp

    # Model files
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/Hamming_Weight/Model_Hamming_Weight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/Power/Model_Power.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/Models/TEMPLATE/Model_TEMPLATE.cpp

    # Simulator files
    ${CMAKE_CURRENT_SOURCE_DIR}/Simulators/Thumb_Sim/Emulator_Thumb_Sim.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/Simulators/TEMPLATE/Emulator_TEMPLATE.cpp
)

target_compile_options(lib${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Weffc++
        -Wno-c++98-compat
        -Werror
        -fdiagnostics-color
)

find_package(Boost REQUIRED COMPONENTS system)

# If OpenMP is available then use it.
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(lib${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(lib${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Factory
        ${CMAKE_CURRENT_SOURCE_DIR}/Models
        ${CMAKE_CURRENT_SOURCE_DIR}/Simulators
        ${LIEF_INCLUDE_DIRS}
)

# Link to required external projects
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC
        Traces-Serialiser
        nlohmann_json::nlohmann_json
        fmt::fmt-header-only
        libthumb-sim
        Boost::system
)

# Removes lib prefix that is auto added, to stop it being called liblibEMLO-2
set_target_properties(lib${PROJECT_NAME} PROPERTIES PREFIX "")

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    # Add executable that is a command line wrapper around the library
    add_executable(${PROJECT_NAME} Main.cpp)

    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            lib${PROJECT_NAME}
            cxxopts
    )
endif()

