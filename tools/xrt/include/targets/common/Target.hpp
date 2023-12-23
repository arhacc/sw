//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <span>

class MatrixView;

//-------------------------------------------------------------------------------------
/* array memory is viewd as a matrix where
each line has "Ncores" length and each column has memorySize length
*/
//-------------------------------------------------------------------------------------
class Target {
  public:
    Target() = default;

    virtual ~Target() = default;

    virtual void reset() = 0;

    virtual uint32_t readRegister(uint32_t _address)                  = 0;
    virtual void writeRegister(uint32_t _address, uint32_t _register) = 0;

    virtual void writeInstruction(uint32_t _instruction) = 0;
    virtual void writeInstructions(std::span<const uint32_t> _instructions);

    virtual void getMatrixArray(MatrixView* _matrixView) = 0;

    virtual void sendMatrixArray(const MatrixView* _matrixView) = 0;
};

//-------------------------------------------------------------------------------------
