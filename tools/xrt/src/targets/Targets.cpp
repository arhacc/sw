//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "targets/file/FileTarget.h"
#include <cstdint>
#include <targets/Targets.h>

//-------------------------------------------------------------------------------------
Targets::Targets(Arch& _arch, const std::vector<std::string> &_fileTargetPaths,
                 bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget)
    : fpgaTarget(nullptr), simTarget(nullptr), goldenModelTarget(nullptr), arch(_arch) {

    fmt::println("Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}", _enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);

    enableFpgaTarget = _enableFpgaTarget;
    enableSimTarget = _enableSimTarget;
    enableGoldenModelTarget = _enableGoldenModelTarget;

    if (_enableFpgaTarget) {
        fpgaTarget = new FpgaTarget(_arch);
    }
    if (_enableSimTarget) {
        simTarget = new SimTarget();
    }
    if (_enableGoldenModelTarget) {
        goldenModelTarget = new GoldenModelTarget();
    }

    for (auto &_fileTargetPath : _fileTargetPaths) {
        fileTargets.push_back(new FileTarget(_fileTargetPath, _arch));
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
    for (auto _fileTarget : fileTargets) {
        _fileTarget->reset();
    }
}

//-------------------------------------------------------------------------------------
void Targets::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
    if (enableFpgaTarget) {
        fpgaTarget->runRuntime(_address, _argc, _args);
    }
    if (enableSimTarget) {
        simTarget->runRuntime(_address, _argc, _args);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->runRuntime(_address, _argc, _args);
    }
    for (auto _fileTarget : fileTargets) {
        _fileTarget->runRuntime(_address, _argc, _args);
    }
}

//-------------------------------------------------------------------------------------
void Targets::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {
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

    fmt::println("WARNING: Returning default value 0 for register read from Targets");
    return 0;
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
    for (auto _fileTarget : fileTargets) {
        _fileTarget->writeRegister(_address, _register);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    if (enableFpgaTarget) {
        fpgaTarget->writeCode(_address, _code, _length);
    }
    if (enableSimTarget) {
        simTarget->writeCode(_address, _code, _length);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeCode(_address, _code, _length);
    }
    for (auto _fileTarget : fileTargets) {
        _fileTarget->writeCode(_address, _code, _length);
    }
}

//-------------------------------------------------------------------------------------
void Targets::getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) {
    if (enableFpgaTarget) {
        fpgaTarget->getMatrixArray(_accAddress, _rawRamAddress, _numLines, _numColumns, _waitResult);
    }
    if (enableSimTarget) {
        throw std::runtime_error("SimTarget does not support getMatrixArray (TODO)");
    }
    if (enableGoldenModelTarget) {
        throw std::runtime_error("GoldenModelTarget does not support getMatrixArray (TODO)");
    }
    for (auto _fileTarget : fileTargets) {
        throw std::runtime_error("FileTarget does not support getMatrixArray (TODO)");
    }
}

//-------------------------------------------------------------------------------------
void Targets::sendMatrixArray(uint32_t _rawRamAddress, uint32_t _accAddress, uint32_t _numLines, uint32_t _numColumns) {
    if (enableFpgaTarget) {
        fpgaTarget->sendMatrixArray(_rawRamAddress, _accAddress, _numLines, _numColumns);
    }
    if (enableSimTarget) {
        throw std::runtime_error("SimTarget does not support sendMatrixArray (TODO)");
    }
    if (enableGoldenModelTarget) {
        throw std::runtime_error("GoldenModelTarget does not support sendMatrixArray (TODO)");
    }
    for (auto _fileTarget : fileTargets) {
        throw std::runtime_error("FileTarget does not support sendMatrixArray (TODO)");
    }
}

//-------------------------------------------------------------------------------------
void Targets::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    if (enableFpgaTarget) {
        fpgaTarget->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableSimTarget) {
        simTarget->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    for (auto _fileTarget : fileTargets) {
        _fileTarget->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
}

//-------------------------------------------------------------------------------------
void Targets::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    //  printf("Targets.loadCode @%d, length=%d\n", _address, _length);
    if (enableFpgaTarget) {
        fpgaTarget->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableSimTarget) {
        simTarget->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
    for (auto _fileTarget : fileTargets) {
        _fileTarget->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
    }
}

//-------------------------------------------------------------------------------------
void Targets::dump(const std::string &_address) {
    if (enableFpgaTarget) {
        fpgaTarget->dump(_address);
    }
    if (enableSimTarget) {
        simTarget->dump(_address);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->dump(_address);
    }
    for (auto _fileTarget : fileTargets) {
        _fileTarget->dump(_address);
    }
}

//-------------------------------------------------------------------------------------
