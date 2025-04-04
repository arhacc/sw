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
class Manager;
struct Arch;

class Targets {
    std::unique_ptr<FpgaTarget> fpgaTarget_;
    std::unique_ptr<SimTarget> simTarget_;
    std::unique_ptr<GoldenModelTarget> goldenModelTarget_;
    std::unique_ptr<FileTarget> fileTarget_;

  public:
    Targets(
        Arch& arch,
        std::string_view fileTargetPath,
        bool enableFpgaTarget,
        bool enableSimTarget,
        bool enableGoldenModelTarget,
        bool enableWdb,
        bool haveAcceleratorImageFromLog,
        std::string_view logSuffix,
        std::uint32_t clockPeriodNs);

    ~Targets();

    void reset() const;

    std::uint32_t readRegister(std::uint32_t address) const;
    void writeRegister(std::uint32_t address, std::uint32_t data) const;
    void readMatrixBefore(MatrixView& view) const;
    void readMatrixAfter(MatrixView& view) const;
    void writeMatrixBefore(const MatrixView& view) const;
    void writeMatrixAfter(const MatrixView& view) const;

    [[nodiscard]] std::uint64_t getSimSteps() const;
    [[nodiscard]] std::uint64_t getSimCycles() const;
    void setMaxSimSteps(std::uint64_t steps) const;
    void setMaxSimCycles(std::uint64_t cycles) const;

    void runClockCycle() const;
    void runClockCycles(unsigned cycles) const;
    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog() const;
    void setReportInterrupt(bool reportInterrupt) const;
    void setInterruptCallback(const std::function<void()>& callback) const;
};
