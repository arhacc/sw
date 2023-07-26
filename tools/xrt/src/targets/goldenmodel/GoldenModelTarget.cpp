//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <cstdio>
#include <targets/goldenmodel/GoldenModelTarget.h>

//-------------------------------------------------------------------------------------
void GoldenModelTarget::reset() {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
uint32_t GoldenModelTarget::readRegister(uint32_t _address) {
        return 0;
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("Target.loadCode @%d, length=%d\n", _address, _length);
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::readMatrixArray(uint32_t _accMemStart,
                        uint32_t *_ramMatrix,
                        uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                        uint32_t _ramStartLine, uint32_t _ramStartColumn,
                        uint32_t _numLines, uint32_t _numColumns,
                        bool     _accRequireResultReady) {

}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeMatrixArray(uint32_t _accMemStart,
                          uint32_t *_ramMatrix,
                          uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                          uint32_t _ramStartLine, uint32_t _ramStartColumn,
                          uint32_t _numLines, uint32_t _numColumns) {}

//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
void GoldenModelTarget::dump(const std::string &_address) {

}

//-------------------------------------------------------------------------------------
