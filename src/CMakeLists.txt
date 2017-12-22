#[=[
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
#]=]

# C++14 is required for unique pointers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#Add executable called "ELMO-2" that is built from the source files
add_executable(ELMO-2
    Main.cpp
    ELMO2.cpp
    Assembly_Instruction.hpp
    Coefficients.hpp
    Emulator_Interface.hpp
    Execution.hpp
    IO.hpp
    Model_Power.hpp
    Model.hpp
    Register.hpp
    Traces.hpp
    Unicorn_Interface.hpp
)
