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

    virtual void getMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) = 0;

    virtual void sendMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) = 0;

    virtual void writeInstruction(uint32_t _instruction) = 0;
    virtual void writeInstructions(std::span<const uint32_t> _instructions);

    virtual uint32_t readRegister(uint32_t _address) = 0;

    virtual void writeRegister(uint32_t _address, uint32_t _register) = 0;
};

//-------------------------------------------------------------------------------------
