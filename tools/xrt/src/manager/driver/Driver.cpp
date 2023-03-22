//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <string>
#include "manager/driver/Driver.h"

//-------------------------------------------------------------------------------------
Driver::Driver(Targets *_targets) {
    targets = _targets;
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
