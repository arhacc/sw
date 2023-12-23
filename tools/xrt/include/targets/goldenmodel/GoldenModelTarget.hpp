//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Target.hpp>

#include <cstdint>

class MatrixView;

//-------------------------------------------------------------------------------------
class GoldenModelTarget : public Target {
  public:
    GoldenModelTarget() = default;

    ~GoldenModelTarget() override = default;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;

    void writeRegister(uint32_t _address, uint32_t _register) override;

    void writeInstruction(uint32_t _instruction) override;

    void getMatrixArray(MatrixView* _matrixView) override;

    void sendMatrixArray(const MatrixView* _matrixView) override;
};

//-------------------------------------------------------------------------------------
