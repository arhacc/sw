//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Target.h>
#include <fstream>


//-------------------------------------------------------------------------------------
class FileTarget : public Target {
    std::ofstream out;

    inline void writeInstruction(uint32_t _instruction);
    inline void writeInstruction(uint16_t _instruction, uint16_t _argument);
public:
    FileTarget(std::string _path);

    ~FileTarget() override = default;

    void reset() override;

    void runRuntime(uint32_t _address, uint32_t *_args) override;

    void runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) override;

    void readRegister(uint32_t _address, uint32_t _register) override;

    void writeRegister(uint32_t _address, uint32_t _register) override;

    void writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) override;

    void readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) override;

    void writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) override;

    void
    readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop) override;

    void
    writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop) override;

    void dump(const std::string & _address) override;

};

//-------------------------------------------------------------------------------------
