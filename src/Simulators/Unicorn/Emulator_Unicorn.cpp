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
    @file Emulator_UNICORN.hpp
    @todo add brief
    @author Scott Egerton
    @date 2017-2019
    @copyright GNU Affero General Public License Version 3+
*/

#include <cstdint>  // for uint16_t, uint32_t, uint64_t
#include <memory>   // for unique_ptr

#include <fstream>
#include <iterator>
#include <vector>

#include <LIEF/LIEF.hpp>
#include <unicorn/unicorn.h>

#include "fmt/format.h"

#include "Emulator_Unicorn.hpp"
#include "Error.hpp"
#include "Execution.hpp"

// code to be emulated
#define ARM_CODE                                                               \
    "\x37\x00\xa0\xe3\x03\x10\x42\xe0"  // mov r0, #0x37; sub r1, r2, r3
#define THUMB_CODE "\x83\xb0"           // sub    sp, #0xc

// memory address where emulation starts
//#define ADDRESS 0x10000

static void hook_code(uc_engine* uc,
                      const std::uint64_t address,
                      const std::uint32_t size,
                      const void* user_data)
{
    (void)uc;
    (void)user_data;

    std::uint16_t sp, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;

    uc_reg_read(uc, UC_ARM_REG_SP, &sp);
    uc_reg_read(uc, UC_ARM_REG_SP, &r0);
    uc_reg_read(uc, UC_ARM_REG_SP, &r1);
    uc_reg_read(uc, UC_ARM_REG_SP, &r2);
    uc_reg_read(uc, UC_ARM_REG_SP, &r3);
    uc_reg_read(uc, UC_ARM_REG_SP, &r4);
    uc_reg_read(uc, UC_ARM_REG_SP, &r5);
    uc_reg_read(uc, UC_ARM_REG_SP, &r6);
    uc_reg_read(uc, UC_ARM_REG_SP, &r7);
    uc_reg_read(uc, UC_ARM_REG_SP, &r8);
    uc_reg_read(uc, UC_ARM_REG_SP, &r9);
    uc_reg_read(uc, UC_ARM_REG_SP, &r10);
    uc_reg_read(uc, UC_ARM_REG_SP, &r11);
    uc_reg_read(uc, UC_ARM_REG_SP, &r12);

    fmt::print("Executing at {}, ilen = {}\n"
               "R0: {}\n"
               "R1: {}\n"
               "R2: {}\n"
               "R3: {}\n"
               "R4: {}\n"
               "R5: {}\n"
               "R6: {}\n"
               "R7: {}\n"
               "R8: {}\n"
               "R9: {}\n"
               "R10: {}\n"
               "R11: {}\n"
               "R12: {}\n"
               "user_data: {}\n\n",
               address,
               size,
               r0,
               r1,
               r2,
               r3,
               r4,
               r5,
               r6,
               r7,
               r8,
               r9,
               r10,
               r11,
               r12,
               user_data);
}

