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

static constexpr
uint8_t makeInstuction(uint8_t opcode, uint8_t operand)
{
    return (opcode << 3) | ((operand & 0x7));
}

constexpr uint8_t ISA_pload = 13;
constexpr uint8_t ISA_prun  = 12;
constexpr uint8_t ISA_ctl   = 7;
constexpr uint8_t ISA_bwor  = 10;
constexpr uint8_t ISA_val   = 0;

constexpr uint8_t INSTR_pload = makeInstuction(ISA_pload, ISA_ctl);
constexpr uint8_t INSTR_prun  = makeInstuction(ISA_prun, ISA_ctl);
constexpr uint8_t INSTR_nop   = makeInstuction(ISA_bwor, ISA_val);

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
void FileTarget::writeInstruction(uint8_t _instruction, uint32_t _argument)
{
    writeInstruction(static_cast<uint32_t>(_instruction) << 24 | (_argument & 0xFF'FF'FF));
}

//-------------------------------------------------------------------------------------
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length)
{
    printf("Writing code at 0x%016" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(INSTR_pload, static_cast<uint16_t>(_address));
    writeInstruction(INSTR_nop, 0);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(INSTR_prun, 0);
    writeInstruction(INSTR_nop, 0);
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t *_args)
{
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(INSTR_pload, static_cast<uint16_t>(_address));
    writeInstruction(INSTR_nop, 0);

    while (_args && *_args) {
        writeInstruction(*_args);
        writeInstruction(INSTR_nop, 0);
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
