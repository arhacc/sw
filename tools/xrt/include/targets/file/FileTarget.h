//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <sys/types.h>
#include <targets/common/Target.h>
#include <fstream>
#include <common/arch/Arch.hpp>


//-------------------------------------------------------------------------------------
class FileTarget : public Target {
    const Arch& arch;

    std::ofstream out;

    bool ctrl_col = true;

    void writeInstruction(uint32_t _instruction);
    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);
public:
    FileTarget(const std::string& _path, const Arch& _arch);

    ~FileTarget() override = default;

    void reset() override;

    void runRuntime(uint32_t _address, uint32_t *_args) override;

    void runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args);

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
