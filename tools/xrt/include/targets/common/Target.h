//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <cstdio>
#include <cstdint>
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

    virtual void runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) = 0;

    virtual void runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) = 0;

    virtual uint32_t readRegister(uint32_t _address) = 0;

    virtual void writeRegister(uint32_t _address, uint32_t _register) = 0;

    virtual void writeCode(uint32_t _address, uint32_t *_code, uint32_t _length);

    virtual void readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) = 0;

    virtual void writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) = 0;

    virtual void getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) = 0;

    virtual void sendMatrixArray(uint32_t _rawRamAddress, uint32_t _accAddress, uint32_t _numLines, uint32_t _numColumns) = 0;

    virtual void dump(const std::string &_address) = 0;
};

//-------------------------------------------------------------------------------------






