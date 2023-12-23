//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/goldenmodel/GoldenModelTarget.hpp>

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
void GoldenModelTarget::getMatrixArray(MatrixView* _matrixView) {}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::sendMatrixArray(const MatrixView* _matrixView) {}

//-------------------------------------------------------------------------------------
