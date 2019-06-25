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

include("${PROJECT_SOURCE_DIR}/cmake/External.cmake")

# Downloads and updates all git submodules.
update_external_submodules()

# Build external projects
add_subdirectory(thumb-sim EXCLUDE_FROM_ALL)
add_subdirectory(Traces_Serialiser EXCLUDE_FROM_ALL)
add_subdirectory(json EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
