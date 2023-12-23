//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Target.hpp>

#include <cstdint>
#include <fstream>
#include <optional>

#include <fmt/os.h>
#include <sys/types.h>

// forward declarations
struct Arch;

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

    void getMatrixArray(MatrixView* _matrixView) override;

    void sendMatrixArray(const MatrixView* _matrixView) override;
};

//-------------------------------------------------------------------------------------
