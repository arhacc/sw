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

    void process(Future* _future) override;
};

//-------------------------------------------------------------------------------------
