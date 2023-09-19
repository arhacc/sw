//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/goldenmodel/GoldenModelTarget.h>

#include <cstdint>
#include <cstdio>

//-------------------------------------------------------------------------------------
void GoldenModelTarget::reset() {}

//-------------------------------------------------------------------------------------
uint32_t GoldenModelTarget::readRegister(uint32_t _address) {
    return 0;
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeRegister(uint32_t _address, uint32_t _register) {}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeInstruction(uint32_t _instruction) {}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::getMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::sendMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {}

//-------------------------------------------------------------------------------------
