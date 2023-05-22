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
void FileTarget::runRuntime(uint32_t _address, uint32_t *_args) {
    throw std::runtime_error("wrong runRuntime function");
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
void FileTarget::writeArrayData(uint32_t _accAddress, uint32_t *_memAddress, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    

    printf("Writing array data from %p at addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_memAddress), _accAddress, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::readArrayData(uint32_t _accAddress, uint32_t *_memAddress, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

    printf("Reading array data to %p from addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_memAddress), _accAddress, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// UNIMPLEMENTED
//-------------------------------------------------------------------------------------
void FileTarget::reset() {
}

//-------------------------------------------------------------------------------------
void FileTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
void FileTarget::readRegister(uint32_t _address, uint32_t _register) {

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
