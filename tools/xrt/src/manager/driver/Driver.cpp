//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include "manager/driver/Driver.h"

//-------------------------------------------------------------------------------------
Driver::Driver(Targets *_targets)
    : targets(_targets) {

    // TODO: this is temporary code for setting IO matrix for FPGA. A more abstract way is needed
    memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);

    if (memory_file_descriptor == -1) {
        fmt::println("Error opening /dev/mem, target:fpga requires root.");
        std::exit(1);
    }
    
    io_matrix_max_size = 16 * 1024 * sizeof(uint32_t);

    io_matrix_raw_position = 0x19000000;

    io_matrix = (uint32_t *) mmap(nullptr, io_matrix_max_size, PROT_READ | PROT_WRITE, MAP_SHARED,
            memory_file_descriptor, io_matrix_raw_position);
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t _accMemStart,
                              uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLines, uint32_t _numColumns) {
    
    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);

    // TODO: test performance of liniarization vs sending each part individually on FIFO
    if (_numLines * _numColumns * sizeof(uint32_t) > io_matrix_max_size) {
        throw std::runtime_error("Matrix too large");
    }

    uint32_t io_matrix_i = 0;

    for (uint32_t i = _ramStartLine; i < _ramStartLine + _numLines; i++ ) {
        for (uint32_t j = _ramStartColumn; j < _ramStartColumn + _numColumns; j++) {
            io_matrix[io_matrix_i++] = _ramMatrix[i * _ramTotalColumns + j];
        }
    }

    sendMatrixArray(io_matrix_raw_position, _accMemStart, _numLines, _numColumns);

#ifndef NDEBUG
    for (uint32_t i = 0; i < io_matrix_i; i++) {
        io_matrix[i] = 0;
    }
#endif
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(uint32_t _accMemStart,
                             uint32_t *_ramMatrix,
                             uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn,
                             uint32_t _numLines, uint32_t _numColumns,
                             bool     _accRequireResultReady) {

    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);

    if (_numLines * _numColumns * sizeof(uint32_t) > io_matrix_max_size) {
        throw std::runtime_error("Matrix too large");
    }


    getMatrixArray(_accMemStart, io_matrix_raw_position, _numLines, _numColumns, _accRequireResultReady);

    uint32_t io_matrix_i = 0;

    for (uint32_t i = _ramStartLine; i < _ramStartLine + _numLines; i++ ) {
        for (uint32_t j = _ramStartColumn; j < _ramStartColumn + _numColumns; j++) {
            _ramMatrix[i * _ramTotalColumns + j] = io_matrix[io_matrix_i++];
        }
    }
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();
}

//-------------------------------------------------------------------------------------
void Driver::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
    targets->runRuntime(_address, _argc, _args);
}

//-------------------------------------------------------------------------------------
void Driver::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {
    targets->runDebug(_address, _args, _breakpointAddress);
}

//-------------------------------------------------------------------------------------
uint32_t Driver::readRegister(uint32_t _address) {
    return targets->readRegister(_address);
}

//-------------------------------------------------------------------------------------
void Driver::writeRegister(uint32_t _address, uint32_t _register) {
    targets->writeRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Driver::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    targets->writeCode(_address, _code, _length);
}

//-------------------------------------------------------------------------------------
void Driver::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    targets->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Driver::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    targets->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Driver::getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) {
    targets->getMatrixArray(_accAddress, _rawRamAddress, _numLines, _numColumns, _waitResult);
}

//-------------------------------------------------------------------------------------
void Driver::sendMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns) {
    targets->sendMatrixArray(_accAddress, _rawRamAddress, _numLines, _numColumns);
}

//-------------------------------------------------------------------------------------
void Driver::dump(const std::string &_address) {
    targets->dump(_address);
}

//-------------------------------------------------------------------------------------
