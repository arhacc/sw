//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct LowLevelFunctionInfo {
    std::vector<uint32_t> code; /// machine code
    std::string name;           /// function name
    uint32_t address;           /// address in HW accelerator

    inline size_t memLength() {
        return code.size() / 2;
    }
};
