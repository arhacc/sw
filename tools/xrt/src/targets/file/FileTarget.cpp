//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <cstdint>
#include <sys/types.h>
#include <targets/file/FileTarget.h>

#include <cstdio>
#include <cinttypes>
#include <iomanip>
#include <targets/common/CodeGen.h>

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(std::string _path) :
    out(_path)
{}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction)
{
    if (!ctrl_col)
        out << " ";
    out << std::hex << std::setw(8) << std::setfill('0') << _instruction;
    if (!ctrl_col)
        out << std::endl;
    
    ctrl_col = !ctrl_col;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument)
{
    writeInstruction(makeInstruction(_instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length)
{
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(INSTRB_pload, _address);
    writeInstruction(INSTR_nop);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(INSTRB_prun, 0);
    writeInstruction(INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t *_args)
{
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(INSTRB_prun, _address);
    writeInstruction(INSTR_nop);

    while (_args && *_args) {
        writeInstruction(*_args++);
        writeInstruction(INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    

    printf("Writing array data from %p at addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_data), _address, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(INSTR_send_array_matrix_header);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _address);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

    printf("Reading array data to %p from addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_data), _address, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(INSTR_send_array_matrix_header);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _address);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(INSTR_nop);
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
