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

struct UserBreakpoint {
    std::optional<unsigned> id;
    std::optional<unsigned> hardwareBreakpointID;
    BreakpointCallback callback;
    uint32_t lineNumber;
};
