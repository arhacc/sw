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
#include <manager/driver/Driver.hpp>
#include <targets/Targets.hpp>

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
Driver::Driver(Targets* _targets, Arch& _arch) : targets(_targets), arch(_arch) {
    std::string _hwArch = fmt::format(
        "xpu_{:08X}{:08X}{:08X}{:08X}",
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR));

    logInit.print(fmt::format(
        "Detected HW architecture {} will overwrite specified or default architecture\n",
        _hwArch));

    parseArchFile(_arch, _hwArch);

    breakpoints.resize(arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS));
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t _accMemStart, const MatrixView* _matrixView) {
    logWork.print(fmt::format(
        "Writing matrix of size {}x{} at address {}\n",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

    writeTransferInstruction(arch.INSTR_send_matrix_array);
    writeTransferInstruction(_accMemStart);
    writeTransferInstruction(_matrixView->numRows());
    writeTransferInstruction(_matrixView->numColumns());

    targets->sendMatrixArray(_matrixView);
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(
    uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady) {
    logWork.print(fmt::format(
        "Reading matrix of size {}x{} at address {}",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

    if (_accRequireResultReady) {
        logWork.print(" (waiting for result)\n");
    } else {
        logWork.print(" (not waiting for result)\n");
    }

    writeTransferInstruction(
        _accRequireResultReady ? arch.INSTR_get_matrix_array_w_result_ready
                               : arch.INSTR_get_matrix_array_wo_result_ready);
    writeTransferInstruction(_accMemStart);
    writeTransferInstruction(_matrixView->numRows());
    writeTransferInstruction(_matrixView->numColumns());

    targets->getMatrixArray(_matrixView);
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();
}

//-------------------------------------------------------------------------------------
void Driver::resetBreakpoints() {
    for (unsigned i = 0; i < arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
        clearBreakpoint(i);
    }
}

//-------------------------------------------------------------------------------------
void Driver::run(uint32_t _address, std::span<const uint32_t> _args) {
    writeInstruction(arch.INSTRB_prun, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _arg : _args) {
        writeInstruction(_arg);
        writeInstruction(arch.INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
uint32_t Driver::readRegister(uint32_t _address) {
    return targets->readRegister(_address);
}

//-------------------------------------------------------------------------------------
void Driver::writeRegister(uint32_t _address, uint32_t _register) {
    targets->writeRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Driver::writeTransferInstruction(uint32_t _instruction) {
    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_DTE_FIFO_IN_ADDR),
        _instruction);
}

//-------------------------------------------------------------------------------------
void Driver::writeCode(uint32_t _address, std::span<const uint32_t> _code) {
    writeInstruction(arch.INSTRB_pload, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _instruction : _code) {
        writeInstruction(_instruction);
    }

    writeInstruction(arch.INSTRB_prun, 0);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint32_t _instruction) {
    targets->writeInstruction(_instruction);
}

//-------------------------------------------------------------------------------------
void Driver::writeInstructions(std::span<const uint32_t> _instructions) {
    targets->writeInstructions(_instructions);
}

//-------------------------------------------------------------------------------------
void Driver::registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID) {
    assert(
        _breakpoint.conditions.size() == arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS));

    if (_breakpointID >= arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS)) {
        throw std::runtime_error("Breakpoint ID out of range");
    }

    unsigned _lastConditionID = arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS);

    for (unsigned _conditionID = 0; _conditionID <= _lastConditionID; _conditionID++) {
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_OPERATION_ADDR),
            _breakpoint.conditions.at(_conditionID).condition);
        writeRegister(
            arch.get(
                ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_SEL_ADDR),
            _breakpoint.conditions.at(_conditionID).operand);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_COMP_VAL_ADDR),
            _breakpoint.conditions.at(_conditionID).value);
        writeRegister(
            arch.get(
                ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_MASK_ADDR),
            0);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WANT_IN_DEPTH_DEBUG_ADDR),
            0);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR),
            _breakpointID);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_COND_ADDR),
            _conditionID);

        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR),
            _conditionID == _lastConditionID);

        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR),
            1);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR),
            0);
    }
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoint(unsigned _breakpointID) {
    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), _breakpointID);

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 0);

    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);
}

//-------------------------------------------------------------------------------------