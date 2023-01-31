//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <targets/common/Target.h>
//-------------------------------------------------------------------------------------
class GoldenModelTarget : public Target {

public:
    GoldenModelTarget();
    ~GoldenModelTarget();

    void init();
    void quit();

    void  readRegister(uint32_t _address, uint32_t _register);
    void writeRegister(uint32_t _address, uint32_t _register);

    void writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

    void  readData(uint32_t _address, uint32_t* _data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);
    void writeData(uint32_t _address, uint32_t* _data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);


    void reset();
    void runRuntime(uint32_t _address, uint32_t* _args);
    void runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress);

private:
};

//-------------------------------------------------------------------------------------
