//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <targets/file/FileTarget.h>

#include <cstdio>
#include <cinttypes>
#include <iomanip>

#define PLOAD_INSTRUCTION 0x6f00
#define PRUN_INSTRUCTION 0x6700
#define NOP_INSTRUCTION 0x5000

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(std::string _path) :
    out(_path)
{}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction)
{
    out << std::hex << std::setw(8) << std::setfill('0') << _instruction << std::endl;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint16_t _instruction, uint16_t _argument)
{
    writeInstruction(static_cast<uint32_t>(_instruction) << 16 | _argument);
}

//-------------------------------------------------------------------------------------
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length)
{
    printf("Writing code at 0x%016" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(PLOAD_INSTRUCTION, static_cast<uint16_t>(_address));
    writeInstruction(NOP_INSTRUCTION, 0);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t *_args)
{
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(PRUN_INSTRUCTION, static_cast<uint16_t>(_address));
    writeInstruction(NOP_INSTRUCTION, 0);

    // TODO: handle arguments
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
