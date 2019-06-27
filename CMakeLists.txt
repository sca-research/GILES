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

# Specify the minimum required version for CMake
cmake_minimum_required(VERSION 3.0)

# Project's name
project(GILES)

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_PATCH 0)

# Stricter warnings
# TODO: Replace with this: https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/#leave-cmake_cxx_flags-alone
#set(Custom_Flags "${Custom_Flags} -Wall -Wextra -Wpedantic -Weffc++")

# Even stricter warnings when using clang only
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")

    #set(Custom_Flags "${Custom_Flags} -Weverything")

    # Disable unwanted warnings
    set(Custom_Flags "${Custom_Flags} -Wno-c++98-compat")
endif()

# Coloured compilation output
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")

# Performance profiling
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Warnings are errors
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# Place executables into a 'bin' folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Place libraries into a 'lib' folder
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(EXTERNAL_PROJECT_DIR ${PROJECT_SOURCE_DIR}/external)

# Recurse into the "test" subdirectory. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
# The "test" directory is added before "src" so that compile flags can be added
# if coverage is enabled.
add_subdirectory(test EXCLUDE_FROM_ALL)

add_subdirectory(doc)

# Build external projects
add_subdirectory(${EXTERNAL_PROJECT_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Custom_Flags}")
add_subdirectory(src)


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "General Instruction Leakage Simulator")
set(CPACK_PACKAGE_DESCRIPTION "A tool for simulating side-channel analysis traces")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/bristol-sca/GILES")
set(CPACK_PACKAGE_CONTACT "scott.egerton@bristol.ac.uk")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")

if(APPLE)
    set(CPACK_GENERATOR "productbuild" ${CPACK_GENERATOR})
endif()

set(CPACK_BINARY_DEB ON)

# There is a dependency bug here "Nothing provides libGILES"
set(CPACK_BINARY_RPM OFF)

set(CPACK_RPM_PACKAGE_PROVIDES lib${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_PROVIDES ${CPACK_RPM_PACKAGE_PROVIDES})

include(CPack)
