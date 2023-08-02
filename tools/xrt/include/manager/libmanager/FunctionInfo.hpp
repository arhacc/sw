//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>

enum class LibLevel {
    ANY_LEVEL,
    LOW_LEVEL,
    MID_LEVEL,
    HIGH_LEVEL,
};

struct FunctionInfo {
    LibLevel level;

    union {
        LowLevelFunctionInfo* lowLevel;
        const ModFunctionInfo* midLevel;
    };

    inline FunctionInfo(LowLevelFunctionInfo* _lowLevel)
        : level(LibLevel::LOW_LEVEL), lowLevel(_lowLevel) {}

    inline FunctionInfo(const ModFunctionInfo* _midLevel)
        : level(LibLevel::MID_LEVEL), midLevel(_midLevel) {}
};
