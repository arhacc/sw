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
#include <string>
#include "manager/driver/Driver.h"

uint32_t example_matrix_in[] = { 1,  2,  3,  4,  5,
                                 6,  7,  8,  9, 10,
                                 11, 12, 13, 14, 15,
                                 16, 17, 18, 19, 20,
                                 21, 22, 23, 24, 25};

uint32_t example_matrix_out[25] = {0};

//-------------------------------------------------------------------------------------
Driver::Driver(Targets *_targets) {
    targets = _targets;

    io_matrix = new uint32_t[25];

    writeMatrixArray(example_matrix_in, 5, 5, 1, 1, 3, 3, 0, 1, 1);


    std::memset(io_matrix, '\0', 25 * sizeof(uint32_t));

    readMatrixArray(0, 1, 1, true, example_matrix_out, 5, 5, 1, 1, 3, 3);


    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", example_matrix_in[i * 5 + j]);
        }

        printf("\n");
    }
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t *_ramMatrix,
                              uint32_t _ramLineSize, uint32_t _ramColumnSize,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _ramNumLine, uint32_t _ramNumColumn,
                              uint32_t _accMemStart,
                              uint32_t _accNumLine, uint32_t _accNumColumn) {

    uint32_t io_matrix_i = 0;

    for (uint32_t i = _ramStartLine; i < _ramStartLine + _ramNumLine; i++ ) {
        for (uint32_t j = _ramStartColumn; j < _ramStartColumn + _ramNumColumn; j++) {
            io_matrix[io_matrix_i++] = _ramMatrix[i * _ramColumnSize + j];
        }
    }

    writeArrayData(_accMemStart, io_matrix, 0, _accNumLine, 0, _accNumColumn);
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(uint32_t _accMemStart,
                             uint32_t _accNumLine, uint32_t _accNumColumn,
                             bool     _accRequireResultReady,
                             uint32_t *_ramMatrix,
                             uint32_t _ramLineSize, uint32_t _ramColumnSize,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn,
                             uint32_t _ramNumLine, uint32_t _ramNumColumn) {

}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();
}

//-------------------------------------------------------------------------------------
void Driver::runRuntime(uint32_t _address, uint32_t *_args) {
    targets->runRuntime(_address, _args);
}

//-------------------------------------------------------------------------------------
void Driver::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {
    targets->runDebug(_address, _args, _breakpointAddress);
}

//-------------------------------------------------------------------------------------
void Driver::readRegister(uint32_t _address, uint32_t _register) {
    targets->readRegister(_address, _register);
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
    //  printf("Driver.loadCode @%d, length=%d\n", _address, _length);
    targets->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Driver::readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    targets->readArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Driver::writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    //  printf("Driver.loadCode @%d, length=%d\n", _address, _length);
    targets->writeArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Driver::dump(const std::string &_address) {
    targets->dump(_address);
}

//-------------------------------------------------------------------------------------
