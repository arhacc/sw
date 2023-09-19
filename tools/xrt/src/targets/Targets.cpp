//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/Targets.h>

#include <cstdint>

#include "targets/file/FileTarget.h"

//-------------------------------------------------------------------------------------
Targets::Targets(
    Arch& _arch,
    std::string_view _fileTargetPath,
    bool _enableFpgaTarget,
    bool _enableSimTarget,
    bool _enableGoldenModelTarget)
    : fpgaTarget(nullptr),
      simTarget(nullptr),
      goldenModelTarget(nullptr),
      fileTarget(nullptr) {
    fmt::println(
        "Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}, FILETARGET: {}",
        _enableFpgaTarget,
        _enableSimTarget,
        _enableGoldenModelTarget,
        _fileTargetPath == "");

    enableFpgaTarget        = _enableFpgaTarget;
    enableSimTarget         = _enableSimTarget;
    enableGoldenModelTarget = _enableGoldenModelTarget;

    if (_enableFpgaTarget) {
        fpgaTarget = new FpgaTarget(_arch);
    }
    if (_enableSimTarget) {
        simTarget = new SimTarget(_arch);
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

    fmt::println("Targets::readRegister: no target enabled");
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
void Targets::getMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    if (enableFpgaTarget) {
        fpgaTarget->getMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (enableSimTarget) {
        simTarget->getMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (enableGoldenModelTarget) {
        goldenModelTarget->getMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (fileTarget) {
        fileTarget->getMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    }
}

//-------------------------------------------------------------------------------------
void Targets::sendMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    if (enableFpgaTarget) {
        fpgaTarget->sendMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (enableSimTarget) {
        simTarget->sendMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (enableGoldenModelTarget) {
        goldenModelTarget->sendMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } else if (fileTarget) {
        fileTarget->sendMatrixArray(
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    }
}

//-------------------------------------------------------------------------------------
