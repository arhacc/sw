//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <cstdint>
#include <stdexcept>
#include <sys/types.h>
#include <targets/file/FileTarget.h>

#include <cstdio>
#include <cinttypes>
#include <iomanip>
#include <common/CodeGen.h>
#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(const std::string& _path, const Arch& _arch) :
    arch(_arch), out(_path)
{}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction) {
    if (!ctrl_col)
        out << " ";
    out << std::hex << std::setw(8) << std::setfill('0') << _instruction;
    if (!ctrl_col)
        out << std::endl;
    
    ctrl_col = !ctrl_col;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(arch.INSTRB_pload, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(arch.INSTRB_prun, 0);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(arch.INSTRB_prun, _address);
    writeInstruction(arch.INSTR_nop);

    while (_args && *_args) {
        writeInstruction(*_args++);
        writeInstruction(arch.INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) {
    fmt::print("FileTarget: Getting matrix array from 0x{:08x} of dimension {:4}x{:<4} into ram address 0x{:08x}",
            _accAddress, _numLines, _numColumns, _rawRamAddress);

    if (_waitResult) {
        fmt::println(" (waiting for result)");
    } else {
        fmt::println(" (not waiting for result)");
    }

    writeInstruction(_waitResult 
                    ? arch.INSTR_get_matrix_array_w_result_ready
                    : arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::sendMatrixArray(uint32_t _rawRamAddress, uint32_t _accAddress, uint32_t _numLines, uint32_t _numColumns) {
    fmt::println("FileTarget: Sending matrix array from ram address 0x{:08x} of dimension {:4}x{:<4} to 0x{:08x}",
            _rawRamAddress, _numLines, _numColumns, _accAddress);

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
// UNIMPLEMENTED
//-------------------------------------------------------------------------------------
void FileTarget::reset() {
}

//-------------------------------------------------------------------------------------
void FileTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
uint32_t FileTarget::readRegister(uint32_t _address) {
    fmt::println("WARNING: Reading register from file target");

    return 0;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void FileTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void FileTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

}

//-------------------------------------------------------------------------------------
void FileTarget::dump(const std::string &_address) {

}

//-------------------------------------------------------------------------------------
