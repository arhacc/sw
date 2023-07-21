//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "targets/Targets.h"

//-------------------------------------------------------------------------------------
class Driver {
    Targets *targets;

    volatile uint32_t *io_matrix = nullptr;
    uint32_t  io_matrix_raw_position = 0;
    size_t    io_matrix_max_size = 0;

    int memory_file_descriptor;
public:
    Driver(Targets *_targets);

    ~Driver() = default;

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

    void dump(const std::string &_address);

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
                          uint32_t _numLines, uint32_t _numColumns);};
//-------------------------------------------------------------------------------------
