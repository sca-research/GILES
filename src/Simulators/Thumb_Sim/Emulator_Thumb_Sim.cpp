/*
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
*/

/*!
    @file Emulator_Thumb_Sim.hpp
    @todo add brief
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <string>   // for string
#include <utility>  // for pair

#include <iostream>  // for temp debugging

#include "simulator/regfile.h"  // for Reg

#include "Emulator_Thumb_Sim.hpp"
#include "Execution.hpp"

const GILES::Internal::Execution GILES::Internal::Emulator_Thumb_Sim::Run_Code()
{
    m_simulator.run(m_program_path);

    m_execution_recording = m_simulator.Get_Cycle_Recorder();

    // Retrieve the results from the simulator
    const auto& fetch   = m_execution_recording.Get_Fetch();
    const auto& decode  = m_execution_recording.Get_Decode();
    const auto& execute = m_execution_recording.Get_Execute();

    // TODO: When are the registers recorded? Should it be once or between every
    // stage?
    const auto& registers = m_execution_recording.Get_Registers();

    // Create an Execution object and add the required data to it.
    Execution execution(m_execution_recording.Get_Cycle_Count());
    execution.Add_Registers_All(registers);
    execution.Add_Pipeline_Stage("Fetch", fetch);
    execution.Add_Pipeline_Stage("Decode", decode);
    execution.Add_Pipeline_Stage("Execute", execute);

    // Correctly place stalls and flushes so that they can be easily identified.
    // TODO: Flushes
    for (std::size_t i{0}; i < execute.size(); ++i)
    {
        if ("Stalled, pending decode" == execute[i])
        {
            execution.Add_Value(
                i, "Execute", GILES::Internal::Execution::State::Stalled);
        }
    }
    return execution;
}

const std::string& GILES::Internal::Emulator_Thumb_Sim::Get_Extra_Data()
{
    return m_execution_recording.Get_Extra_Data();
}

void GILES::Internal::Emulator_Thumb_Sim::Inject_Fault(
    const std::uint32_t p_cycle_to_fault,
    const std::string& p_register_to_fault,
    const std::uint8_t p_bit_to_fault)
{
    const Reg register_to_fault{[&p_register_to_fault] {
        if ("R0" == p_register_to_fault)
        {
            return Reg::R0;
        }
        if ("R1" == p_register_to_fault)
        {
            return Reg::R1;
        }
        if ("R2" == p_register_to_fault)
        {
            return Reg::R2;
        }
        if ("R3" == p_register_to_fault)
        {
            return Reg::R3;
        }
        if ("R4" == p_register_to_fault)
        {
            return Reg::R4;
        }
        if ("R5" == p_register_to_fault)
        {
            return Reg::R5;
        }
        if ("R6" == p_register_to_fault)
        {
            return Reg::R6;
        }
        if ("R7" == p_register_to_fault)
        {
            return Reg::R7;
        }
        if ("R8" == p_register_to_fault)
        {
            return Reg::R8;
        }
        if ("R9" == p_register_to_fault)
        {
            return Reg::R9;
        }
        if ("R10" == p_register_to_fault)
        {
            return Reg::R10;
        }
        if ("R11" == p_register_to_fault)
        {
            return Reg::R11;
        }
        if ("R12" == p_register_to_fault)
        {
            return Reg::R12;
        }
        if ("R13" == p_register_to_fault || "MSP" == p_register_to_fault)
        {
            return Reg::MSP;
        }
        if ("R14" == p_register_to_fault || "LR" == p_register_to_fault)
        {
            return Reg::LR;
        }
        if ("R15" == p_register_to_fault || "PC" == p_register_to_fault)
        {
            return Reg::PC;
        }
        if ("PSP" == p_register_to_fault)
        {
            return Reg::PSP;
        }
        if ("XPSR" == p_register_to_fault)
        {
            return Reg::XPSR;
        }
        if ("CONTROL" == p_register_to_fault)
        {
            return Reg::CONTROL;
        }
        Error::Report_Error("Could not find register with the name \"{}\"",
                            p_register_to_fault);
    }()};

    m_simulator.injectFault(
        p_cycle_to_fault, register_to_fault, p_bit_to_fault);
}

void GILES::Internal::Emulator_Thumb_Sim::Add_Timeout(
    const std::uint32_t p_number_of_cyclest)
{
    Error::Report_Warning("This feature is not yet implemented.");
}
