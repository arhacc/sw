//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <targets/common/Target.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "Parameters.hpp"
#include "Simulation.hpp"
#include <fmt/format.h>

//-------------------------------------------------------------------------------------
class SimTarget : public Target {
    // XpuTestBench* xpuTestBench;

    const Arch& arch;

    // std::unique_ptr<Simulator> lastSimulator;

    std::vector<uint32_t> programFile;
    std::vector<uint32_t> dataFile;

    uint32_t skipGetMatrix = 0;

    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);

  public:
    void writeInstruction(uint32_t _instruction) override;

    SimTarget(const Arch& _arch);

    ~SimTarget() override = default;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;

    void writeRegister(uint32_t _address, uint32_t _register) override;

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
