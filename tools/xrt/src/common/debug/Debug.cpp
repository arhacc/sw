//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>

#include <cstdint>
#include <stdexcept>

#include "fmt/core.h"

Breakpoint::Breakpoint(BreakpointCallback _callback, std::span<BreakpointCondition> _conditions, const Arch& _arch) {
    if (_conditions.size() > _arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS)) {
        throw std::runtime_error("More conditions than supported on arch");
    }

    callback = _callback;

    for (auto& _condition : _conditions) {
        conditions.push_back(_condition);
    }

    for (size_t i = conditions.size(); i < _arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS); i++) {
        conditions.push_back({
            _arch.get(ArchConstant::DEBUG_BP_COND_COND_ALWAYS_ACTIVE),
            0,
            0,
        });
    }
}

void AcceleratorImage::print(bool printMemory) {
    logWork.print("AcceleratorImage:\n");

    logWork.print(fmt::format("\tpc\t{}\n", pc));
    logWork.print(fmt::format("\tprevPc1\t{}\n", prevPc1));
    logWork.print(fmt::format("\tprevPc2\t{}\n", prevPc2));
    logWork.print(fmt::format("\tprevPc3\t{}\n", prevPc3));
    logWork.print(fmt::format("\tnextPc\t{}\n", nextPc));
    logWork.print(fmt::format("\tcc\t{}\n", cc));

    logWork.print(fmt::format("\tnextInstrCtrl\t{}\n", nextInstrCtrl));
    logWork.print(fmt::format("\tnextInstrArray\t{}\n", nextInstrArray));

    logWork.print(fmt::format("\tctrlFlags\t{}\n", ctrlFlags));
    logWork.print(fmt::format("\tctrlAcc\t{}\n", ctrlAcc));

    logWork.print("\tctrlStack:\n");
    for (uint32_t _ctrlStackValue : ctrlStack) {
        logWork.print(fmt::format("\t\t{}\n", _ctrlStackValue));
    }
}
