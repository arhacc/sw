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
    
    targets->writeMatrixArray(
        _accMemStart, _ramMatrix,
        _ramTotalLines, _ramTotalColumns,
        _ramStartLine, _ramStartColumn,
        _numLines, _numColumns
    );
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(uint32_t _accMemStart,
                             uint32_t *_ramMatrix,
                             uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn,
                             uint32_t _numLines, uint32_t _numColumns,
                             bool     _accRequireResultReady) {

    targets->readMatrixArray(
        _accMemStart, _ramMatrix,
        _ramTotalLines, _ramTotalColumns,
        _ramStartLine, _ramStartColumn,
        _numLines, _numColumns,
        _accRequireResultReady
    );
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
void Driver::dump(const std::string &_address) {
    targets->dump(_address);
}

//-------------------------------------------------------------------------------------
