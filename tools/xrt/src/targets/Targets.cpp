//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <targets/Targets.hpp>
#include <targets/file/FileTarget.hpp>
#include <targets/fpga/FpgaTarget.hpp>
#include <targets/goldenmodel/GoldenModelTarget.hpp>
#include <targets/sim/SimTarget.hpp>

#include "common/types/Matrix.hpp"

//-------------------------------------------------------------------------------------
Targets::Targets(
    Arch& _arch,
    std::string_view _fileTargetPath,
    bool _enableFpgaTarget,
    bool _enableSimTarget,
    bool _enableGoldenModelTarget,
    bool _enableWdb,
    std::string_view _logSuffix)
    : fpgaTarget(nullptr),
      simTarget(nullptr),
      goldenModelTarget(nullptr),
      fileTarget(nullptr) {
    logInit.print(fmt::format(
        "Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}, FILETARGET: {}\n",
        _enableFpgaTarget,
        _enableSimTarget,
        _enableGoldenModelTarget,
        _fileTargetPath == ""));

    enableFpgaTarget        = _enableFpgaTarget;
    enableSimTarget         = _enableSimTarget;
    enableGoldenModelTarget = _enableGoldenModelTarget;

    if (_enableFpgaTarget) {
        fpgaTarget = new FpgaTarget(_arch);
    }
    if (_enableSimTarget) {
        simTarget = new SimTarget(_arch, _enableWdb, _logSuffix);
    }
    if (_enableGoldenModelTarget) {
        goldenModelTarget = new GoldenModelTarget();
    }

    if (_fileTargetPath != "") {
        fileTarget = new FileTarget(_fileTargetPath, _arch);
    }
}

//-------------------------------------------------------------------------------------
Targets::~Targets() {
    if (fpgaTarget) {
        delete (fpgaTarget);
    }
    if (simTarget) {
        delete (simTarget);
    }
    if (goldenModelTarget) {
        delete (goldenModelTarget);
    }
}

//-------------------------------------------------------------------------------------
void Targets::reset() {
    if (enableFpgaTarget) {
        fpgaTarget->reset();
    }
    if (enableSimTarget) {
        simTarget->reset();
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->reset();
    }
    if (fileTarget) {
        fileTarget->reset();
    }
}

//-------------------------------------------------------------------------------------
uint32_t Targets::readRegister(uint32_t _address) {
    if (enableFpgaTarget) {
        return fpgaTarget->readRegister(_address);
    }
    if (enableSimTarget) {
        return simTarget->readRegister(_address);
    }
    if (enableGoldenModelTarget) {
        return goldenModelTarget->readRegister(_address);
    }
    if (fileTarget) {
        return fileTarget->readRegister(_address);
    }

    logWork.print("Targets::readRegister: no target enabled\n");
    return 0xdeadbeef;
}

//-------------------------------------------------------------------------------------
void Targets::writeRegister(uint32_t _address, uint32_t _register) {
    if (enableFpgaTarget) {
        fpgaTarget->writeRegister(_address, _register);
    }
    if (enableSimTarget) {
        simTarget->writeRegister(_address, _register);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeRegister(_address, _register);
    }
    if (fileTarget) {
        fileTarget->writeRegister(_address, _register);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeInstruction(uint32_t _instruction) {
    if (enableFpgaTarget) {
        fpgaTarget->writeInstruction(_instruction);
    }
    if (enableSimTarget) {
        simTarget->writeInstruction(_instruction);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeInstruction(_instruction);
    }
    if (fileTarget) {
        fileTarget->writeInstruction(_instruction);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeInstructions(std::span<const uint32_t> _instructions) {
    if (enableFpgaTarget) {
        fpgaTarget->writeInstructions(_instructions);
    }
    if (enableSimTarget) {
        simTarget->writeInstructions(_instructions);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeInstructions(_instructions);
    }
    if (fileTarget) {
        fileTarget->writeInstructions(_instructions);
    }
}

//-------------------------------------------------------------------------------------
void Targets::getMatrixArray(MatrixView* _matrixView) {
    if (enableFpgaTarget) {
        fpgaTarget->getMatrixArray(_matrixView);
    } else if (enableSimTarget) {
        simTarget->getMatrixArray(_matrixView);
    } else if (enableGoldenModelTarget) {
        goldenModelTarget->getMatrixArray(_matrixView);
    } else if (fileTarget) {
        fileTarget->getMatrixArray(_matrixView);
    }
}

//-------------------------------------------------------------------------------------
void Targets::sendMatrixArray(const MatrixView* _matrixView) {
    if (enableFpgaTarget) {
        fpgaTarget->sendMatrixArray(_matrixView);
    } else if (enableSimTarget) {
        simTarget->sendMatrixArray(_matrixView);
    } else if (enableGoldenModelTarget) {
        goldenModelTarget->sendMatrixArray(_matrixView);
    } else if (fileTarget) {
        fileTarget->sendMatrixArray(_matrixView);
    }
}

//-------------------------------------------------------------------------------------
