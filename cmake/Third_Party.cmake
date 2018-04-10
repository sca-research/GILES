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
option(${PROJECT_NAME}_UPDATE_EXTERNAL_HEADERS "Force re-download all external header files"
        OFF)

# This macro downloads the file specified by ${URL} and saves it to
# ${THIRD_PARTY_DIR}/${FILE_NAME}. This is to be used as a short hand notation
# to easily download dependencies.
macro(get_external_header FILE_NAME URL)

    # Download the file, if it is not already downloaded, or if the update
    # option is specified.
    if(${PROJECT_NAME}_UPDATE_EXTERNAL_HEADERS OR NOT EXISTS "${THIRD_PARTY_DIR}/${FILE_NAME}")
        message("Downloading dependency: ${FILE_NAME}")
        file(
                DOWNLOAD
                "${URL}"
                "${THIRD_PARTY_DIR}/${FILE_NAME}"
        )
    endif()
endmacro()
