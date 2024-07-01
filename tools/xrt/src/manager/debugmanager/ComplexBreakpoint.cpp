#include <manager/debugmanager/ComplexBreakpoint.hpp>
#include <vector>
#include "common/arch/generated/ArchConstants.hpp"
#include <common/debug/Debug.hpp>
#include <common/arch/Arch.hpp>

ComplexBreakpointLineNumber::ComplexBreakpointLineNumber(uint32_t _lineNumber) : lineNumber(_lineNumber) {}

Breakpoint ComplexBreakpointLineNumber::toHwBreakpoint(const BreakpointSetLoadData &_breakpointLoadData) const {
    std::vector<BreakpointCondition> _breakpointConditions;

    _breakpointConditions.push_back({
        _breakpointLoadData.arch.get(ArchConstant::DEBUG_BP_COND_COND_EQUAL),
        _breakpointLoadData.arch.get(ArchConstant::DEBUG_BP_COND_OPERAND0_SEL_PC),
        _breakpointLoadData.baseAddress + lineNumber
    });

    return Breakpoint(_breakpointLoadData.breakpointCallback, _breakpointConditions, _breakpointLoadData.arch);
}

Breakpoint ComplexBreakpointAlwaysBreak::toHwBreakpoint(const BreakpointSetLoadData &_breakpointLoadData) const {
    std::vector<BreakpointCondition> _breakpointConditions;

    _breakpointConditions.push_back({
        _breakpointLoadData.arch.get(ArchConstant::DEBUG_BP_COND_COND_ALWAYS_ACTIVE),
        0,
        0
    });

    return Breakpoint(_breakpointLoadData.breakpointCallback, _breakpointConditions, _breakpointLoadData.arch);
}