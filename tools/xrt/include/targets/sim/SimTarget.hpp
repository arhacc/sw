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


//-------------------------------------------------------------------------------------
class SimTarget : public Target {
    const Arch& arch;

    Tb* tb;
    SimStreams* simStreams;

    static const std::filesystem::path cDesignDirPath;

    bool reportInterrupt    = true;
    std::function<void()> interruptCallback;

    bool lastClockInterrupt = false;
    bool haveAcceleratorImageFromLog;

    FILE *logFile = nullptr;
    std::unique_ptr<AcceleratorImage> acceleratorImageFromLog;
    std::mutex acceleratorImageFromLogMutex;

    std::unique_ptr<std::thread> processAcceleratorImageFromLogThread;
    void processAcceleratorImageFromLog();

  public:
    SimTarget(const Arch& _arch, bool enableWdb, bool haveAcceleratorImageFromLog, std::string_view _logSuffix, uint32_t _clockPeriodNs);
    ~SimTarget() override;

    void reset() override;

    std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation) override;
    std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data) override;
    std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) override;
    std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) override;

    void runClockCycle();
    void runClockCycles(unsigned);

    void setReportInterrupt(bool);
    void setInterruptCallback(const std::function<void()>& callback);

    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog();

    std::uint64_t getSimSteps() const;
    std::uint64_t getSimCycles() const;
    void setMaxSimSteps(std::uint64_t);
    void setMaxSimCycles(std::uint64_t);
};

//-------------------------------------------------------------------------------------
