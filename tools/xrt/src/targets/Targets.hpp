//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/debug/Debug.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

// forward declarations
class MatrixView;
class FpgaTarget;
class SimTarget;
class GoldenModelTarget;
class FileTarget;
class Future;
class Manager;
struct Arch;

//-------------------------------------------------------------------------------------
class Targets {
    bool enableFpgaTarget;
    bool enableSimTarget;
    bool enableGoldenModelTarget;

    FpgaTarget* fpgaTarget;
    SimTarget* simTarget;
    GoldenModelTarget* goldenModelTarget;

    FileTarget* fileTarget;

  public:
    Targets(
        Arch& _arch,
        std::string_view _fileTargetPath,
        bool _enableFpgaTarget,
        bool _enableSimTarget,
        bool _enableGoldenModelTarget,
        bool _enableWdb,
        bool _haveAcceleratorImageFromLog,
        std::string_view _logSuffix,
        uint32_t _clockPeriodNs);

    ~Targets();

    void reset();

    std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation);
    std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data);
    std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view);
    std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view);

    uint64_t getSimSteps() const;
    uint64_t getSimCycles() const;
    void setMaxSimSteps(uint64_t);
    void setMaxSimCycles(uint64_t);

    void runClockCycle();
    void runClockCycles(unsigned);
    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog();
    void setReportInterrupt(bool _reportInterrupt);
    void setInterruptCallback(const std::function<void()>& callback);
};

//-------------------------------------------------------------------------------------
