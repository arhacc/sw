//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>

#include <algorithm>
#include <cstdint>
#include <stdexcept>

#include "fmt/core.h"

AcceleratorImage::AcceleratorImage(const Arch& _arch, uint32_t _fill) {
    pc             = _fill;
    prevPc1        = _fill;
    prevPc2        = _fill;
    prevPc3        = _fill;
    nextPc         = _fill;
    cc             = _fill;
    nextInstrCtrl  = _fill;
    nextInstrArray = _fill;
    ctrlFlags      = _fill;
    ctrlAcc        = _fill;

    ctrlStack.resize(_arch.get(ArchConstant::RESOURCE_CTRL_STACK_SIZE));
    std::fill(ctrlStack.begin(), ctrlStack.end(), _fill);

    ctrlActiveLoopCounter = _fill;

    ctrlLoopCounters.resize(_arch.get(ArchConstant::CTRL_NR_LOOP_COUNTERS));
    std::fill(ctrlLoopCounters.begin(), ctrlLoopCounters.end(), _fill);

    ctrlDecrReg          = _fill;
    reduceNetOut         = _fill;
    boolScanOr           = _fill;
    ctrlAddrRegsSelector = _fill;

    ctrlAddrRegs.resize(_arch.get(ArchConstant::CONTROLLER_ADDR_REG_NR_LOCATIONS));
    std::fill(ctrlAddrRegs.begin(), ctrlAddrRegs.end(), _fill);

    ctrlMem.resize(_arch.get(ArchConstant::CONTROLLER_MEM_SIZE));
    std::fill(ctrlMem.begin(), ctrlMem.end(), _fill);

    ctrlInstrMemCtrl.resize(_arch.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    std::fill(ctrlInstrMemCtrl.begin(), ctrlInstrMemCtrl.end(), _fill);

    ctrlInstrMemArray.resize(_arch.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    std::fill(ctrlInstrMemArray.begin(), ctrlInstrMemArray.end(), _fill);

    arrayActivationReg.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    std::fill(arrayActivationReg.begin(), arrayActivationReg.end(), _fill);

    arrayBool.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    std::fill(arrayBool.begin(), arrayBool.end(), _fill);

    arrayAcc.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    std::fill(arrayAcc.begin(), arrayAcc.end(), _fill);

    arrayIORegData.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    std::fill(arrayIORegData.begin(), arrayIORegData.end(), _fill);

    arrayAddrReg.resize(2);
    std::for_each(arrayAddrReg.begin(), arrayAddrReg.end(), [_arch](auto& _v) {
        _v.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    });

    arrayGlobalShiftReg.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    std::fill(arrayGlobalShiftReg.begin(), arrayGlobalShiftReg.end(), _fill);

    arrayMem.resize(_arch.get(ArchConstant::ARRAY_CELL_MEM_SIZE));
    std::for_each(arrayMem.begin(), arrayMem.end(), [_arch](auto& _v) {
        _v.resize(_arch.get(ArchConstant::ARRAY_NR_CELLS));
    });
}

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

void AcceleratorImage::print(bool _full) {
    logWork.print("AcceleratorImage:\n\n");

    logWork.print("Controller\n\n");

    logWork.print(fmt::format("PC {}\n", pc));
    logWork.print(fmt::format("PC_pre1 {}\n", prevPc1));
    logWork.print(fmt::format("PC_pre2 {}\n", prevPc2));
    logWork.print(fmt::format("PC_pre3 {}\n", prevPc3));
    logWork.print(fmt::format("PC_next {}\n", nextPc));
    logWork.print(fmt::format("CC {}\n", cc));
    logWork.print(fmt::format("Next instr ctrl {}\n", nextInstrCtrl));
    logWork.print(fmt::format("Next instr array {}\n", nextInstrArray));
    logWork.print(fmt::format("Flags {}\n", ctrlFlags));
    logWork.print(fmt::format("ACC {}\n", ctrlAcc));

    uint32_t _ctrlStackIndex = 0;
    for (uint32_t _ctrlStackValue : ctrlStack) {
        logWork.print(fmt::format("Stack {} {}\n", _ctrlStackIndex++, _ctrlStackValue));
    }

    logWork.print(fmt::format("Loop counter actual {}\n", ctrlActiveLoopCounter));

    uint32_t _ctrlLoopCounterIndex = 0;
    for (uint32_t _ctrlLoopConter : ctrlLoopCounters) {
        logWork.print(fmt::format("Loop counter {} {}\n", _ctrlLoopCounterIndex++, _ctrlLoopConter));
    }

    logWork.print(fmt::format("Decrement reg {}\n", ctrlDecrReg));

    logWork.print(fmt::format("Reduce net out {}\n", reduceNetOut));
    logWork.print(fmt::format("Bool scan or {}\n", reduceNetOut));

    logWork.print(fmt::format("Addr regs selector {}\n", ctrlAddrRegsSelector));

    uint32_t _ctrlAddrRegIndex = 0;
    for (uint32_t _ctrlAddrReg : ctrlAddrRegs) {
        logWork.print(fmt::format("Loop counter {} {}\n", _ctrlAddrRegIndex++, _ctrlAddrReg));
    }
}
