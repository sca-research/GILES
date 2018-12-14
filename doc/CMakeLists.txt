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

# TODO: Look into replacing this with DoxygenTargets from https://github.com/rpavlik/cmake-modules
# Maybe auto download?

find_package(Doxygen)

option(${PROJECT_NAME}_BUILD_DOCUMENTATION
    "Create the API documentation (requires Doxygen)" ${DOXYGEN_FOUND}
)

# if option is selected then build the documentation with target "all".
if(${PROJECT_NAME}_BUILD_DOCUMENTATION)
    set(ALL_OPTION ALL)
endif()

if(DOXYGEN_FOUND)
    # Replace directories with those of the current build directory etc.
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
    )

    # add a target to generate API documentation with Doxygen
    add_custom_target(doc ${ALL_OPTION}
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
