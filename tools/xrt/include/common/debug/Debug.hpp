//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <vector>

struct Arch;

struct AcceleratorImage {
    uint32_t pc;
    uint32_t prevPc1;
    uint32_t prevPc2;
    uint32_t prevPc3;
    uint32_t nextPc;

    uint32_t cc;

    uint32_t nextInstrCtrl;
    uint32_t nextInstrArray;

    uint32_t ctrlFlags;
    uint32_t ctrlAcc;
    uint32_t ctrlActiveLoopCounter;
    std::vector<uint32_t> ctrlLoopCounters;
    uint32_t ctrlDecrReg;

    uint32_t reduceNetOut;
    uint32_t boolScanOr;
    uint32_t ctrlAddrRegsSelector;
    std::vector<uint32_t> ctrlAddrRegs;

    std::vector<uint32_t> ctrlMem;
    std::vector<uint32_t> ctrlInstrMemCtrl;
    std::vector<uint32_t> ctrlInstrMemArray;

    std::vector<uint32_t> arrayActivationReg;
    std::vector<uint32_t> arrayBool;
    std::vector<uint32_t> arrayAcc;
    std::vector<uint32_t> arrayIORegData;
    std::vector<uint32_t> arrayAddrRegLayer0;
    std::vector<uint32_t> arrayAddrRegLayer1;
    std::vector<uint32_t> arrayGlobalShiftReg;

    std::vector<std::vector<uint32_t>> arrayStack;
    std::vector<std::vector<uint32_t>> arrayMem;
};

typedef std::function<void(AcceleratorImage&)> BreakpointCallback;

struct BreakpointCondition {
    unsigned condition;
    unsigned operand;
    uint32_t value;
};

struct Breakpoint {
    BreakpointCallback callback;

    std::vector<BreakpointCondition> conditions;

    Breakpoint(
        BreakpointCallback _callback,
        std::span<BreakpointCondition> _conditions,
        const Arch& _arch);
};