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

struct LowLevelFunctionInfo {
    uint32_t length;  /// length of machine code (in pairs of instructions)
    std::string name; /// function name
    uint32_t address; /// address in HW accelerator
    uint32_t* code;   /// machine code
};
