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
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length)
{
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(makeInstruction(INSTRB_pload, 0));
    writeInstruction(INSTR_nop);
    writeInstruction(makeInstruction(INSTRB_prun, 0));
    writeInstruction(INSTR_nop);

    writeInstruction(makeInstruction(INSTRB_pload, static_cast<uint16_t>(_address)));
    writeInstruction(INSTR_nop);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(makeInstruction(INSTRB_prun, 0));
    writeInstruction(INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t *_args)
{
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(makeInstruction(INSTRB_pload, _address));
    writeInstruction(INSTR_nop);

    while (_args && *_args) {
        writeInstruction(*_args++);
        writeInstruction(INSTR_nop);
    }
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
void FileTarget::readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void FileTarget::writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void FileTarget::dump(const std::string &_address) {

}

//-------------------------------------------------------------------------------------
