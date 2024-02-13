//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/debug/Debug.hpp>

#include <cstdint>
#include <optional>
#include <string_view>

struct UserBreakpoint {
    std::optional<unsigned> id;
    std::optional<unsigned> hardwareBreakpointID;
    BreakpointCallback callback;
    std::string_view functionName;
    uint32_t lineNumber;
};
