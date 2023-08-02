//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cinttypes>
#include <string>
#include <vector>

enum class ModFunctionArgumentType {
    SInteger,
    UInteger,
    Float,
    Ptr,
    Vector, // void * (data) + size_t (elements)
    Matrix, // void * (data) + size_t (cols) + size_t (rows)
    Tensor, // void * (data) + size_t * (dimensions) + size_t (number of dimentions)
};

struct ModFunctionArgumentPrimitive {
    ModFunctionArgumentType type; // Must be SInteger, UInteger or Float
    uint8_t size;
};

struct ModFunctionArgument {
    ModFunctionArgumentType type;

    union {
        uint8_t size;                           // If type is SInteger, UInteger or Float
        ModFunctionArgumentPrimitive primitive; // If type is vector, matrix or tensor
    };
};

struct ModFunctionInfo {
    std::string name;
    std::vector<ModFunctionArgument> args;
    void* addr;
};

std::vector<ModFunctionInfo> getKnownModFunctions();
