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

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0)

# Download the HunterGate CMake file, if it is not already downloaded.
# See https://github.com/hunter-packages/gate
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake/HunterGate.cmake")
    file(
        DOWNLOAD
        "https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
        "${CMAKE_SOURCE_DIR}/cmake/HunterGate.cmake"
    )
endif()

# Hunter is used for managment of dependencies. See https://docs.hunter.sh
include("cmake/HunterGate.cmake")

# Keeps Hunter files within source subdirectory if nowhere has been explicitly
# specified
if(NOT DEFINED ENV{HUNTER_ROOT})
    set(HUNTER_ROOT ".hunter")
endif()

# Download Hunter
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.228.tar.gz"
    SHA1 "5f8bde642d44d2ef652c917fa6b36dbc5f693d51"
)

# Project's name
project(ELMO-2)

# Stricter warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++")

# Place executables into a 'bin' folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Place libraries into a 'lib' folder
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Recurse into the "src" subdirectory. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory(src)

# Recurse into the "doc" subdirectory.
add_subdirectory(doc)
