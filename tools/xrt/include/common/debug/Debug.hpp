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
    std::vector<uint32_t> ctrlStack;
    uint32_t ctrlActiveLoopCounter; // is not written back
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
    std::vector<std::vector<uint32_t>> arrayAddrReg; // write layers reverse
    std::vector<uint32_t> arrayGlobalShiftReg;

    std::vector<std::vector<uint32_t>> arrayStack;
    std::vector<std::vector<uint32_t>> arrayMem;

    AcceleratorImage() = default;
    AcceleratorImage(const Arch& _arch, uint32_t _fill = 0xDEADBEEF);
    ~AcceleratorImage() = default;

    void print(bool printMemory = false);
};

typedef std::function<bool(AcceleratorImage&, uint32_t)> BreakpointCallback;

struct BreakpointCondition {
    unsigned condition;
    unsigned operand;
    uint32_t value;
};

struct Breakpoint {
    BreakpointCallback callback;

    std::vector<BreakpointCondition> conditions;

    Breakpoint(BreakpointCallback _callback, std::span<BreakpointCondition> _conditions, const Arch& _arch);
};
