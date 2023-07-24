//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <targets/common/Target.h>
#include <targets/fpga/FpgaTarget.h>
#include <targets/sim/SimTarget.h>
#include <targets/file/FileTarget.h>
#include <targets/goldenmodel/GoldenModelTarget.h>
#include <targets/file/FileTarget.h>
#include <vector>
#include <common/arch/Arch.hpp>

//-------------------------------------------------------------------------------------
class Targets {
    bool enableFpgaTarget;
    bool enableSimTarget;
    bool enableGoldenModelTarget;

    FpgaTarget *fpgaTarget;
    SimTarget *simTarget;
    GoldenModelTarget *goldenModelTarget;

    FileTarget *fileTarget;

public:
    Targets(Arch& _arch, std::string_view _fileTargetPath, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget);

    ~Targets();

    void reset();

    void runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args);

    void runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress);

    uint32_t readRegister(uint32_t _address);

    void writeRegister(uint32_t _address, uint32_t _register);

    void writeCode(uint32_t _address, uint32_t *_code, uint32_t _length);

    void readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop);

    void writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop);
    
    void readMatrixArray(uint32_t _accMemStart,
                         uint32_t *_ramMatrix,
                         uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                         uint32_t _ramStartLine, uint32_t _ramStartColumn,
                         uint32_t _numLines, uint32_t _numColumns,
                         bool     _accRequireResultReady);

    void writeMatrixArray(uint32_t _accMemStart,
                          uint32_t *_ramMatrix,
                          uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                          uint32_t _ramStartLine, uint32_t _ramStartColumn,
                          uint32_t _numLines, uint32_t _numColumns);

    void dump(const std::string &_address);
};

//-------------------------------------------------------------------------------------






