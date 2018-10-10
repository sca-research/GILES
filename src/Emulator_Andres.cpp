/*
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
*/

/*!
    @file Emulator_Andres.hpp
    @todo add brief
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <string>   // for string
#include <utility>  // for pair

#include <iostream>  // for temp debugging

#include "Emulator_Andres.hpp"
#include "Execution.hpp"

#include "simulator.cpp"
#include "simulator/processor.h"

const ELMO2::Internal::Execution ELMO2::Internal::Emulator_Andres::Run_Code()
{
    Simulator sim;
    sim.run(m_program_path);

    // Retrieve the results from the simulator
    const auto& fetch     = Simulator_Debug::Debug::Get_Fetch();
    const auto& decode    = Simulator_Debug::Debug::Get_Decode();
    const auto& execute   = Simulator_Debug::Debug::Get_Execute();
    const auto& registers = Simulator_Debug::Debug::Get_Registers();

    // Create an Execution object and add the required data to it.
    Execution execution(Simulator_Debug::Debug::Get_Cycle_Count());

    for (size_t i = 0; i < Simulator_Debug::Debug::Get_Cycle_Count(); ++i)
    {
        execution.Add_Registers(i, registers.at(i));
    }

    execution.Add_Pipeline_Stage<std::pair<uint16_t, uint16_t>>("Fetch", fetch);
    execution.Add_Pipeline_Stage<std::string>("Decode", decode);
    execution.Add_Pipeline_Stage<std::string>("Execute", execute);

    // Correctly place stalls and flushes so that they can be easily identified.
    // TODO: Flushes
    for (size_t i = 0; i < execute.size(); ++i)
    {
        if ("Stalled, pending decode" == execute[i])
        {
            execution.Add_Value(
                i, "Execute", ELMO2::Internal::Execution::State::Stalled);
        }
    }
    return execution;
}
