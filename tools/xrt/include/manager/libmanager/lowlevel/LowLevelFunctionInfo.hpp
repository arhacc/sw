//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/memmanager/UserBreakpoint.hpp>

#include <cstdint>
#include <string>
#include <vector>

struct LowLevelFunctionInfo {
    std::vector<uint32_t> code; /// machine code
    std::string name;           /// function name
    uint32_t address;           /// address in HW accelerator

    std::vector<UserBreakpoint> breakpoints;

    inline size_t memLength() const {
        return code.size() / 2;
    }
};
