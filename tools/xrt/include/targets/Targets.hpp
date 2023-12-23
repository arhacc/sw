//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

// forward declarations
class MatrixView;
class FpgaTarget;
class SimTarget;
class GoldenModelTarget;
class FileTarget;
struct Arch;

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

    void getMatrixArray(MatrixView* _matrixView);

    void sendMatrixArray(const MatrixView* _matrixView);
};

//-------------------------------------------------------------------------------------
