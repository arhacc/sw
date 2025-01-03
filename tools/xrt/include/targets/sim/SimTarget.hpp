//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <targets/common/Target.hpp>
#include <common/debug/Debug.hpp>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <memory>
#include <thread>

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

    bool reportInterrupt    = true;
    bool lastClockInterrupt = false;
    bool haveAcceleratorImageFromLog;

    FILE *logFile = NULL;
    std::unique_ptr<AcceleratorImage> acceleratorImageFromLog;
    std::mutex acceleratorImageFromLogMutex;

    std::unique_ptr<std::thread> processAcceleratorImageFromLogThread;
    void processAcceleratorImageFromLog();

  public:
    SimTarget(const Arch& _arch, bool enableWdb, bool haveAcceleratorImageFromLog, std::string_view _logSuffix, uint32_t _clockPeriodNs);
    ~SimTarget() override;

    void reset() override;

    void process(std::shared_ptr<Future> _future) override;

    void runClockCycle();
    void runClockCycles(unsigned);

    void setReportInterrupt(bool);

    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog();

    uint64_t getSimSteps() const;
    uint64_t getSimCycles() const;
    void setMaxSimSteps(uint64_t);
    void setMaxSimCycles(uint64_t);
};

//-------------------------------------------------------------------------------------
