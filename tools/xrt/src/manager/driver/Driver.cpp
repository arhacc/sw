//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <targets/Targets.hpp>
#include <targets/common/Future.hpp>

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "fmt/core.h"
#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
Driver::Driver(Manager* _ctx, Targets* _targets, Arch& _arch) : targets(_targets), ctx(_ctx), arch(_arch) {
    std::string _hwArch = fmt::format(
        "xpu_{:08X}{:08X}{:08X}{:08X}",
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR));

    logInit.print(
        fmt::format("Detected HW architecture {} will overwrite specified or default architecture\n", _hwArch));

    parseArchFile(_arch, _hwArch);

    breakpoints.resize(arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS));
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t _accMemStart, const MatrixView* _matrixView) {
    Future* _future = writeMatrixArrayAsync(_accMemStart, _matrixView);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady) {
    Future* _future = readMatrixArrayAsync(_accMemStart, _matrixView, _accRequireResultReady);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 1);
    runClockCycles(100);
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 0);
    runClockCycles(100);
}

//-------------------------------------------------------------------------------------
void Driver::runClockCycle() {
    targets->runClockCycle();
}

//-------------------------------------------------------------------------------------
void Driver::runClockCycles(unsigned _n) {
    targets->runClockCycles(_n);
}

//-------------------------------------------------------------------------------------
void Driver::resetBreakpoints() {
    for (unsigned i = 0; i < arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
        clearBreakpoint(i);
    }
}

