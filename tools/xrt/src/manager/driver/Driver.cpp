//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
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
Driver::Driver(Targets* _targets, const Arch& _arch) : targets(_targets), arch(_arch) {
    reset();
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    fmt::println("SimTarget: Writing matrix");

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);

    targets->sendMatrixArray(
        _ramMatrix,
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns);
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady) {
    fmt::print("Driver: Reading matrix");

    if (_accRequireResultReady) {
        fmt::println(" (waiting for result)");
    } else {
        fmt::println(" (not waiting for result)");
    }

    writeInstruction(
        _accRequireResultReady ? arch.INSTR_get_matrix_array_w_result_ready
                               : arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);

    targets->getMatrixArray(
        _ramMatrix,
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns);
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();
}

//-------------------------------------------------------------------------------------
void Driver::run(uint32_t _address, std::span<const uint32_t> _args) {
    printf("Running code at 0x%016" PRIx32 "\n", _address);

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
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf(
        "length = %5zu (0x%016zu)\n",
        _code.size() / 2,
        _code.size() / 2); // length is in pairs of instructions

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
