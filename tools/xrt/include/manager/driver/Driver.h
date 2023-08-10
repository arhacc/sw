//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/Targets.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

//-------------------------------------------------------------------------------------
class Driver {
    Targets* targets;
    const Arch& arch;

    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);

  public:
    Driver(Targets* _targets, const Arch& _arch);

    ~Driver() = default;

    void reset();

    void runRuntime(uint32_t _address, uint32_t _argc, uint32_t* _args);

    uint32_t readRegister(uint32_t _address);

    void writeRegister(uint32_t _address, uint32_t _register);

    void writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

    void writeInstruction(uint32_t _instruction);
    void writeInstructions(std::span<const uint32_t> _instructions);

    void readMatrixArray(
        uint32_t _accMemStart,
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns,
        bool _accRequireResultReady);

    void writeMatrixArray(
        uint32_t _accMemStart,
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns);
};
//-------------------------------------------------------------------------------------
