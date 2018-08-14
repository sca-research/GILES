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

# This option adds all dependencies to be re-downloaded as a method of updating
# them. Dependencies that had not yet been downloaded will be downloaded.
option(${PROJECT_NAME}_UPDATE_EXTERNAL_PROJECTS "Force re-download all external project files (Requires Git)"
        OFF)

# This macro will clone the git submodule given by NAME into EXTERNAL_PROJECT_DIR if
# it is not already there and include the directory within that project given by
# INCLUDE_DIRECTORY in the building of TARGET.
macro(target_include_external_project TARGET NAME INCLUDE_DIRECTORY)
    if(${PROJECT_NAME}_UPDATE_EXTERNAL_HEADERS OR
        NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${NAME})

        # Git is required to clone the submodules. Ensure this is found first.
        find_package(Git REQUIRED)

        # Checks whether the directory given by EXTERNAL_PROJECT_DIR exists, if it
        # does not then CMake will create it.
        if(NOT EXISTS ${EXTERNAL_PROJECT_DIR})
            file(MAKE_DIRECTORY ${EXTERNAL_PROJECT_DIR})
        endif()

        # Clone/Update the submodule
        execute_process(COMMAND git submodule update --init -- ${NAME}
                        WORKING_DIRECTORY ${EXTERNAL_PROJECT_DIR})

        target_include_directories(${TARGET} SYSTEM PRIVATE
                                ${EXTERNAL_PROJECT_DIR}/${NAME}/${INCLUDE_DIRECTORY})
    endif()
endmacro()
