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
void Targets::runRuntime(uint32_t _address, uint32_t _argc, uint32_t* _args) {
    if (enableFpgaTarget) {
        fpgaTarget->runRuntime(_address, _argc, _args);
    }
    if (enableSimTarget) {
        simTarget->runRuntime(_address, _argc, _args);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->runRuntime(_address, _argc, _args);
    }
    if (fileTarget) {
        fileTarget->runRuntime(_address, _argc, _args);
    }
}

//-------------------------------------------------------------------------------------
void Targets::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress) {
    if (enableFpgaTarget) {
        fpgaTarget->runDebug(_address, _args, _breakpointAddress);
    }
    if (enableSimTarget) {
        simTarget->runDebug(_address, _args, _breakpointAddress);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->runDebug(_address, _args, _breakpointAddress);
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
void Targets::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length) {
    if (enableFpgaTarget) {
        fpgaTarget->writeCode(_address, _code, _length);
    }
    if (enableSimTarget) {
        simTarget->writeCode(_address, _code, _length);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeCode(_address, _code, _length);
    }
    if (fileTarget) {
        fileTarget->writeCode(_address, _code, _length);
    }
}

//-------------------------------------------------------------------------------------
void Targets::readControllerData(
    uint32_t _address,
    uint32_t* _data,
    uint32_t _lineStart,
    uint32_t _lineStop,
    uint32_t _columnStart,
    uint32_t _columnStop) {
    if (enableFpgaTarget) {
        fpgaTarget->readControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableSimTarget) {
        simTarget->readControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->readControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (fileTarget) {
        fileTarget->readControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeControllerData(
    uint32_t _address,
    uint32_t* _data,
    uint32_t _lineStart,
    uint32_t _lineStop,
    uint32_t _columnStart,
    uint32_t _columnStop) {
    if (enableFpgaTarget) {
        fpgaTarget->writeControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableSimTarget) {
        simTarget->writeControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (fileTarget) {
        fileTarget->writeControllerData(
            _address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
}

//-------------------------------------------------------------------------------------
void Targets::readMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady) {
    if (enableFpgaTarget) {
        fpgaTarget->readMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns,
            _accRequireResultReady);
    }
    if (enableSimTarget) {
        simTarget->readMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns,
            _accRequireResultReady);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->readMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns,
            _accRequireResultReady);
    }
    if (fileTarget) {
        fileTarget->readMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns,
            _accRequireResultReady);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    if (enableFpgaTarget) {
        fpgaTarget->writeMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    }
    if (enableSimTarget) {
        simTarget->writeMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    }
    if (fileTarget) {
        fileTarget->writeMatrixArray(
            _accMemStart,
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
void Targets::dump(const std::string& _address) {
    if (enableFpgaTarget) {
        fpgaTarget->dump(_address);
    }
    if (enableSimTarget) {
        simTarget->dump(_address);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->dump(_address);
    }
    if (fileTarget) {
        fileTarget->dump(_address);
    }
}

//-------------------------------------------------------------------------------------
