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

#include <common/arch/Arch.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <span>
#include <sstream>
#include <string>
#include <unordered_map>

// forward declarations
class Targets;
class MatrixView;

//-------------------------------------------------------------------------------------
class Driver {
    Targets* targets;
    const Arch& arch;

    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);

  public:
    Driver(Targets* _targets, const Arch& _arch);

    ~Driver() = default;

    void reset();

    void run(uint32_t _address, std::span<const uint32_t> _args);

    uint32_t readRegister(uint32_t _address);

    void writeRegister(uint32_t _address, uint32_t _register);

    void writeCode(uint32_t _address, std::span<const uint32_t> _code);

    void writeInstruction(uint32_t _instruction);
    void writeInstructions(std::span<const uint32_t> _instructions);

    void writeTransferInstruction(uint32_t _instruction);

    void readMatrixArray(
        uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady);

    void writeMatrixArray(uint32_t _accMemStart, const MatrixView* _matrixView);
};
//-------------------------------------------------------------------------------------
