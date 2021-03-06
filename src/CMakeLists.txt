#[=[
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
#]=]

option(${PROJECT_NAME}_BUILD_EXECUTABLE
    "Build ${PROJECT_NAME} as a command line executable. (The library will be built regardless)"
    ON
)

# Add library that is built from the source files
add_library(lib${PROJECT_NAME} SHARED
    GILES.cpp
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
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall
            -Wextra
            -Wpedantic
            -Weffc++
            -fdiagnostics-color>
        # Enable -Weverything when using clang
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic>
)

find_package(Boost REQUIRED COMPONENTS system program_options)

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
)

# Link to required external projects
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC
        Traces-Serialiser
        nlohmann_json::nlohmann_json
        fmt::fmt-header-only
        libthumb-sim
        Boost::system
        Boost::program_options
)

set_target_properties(lib${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    # C++17 is required for std::optional and init if statements
    # C++14 is required for unique pointers
    # C++11 is required for nlohmann_json
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS lib${PROJECT_NAME} DESTINATION lib)

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    # Add executable that is a command line wrapper around the library
    add_executable(${PROJECT_NAME} Main.cpp)

    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            lib${PROJECT_NAME}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        # C++17 is required for std::optional and init if statements
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    add_dependencies(${PROJECT_NAME} lib${PROJECT_NAME} libthumb-sim)

    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

