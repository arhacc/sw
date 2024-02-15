//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <targets/common/Target.hpp>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <memory>
#include <queue>
#include <vector>

#include <fmt/format.h>

// forward declarations
class AXILiteSimStream;
class AXIStreamReadSimStream;
class AXIStreamWriteSimStream;
class RegisterFuture;
class Future;
class MatrixViewReadFuture;
class MatrixViewWriteFuture;
class Tb;
struct Arch;
class SimStreams;

class SimInterrupt : std::exception {
    inline const char* what() const noexcept override {
        return "Interrupt";
    };
};

//-------------------------------------------------------------------------------------
class SimTarget : public Target {
    const Arch& arch;

    Tb* tb;
    SimStreams* simStreams;

    static const std::filesystem::path cDesignDirPath;

    bool reportInterrupt = true;

  public:
    SimTarget(const Arch& _arch, bool enableWdb, std::string_view _logSuffix);
    ~SimTarget() override;

    void reset() override;

    void process(std::shared_ptr<Future> _future) override;

    void runClockCycle();
    void runClockCycles(unsigned);

    void setReportInterrupt(bool);
};

//-------------------------------------------------------------------------------------