const GILES::Internal::Execution GILES::Internal::Emulator_Unicorn::Run_Code()
{
    std::ifstream input(/*"/home/se17322/Downloads/thumb_simulator/examples/"*/
                        /*"naive-aes/naive-aes.bin"*/
                        "/home/se17322/build/GILES/bin/GILES",
                        std::ios::binary);

    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input),
                                      {});

    // *** Place your code here ***
    //! @note m_program_path Should contain the path to the target program.
    //! @note invoke_emulator(...) will return the output of a binary as a
    //! string. This can be used if the emulator is a binary.
    // std::unique_ptr<uc_engine> uc{uc_engine{}};  // TODO: Can this be
    // simpler?
    uc_hook trace;

    int sp = 0x8;  // R0 register

    printf("Emulate THUMB code\n");

    // Initialize emulator in ARM mode
    if (const auto& error = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc))
    {
        Error::Report_Error(
            "Failed on uc_open() with error returned: {} ({})\n",
            error,
            uc_strerror(error));
    }

    // map 2MB memory for this emulation
    /*
     *    uc_mem_map(uc, ADDRESS, 2 * 1024 * 1024, UC_PROT_ALL);
     *
     *    // write machine code to be emulated to memory
     *    // uc_mem_write(uc, ADDRESS, THUMB_CODE, sizeof(THUMB_CODE) - 1);
     *    uc_mem_write(uc, ADDRESS, (void*)buffer.data(), sizeof(buffer.data())
     * - 1);
     *
     *    // initialize machine registers
     *    uc_reg_write(uc, UC_ARM_REG_SP, &sp);
     */

    const auto start_address =
        elf_loader("/home/se17322/projects/test-aes/naive-aes.elf");
    //"/home/se17322/build/GILES/bin/GILES");
    //
    /*
     *uc_reg_write(uc, UC_ARM_REG_SP, &sp);
     */

    fmt::print("start_address: {}\n", start_address);
    /*
     *elf_loader("/home/se17322/Downloads/thumb_simulator/examples/"
     *           "naive-aes/naive-aes.elf");
     */

    // tracing one instruction at ADDRESS with customized callback
    // uc_hook_add(uc, &trace, UC_HOOK_CODE, (void*)&hook_code, NULL, 1, 0);

    // emulate machine code in infinite time (last param = 0), or when
    // finishing all the code.
    // Note we start at ADDRESS | 1 to indicate THUMB mode.
    // err = uc_emu_start(uc, ADDRESS | 1, ADDRESS + sizeof(THUMB_CODE) - 1, 0,
    // 0);
    if (const auto& error = uc_emu_start(
            uc, start_address | 1, start_address + buffer.size() - 1, 0, 0))
    {
        printf("Failed on uc_emu_start() with error returned: %u\n", error);
    }

    // now print out some registers
    printf(">>> Emulation done. Below is the CPU context\n");

    std::uint16_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13,
        r14, r15;

    uc_reg_read(uc, UC_ARM_REG_SP, &sp);
    printf(">>> SP = 0x%x\n", sp);

    uc_reg_read(uc, UC_ARM_REG_SP, &sp);
    uc_reg_read(uc, UC_ARM_REG_R0, &r0);
    uc_reg_read(uc, UC_ARM_REG_R1, &r1);
    uc_reg_read(uc, UC_ARM_REG_R2, &r2);
    uc_reg_read(uc, UC_ARM_REG_R3, &r3);
    uc_reg_read(uc, UC_ARM_REG_R4, &r4);
    uc_reg_read(uc, UC_ARM_REG_R5, &r5);
    uc_reg_read(uc, UC_ARM_REG_R6, &r6);
    uc_reg_read(uc, UC_ARM_REG_R7, &r7);
    uc_reg_read(uc, UC_ARM_REG_R8, &r8);
    uc_reg_read(uc, UC_ARM_REG_R9, &r9);
    uc_reg_read(uc, UC_ARM_REG_R10, &r10);
    uc_reg_read(uc, UC_ARM_REG_R11, &r11);
    uc_reg_read(uc, UC_ARM_REG_R12, &r12);
    uc_reg_read(uc, UC_ARM_REG_R13, &r13);
    uc_reg_read(uc, UC_ARM_REG_R14, &r14);
    uc_reg_read(uc, UC_ARM_REG_R15, &r15);

    fmt::print("R0: {}\n"
               "R1: {}\n"
               "R2: {}\n"
               "R3: {}\n"
               "R4: {}\n"
               "R5: {}\n"
               "R6: {}\n"
               "R7: {}\n"
               "R8: {}\n"
               "R9: {}\n"
               "R10: {}\n"
               "R11: {}\n"
               "R12: {}\n"
               "R13: {}\n"
               "R14: {}\n"
               "R15: {}\n\n",
               r0,
               r1,
               r2,
               r3,
               r4,
               r5,
               r6,
               r7,
               r8,
               r9,
               r10,
               r11,
               r12,
               r13,
               r14,
               r15);

    uc_close(uc);

    return Execution{5};  // TODO: Change this.
}

const std::string& GILES::Internal::Emulator_Unicorn::Get_Extra_Data()
{
    const auto string = new std::string();
    return *string;
    // return GILES::Internal::Emulator_Unicorn::Get_Name();
}

