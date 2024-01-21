//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <targets/common/Target.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

#include "common/types/Matrix.hpp"
#include <fmt/format.h>

// forward declarations
class Tb;
struct Arch;

//-------------------------------------------------------------------------------------
class SimTarget : public Target {
    const Arch& arch;

    Tb* tb;

    static const std::filesystem::path cDesignDirPath;

  public:
    SimTarget(const Arch& _arch, bool enableWdb, std::string_view _logSuffix);
    ~SimTarget() override;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;
    void writeRegister(uint32_t _address, uint32_t _register) override;

    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);
    void writeInstruction(uint32_t _instruction) override;

    void getMatrixArray(MatrixView* _matrixView) override;

    void sendMatrixArray(const MatrixView* _matrixView) override;
};

//-------------------------------------------------------------------------------------