//-------------------------------------------------------------------------------------
void Driver::run(uint32_t _address, std::span<const uint32_t> _args) {
    Future* _future = runAsync(_address, _args);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
Future* Driver::runAsync(uint32_t _address, std::span<const uint32_t> _args) {
    std::vector<Future*> _futures;

    _futures.push_back(writeInstructionAsync(arch.INSTRB_prun, _address));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    for (uint32_t _arg : _args) {
        _futures.push_back(writeInstructionAsync(_arg));
        _futures.push_back(writeInstructionAsync(arch.INSTR_nop));
    }

    return new MuxFuture(ctx, std::move(_futures));
}

//-------------------------------------------------------------------------------------
uint32_t Driver::readRegister(uint32_t _address) {
    uint32_t _data;

    Future* _future = readRegisterAsync(_address, &_data);
    _future->wait();
    delete _future;

    return _data;
}

//-------------------------------------------------------------------------------------
void Driver::writeRegister(uint32_t _address, uint32_t _register) {
    Future* _future = writeRegisterAsync(_address, _register);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
Future* Driver::readRegisterAsync(uint32_t _address, uint32_t* _dataLocation) {
    Future* _future = new RegisterReadFuture(ctx, _address, _dataLocation);

    targets->process(_future);

    return _future;
}

//-------------------------------------------------------------------------------------
Future* Driver::writeRegisterAsync(uint32_t _address, uint32_t _data) {
    Future* _future = new RegisterWriteFuture(ctx, _address, _data);

    targets->process(_future);

    return _future;
}

//-------------------------------------------------------------------------------------
Future* Driver::readMatrixArrayAsync(uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady) {
    logWork.print(fmt::format(
        "Reading matrix of size {}x{} at address {}", _matrixView->numRows(), _matrixView->numColumns(), _accMemStart));

    if (_accRequireResultReady) {
        logWork.print(" (waiting for result)\n");
    } else {
        logWork.print(" (not waiting for result)\n");
    }

    Future* _f0 = writeTransferInstructionAsync(
        _accRequireResultReady ? arch.INSTR_get_matrix_array_w_result_ready
                               : arch.INSTR_get_matrix_array_wo_result_ready);
    Future* _f1 = writeTransferInstructionAsync(_accMemStart);
    Future* _f2 = writeTransferInstructionAsync(_matrixView->numRows());
    Future* _f3 = writeTransferInstructionAsync(_matrixView->numColumns());

    // _f0->wait();
    // _f1->wait();
    // _f2->wait();
    // _f3->wait();

    Future* _f4 = new MatrixViewReadFuture(ctx, _matrixView);
    targets->process(_f4);

    return new MuxFuture(ctx, {_f0, _f1, _f2, _f3, _f4});
}

//-------------------------------------------------------------------------------------
Future* Driver::writeMatrixArrayAsync(uint32_t _accMemStart, const MatrixView* _matrixView) {
    logWork.print(fmt::format(
        "Writing matrix of size {}x{} at address {}\n",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

    Future* _f0 = writeTransferInstructionAsync(arch.INSTR_send_matrix_array);
    Future* _f1 = writeTransferInstructionAsync(_accMemStart);
    Future* _f2 = writeTransferInstructionAsync(_matrixView->numRows());
    Future* _f3 = writeTransferInstructionAsync(_matrixView->numColumns());

    // _f0->wait();
    // _f1->wait();
    // _f2->wait();
    // _f3->wait();

    Future* _f4 = new MatrixViewWriteFuture(ctx, _matrixView);
    targets->process(_f4);

    return new MuxFuture(ctx, {_f0, _f1, _f2, _f3, _f4});
}

//-------------------------------------------------------------------------------------
Future* Driver::writeInstructionAsync(uint32_t _instruction) {
    return writeRegisterAsync(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR), _instruction);
}

//-------------------------------------------------------------------------------------
Future* Driver::writeInstructionAsync(uint8_t _instructionByte, uint32_t _argument) {
    return writeInstructionAsync(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
Future* Driver::writeTransferInstructionAsync(uint32_t _instruction) {
    return writeRegisterAsync(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_DTE_FIFO_IN_ADDR), _instruction);
}

//-------------------------------------------------------------------------------------
Future* Driver::writeCodeAsync(uint32_t _address, std::span<const uint32_t> _code) {
    std::vector<Future*> _futures;

    _futures.push_back(writeInstructionAsync(arch.INSTRB_pload, _address));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    for (uint32_t _instruction : _code) {
        _futures.push_back(writeInstructionAsync(_instruction));
    }

    _futures.push_back(writeInstructionAsync(arch.INSTRB_prun, 0));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    return new MuxFuture(ctx, std::move(_futures));
}

//-------------------------------------------------------------------------------------
void Driver::writeCode(uint32_t _address, std::span<const uint32_t> _code) {
    Future* _future = writeCodeAsync(_address, _code);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint32_t _instruction) {
    Future* _future = writeInstructionAsync(_instruction);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
void Driver::writeTransferInstruction(uint32_t _instruction) {
    Future* _future = writeInstructionAsync(_instruction);
    _future->wait();
    delete _future;
}

//-------------------------------------------------------------------------------------
void Driver::writeInstructions(std::span<const uint32_t> _instructions) {
    for (uint32_t _instruction : _instructions) {
        writeInstruction(_instruction);
    }
}

//-------------------------------------------------------------------------------------
void Driver::registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID) {
    assert(_breakpoint.conditions.size() == arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS));

    if (_breakpointID >= arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS)) {
        throw std::runtime_error("Breakpoint ID out of range");
    }

    unsigned _lastConditionID = arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS);

    for (unsigned _conditionID = 0; _conditionID <= _lastConditionID; _conditionID++) {
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_OPERATION_ADDR),
            _breakpoint.conditions.at(_conditionID).condition);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_SEL_ADDR),
            _breakpoint.conditions.at(_conditionID).operand);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_COMP_VAL_ADDR),
            _breakpoint.conditions.at(_conditionID).value);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_MASK_ADDR), 0);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WANT_IN_DEPTH_DEBUG_ADDR), 0);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), _breakpointID);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_COND_ADDR), _conditionID);

        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), _conditionID == _lastConditionID);

        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);
    }
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoint(unsigned _breakpointID) {
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), _breakpointID);

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 0);

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);
}

//-------------------------------------------------------------------------------------