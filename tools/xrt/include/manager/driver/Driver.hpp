//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <common/debug/Debug.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <span>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// forward declarations
class Targets;
class MatrixView;
class Manager;
struct Breakpoint;
class Future;

// interrupt
// IO_INTF_AXILITE_WRITE_REGS_SOFT_INT_ACK_ADDR
// IO_INTF_AXILITE_WRITE_REGS_DEBUG_INT_ACK_ADDR

//-------------------------------------------------------------------------------------
class Driver {
    Targets* targets;
    Manager* ctx;
    const Arch& arch;

    std::unique_ptr<AcceleratorImage> accImage;

    std::vector<std::unique_ptr<Breakpoint>> breakpoints;

    inline std::shared_ptr<Future> writeInstructionAsync(uint8_t _instructionByte, uint32_t _argument);

  public:
    Driver(Manager* _ctx, Targets* _targets, Arch& _arch);

    ~Driver() = default;

    void reset();
    void resetBreakpoints();

    void run(uint32_t _address, std::span<const uint32_t> _args);
    std::shared_ptr<Future> runAsync(uint32_t _address, std::span<const uint32_t> _args);

    void writeCode(uint32_t _address, std::span<const uint32_t> _code);
    std::shared_ptr<Future> writeCodeAsync(uint32_t _address, std::span<const uint32_t> _code);

    void process(std::shared_ptr<Future> _future);

    void runClockCycle();
    void runClockCycles(unsigned);

    void handleInterrupt();
    void handleBreakpointHit();
    void handleBreakpointHitFillAcceleratorImage(AcceleratorImage& _accImage);
    void handleBreakpointHitDumpAcceleratorImage(const AcceleratorImage& _accImage);
    unsigned handleBreakpointHitGetBreakpointID();

    void registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID);
    void clearBreakpoint(unsigned _breakpointID);
    unsigned nextAvailableBreakpoint();

    // TODO: maybe this needs to be removed and the accelerator should have it's own thread
    void continueAfterBreakpoint();

    uint32_t readRegister(uint32_t _address);
    void writeRegister(uint32_t _address, uint32_t _data);
    void readMatrixArray(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady);
    void writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView);

    std::shared_ptr<Future> readRegisterAsync(uint32_t _address, uint32_t* _dataLocation);
    std::shared_ptr<Future> writeRegisterAsync(uint32_t _address, uint32_t _data);
    std::shared_ptr<Future>
    readMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady);
    std::shared_ptr<Future> writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView);

    std::shared_ptr<Future> writeInstructionAsync(uint32_t _instruction);
    std::shared_ptr<Future> writeTransferInstructionAsync(uint32_t _instruction);

    void writeInstruction(uint32_t _instruction);
    void writeTransferInstruction(uint32_t _instruction);
    void writeInstructions(std::span<const uint32_t> _instruction);
};
//-------------------------------------------------------------------------------------
