//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <targets/common/Target.h>

#include <cstdint>
#include <fstream>
#include <optional>

#include <fmt/os.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
class FileTarget : public Target {
    const Arch& arch;

    fmt::ostream controllerFile;
    fmt::ostream dataFile;

    std::optional<uint32_t> waitingInstruction;

    inline void dumpWaitingInstruction();

  public:
    FileTarget(std::string_view _path, const Arch& _arch);

    ~FileTarget() override = default;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;

    void writeRegister(uint32_t _address, uint32_t _register) override;

    void writeInstruction(uint32_t _instruction) override;

    void getMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;

    void sendMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;
};

//-------------------------------------------------------------------------------------
