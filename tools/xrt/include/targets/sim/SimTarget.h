//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <fmt/format.h>

#include <common/arch/Arch.hpp>
#include <targets/common/Target.h>
#include "Tb.h"

//-------------------------------------------------------------------------------------
class SimTarget : public Target {
    const Arch& arch;

    Tb* tb;


  public:
    SimTarget(const Arch& _arch);
    ~SimTarget() override;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;
    void writeRegister(uint32_t _address, uint32_t _register) override;

    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);
    void writeInstruction(uint32_t _instruction) override;

    void getMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;

    void sendMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;
};

//-------------------------------------------------------------------------------------