// Load an .elf file into unicorn's memory using LIEF
std::uint64_t
GILES::Internal::Emulator_Unicorn::elf_loader(const std::string& p_file)
{
    const auto elf_file = LIEF::ELF::Parser::parse(p_file);
    std::cout << *elf_file;
    if (elf_file->segments().size() == 0)
    {
        // if there are no segments, still attempt to map.text area
        const auto section = elf_file->get_section(".text");

        mem_write(section);
    }
    for (const auto& segment : elf_file->segments())
    {
        fmt::print("Found segment\n");
        if (segment.type() != LIEF::ELF::SEGMENT_TYPES::PT_LOAD)
        {
            continue;
        }
        for (const auto& section : segment.sections())
        {
            fmt::print("mem_write section {}\n", section.name());
            mem_write(section);
            continue;
        }
    }
    // Handle relocations
    const auto relocations = elf_file->pltgot_relocations();
    /*
     *for (const LIEF::ELF::Relocation& relocation :
     *     elf_file->dynamic_relocations)
     */
    for (auto it = std::begin(relocations); it != std::end(relocations); ++it)
    {
        fmt::print("Relocation");
    }

    /*
     *    emu.functions = {};
     *
     *    // Handle relocations
     *    for (const auto relocation : elf_file->relocations)
     *    {
     *        if (relocation.symbol.is_function())
     *        {
     *            if (relocation.symbol.value == 0)
     *            {
     *                rsv = relocation.address - (relocation.address & 1);
     *            }
     *            else
     *            {
     *                rsv = relocation.symbol.value -
     * (relocation.symbol.value & 1);
     *            }
     *            emu.functions[relocation.symbol.name] = rsv;
     *            emu[relocation.address]               = rsv;
     *        }
     *    }
     */

    // TODO: when the ELF has relocated functions exported, LIEF fails on
    // get_function_address
    /*
     *    for (const auto i : elf_file->exported_functions)
     *    {
     *        emu.functions.update({
     *            i : ((elf_file->get_function_address(i) >> 1) << 1)
     *        });  // failsafe for arm thumb
     *    }
     *
     *    emu.function_names = {emu.functions[x]: x for x in
     * emu.functions.keys()};
     */
    return elf_file->entrypoint();
}

void GILES::Internal::Emulator_Unicorn::mem_write(
    const LIEF::ELF::Section& p_section)
{
    const auto& address = p_section.virtual_address();
    const auto& size    = p_section.size();

    map_space(address, address + size);

    uc_mem_write(uc, address, (void*)&p_section.content().data()[0], size);
}

void GILES::Internal::Emulator_Unicorn::mem_write(
    const LIEF::ELF::Segment& p_segment)
{
    const auto& address = p_segment.virtual_address();
    const auto& size    = p_segment.virtual_size();

    map_space(address, address + size);

    uc_mem_write(uc, address, (void*)&p_segment.content().data()[0], size);
}

std::set<std::pair<std::uint64_t, std::uint64_t>> mapped_regions;

//! @todo: Document
// Maps area into the unicorn emulator between p_start and p_end, or nothing
// if it was already mapped. Only completes missing portions if there is
// overlapping with p_start previously-mapped segment
void GILES::Internal::Emulator_Unicorn::map_space(const std::uint64_t p_start,
                                                  const std::uint64_t p_end)
{
    // If the region given by p_start and p_end is contained within a
    // pre-existing region then do nothing to avoid overlaps.
    if (std::any_of(std::begin(mapped_regions),
                    std::end(mapped_regions),
                    [&p_start, &p_end](const auto& region) {
                        return p_start >= region.first &&
                               p_end <= region.second;
                    }))
    {
        return;
    }

    // If the region is empty then it does not need to be mapped.
    if (p_start == p_end)
    {
        return;
    }

    uint64_t new_start{p_start};
    uint64_t new_end{p_end};

    // Check for overlaps
    for (const auto region : mapped_regions)
    {
        // These are just to make it a bit more readable.
        const std::uint64_t region_start{region.first};
        const std::uint64_t region_end{region.second};

        // If there is an overlap with an existing region (p_end is within
        // the region).
        if (p_start < region_start && region_start < p_end &&
            p_end <= region_end)
        {
            // Truncate this region to avoid overlapping with region.
            new_end = region_start;
            break;
        }
        // If there is an overlap with an existing region (p_start is within
        // the region).
        else if (region_end > p_start && p_start >= region_start &&
                 p_end > region_end)
        {
            // Truncate this region to avoid overlapping with region.
            new_start = region_end;
            break;
        }
    }

    std::size_t page_size;
    uc_query(uc, UC_QUERY_PAGE_SIZE, &page_size);
    std::size_t page_shift{sizeof(page_size) - 1};

    uint64_t address{new_start};

    // Move the start address to work with page sizes if needed.
    if (address & (page_size - 1))
    {
        // address = (address + page_size - 1) & ~(page_size - 1);
        address = (address >> page_shift) << page_shift;
    }

    uint64_t size{new_end - address};

    // Change the size so that it fits within the page size if needed.
    if (size & (page_size - 1))
    {
        // size = (size + page_size - 1) & ~(page_size - 1);
        size = ((size >> page_shift) << page_shift) + page_size;
    }

    uc_mem_map(uc, address, size, 0);
    // uc_mem_map(uc, address, 2 * 1024 * 1024, 0);
    mapped_regions.emplace(address, address + size);
}
