//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <targets/common/Target.h>
#include <targets/file/FileTarget.h>
#include <targets/fpga/FpgaTarget.h>
#include <targets/goldenmodel/GoldenModelTarget.h>
#include <targets/sim/SimTarget.h>

#include <cstdint>
#include <vector>

//-------------------------------------------------------------------------------------
class Targets {
    bool enableFpgaTarget;
    bool enableSimTarget;
    bool enableGoldenModelTarget;

    FpgaTarget* fpgaTarget;
    SimTarget* simTarget;
    GoldenModelTarget* goldenModelTarget;

    FileTarget* fileTarget;

  public:
    Targets(
        Arch& _arch,
        std::string_view _fileTargetPath,
        bool _enableFpgaTarget,
        bool _enableSimTarget,
        bool _enableGoldenModelTarget);

    ~Targets();

    void reset();

    uint32_t readRegister(uint32_t _address);

    void writeRegister(uint32_t _address, uint32_t _register);

    void writeInstruction(uint32_t _instruction);
    void writeInstructions(std::span<const uint32_t> _instructions);

    void getMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns);

    void sendMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns);
};

//-------------------------------------------------------------------------------------
