//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
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

#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
Driver::Driver(Targets* _targets, const Arch& _arch) : targets(_targets), arch(_arch) {}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t _accMemStart, const MatrixView* _matrixView) {
    logWork.print("Driver: Writing matrix\n");

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_matrixView->numRows());
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_matrixView->numColumns());
    writeInstruction(arch.INSTR_nop);

    targets->sendMatrixArray(_matrixView);
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(
    uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady) {
    logWork.print("Driver: Reading matrix");

    if (_accRequireResultReady) {
        logWork.print(" (waiting for result)");
    } else {
        logWork.print(" (not waiting for result)");
    }

    writeInstruction(
        _accRequireResultReady ? arch.INSTR_get_matrix_array_w_result_ready
                               : arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_matrixView->numRows());
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_matrixView->numColumns());
    writeInstruction(arch.INSTR_nop);

    targets->getMatrixArray(_matrixView);
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();
}

//-------------------------------------------------------------------------------------
void Driver::run(uint32_t _address, std::span<const uint32_t> _args) {
    logWork.print(fmt::format("Running code at {:08x}\n", _address));

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
void Driver::writeCode(uint32_t _address, std::span<const uint32_t> _code) {
    logWork.print(fmt::format("Writing code at {:08x} ", _address));
    logWork.print(fmt::format("length = {:5} (:08x)\n", _code.size(), _code.size()));

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
