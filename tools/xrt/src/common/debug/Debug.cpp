//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <common/debug/Debug.hpp>

#include <stdexcept>

Breakpoint::Breakpoint(
    BreakpointCallback _callback,
    std::span<BreakpointCondition> _conditions,
    const Arch& _arch) {
    if (_conditions.size() > _arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS)) {
        throw std::runtime_error("More conditions than supported on arch");
    }

    callback = _callback;

    for (auto& _condition : _conditions) {
        conditions.push_back(_condition);
    }

    for (size_t i = conditions.size();
         i < _arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS);
         i++) {
        conditions.push_back({
            _arch.get(ArchConstant::DEBUG_BP_COND_COND_ALWAYS_ACTIVE),
            0,
            0,
        });
    }
}