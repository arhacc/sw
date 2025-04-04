//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <common/debug/Debug.hpp>
#include <targets/common/Target.hpp>
#include <targets/sim/SimStreams.hpp>
#include <targets/sim/Tb.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <thread>

// forward declarations
struct Arch;
class SimStreams;

class SimTarget final : public Target {
    static const std::filesystem::path cDesignDirPath_;

    const Arch& arch_;

    // the orders of this is the order of initialization - which must be this

    std::filesystem::path designLibPath_;

    bool haveAcceleratorImageFromLog_;
    std::unique_ptr<AcceleratorImage> acceleratorImageFromLog_{std::make_unique<AcceleratorImage>()};
    std::unique_ptr<std::thread> processAcceleratorImageFromLogThread_;
    Tb tb_;
    SimStreams simStreams_;

    bool reportInterrupt_{true};
    std::function<void()> interruptCallback_;

    bool lastClockInterrupt_{false};

    FILE* logFile_ = nullptr;
    std::mutex acceleratorImageFromLogMutex_;

    static std::uint32_t computeWstrb(const Arch& arch);
    void processAcceleratorImageFromLog();

  public:
    SimTarget(
        const Arch& arch,
        bool enableWdb,
        bool haveAcceleratorImageFromLog,
        std::string_view logSuffix,
        std::uint32_t clockPeriodNs);
    ~SimTarget() override;

    void reset() override;

    void runClockCycle();
    void runClockCycles(unsigned cycles);

    void setReportInterrupt(bool reportInterrupt);
    void setInterruptCallback(const std::function<void()>& callback);

    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog();

    std::uint64_t getSimSteps() const;
    std::uint64_t getSimCycles() const;
    void setMaxSimSteps(std::uint64_t cycles);
    void setMaxSimCycles(std::uint64_t cycles);

    std::uint32_t readRegister(std::uint32_t address) override;
    void writeRegister(std::uint32_t address, std::uint32_t data) override;
    void readMatrixBefore(MatrixView& view) override;
    void readMatrixAfter(MatrixView& view) override;
    void writeMatrixBefore(const MatrixView& view) override;
    void writeMatrixAfter(const MatrixView& view) override;
};
