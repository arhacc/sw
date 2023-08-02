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
    uint32_t length;  /// length of machine code
    std::string name; /// length of machine code
    uint32_t address; /// address in HW accelerator
    uint32_t* code;   /// machine code
};
