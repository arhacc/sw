//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <targets/Targets.hpp>
#include <targets/file/FileTarget.hpp>
#include <targets/fpga/FpgaTarget.hpp>
#include <targets/goldenmodel/GoldenModelTarget.hpp>
#include <targets/sim/SimTarget.hpp>

Targets::Targets(
    Arch& arch,
    const std::string_view fileTargetPath,
    const bool enableFpgaTarget,
    const bool enableSimTarget,
    [[maybe_unused]] const bool enableGoldenModelTarget,
    const bool enableWdb,
    const bool haveAcceleratorImageFromLog,
    const std::string_view logSuffix,
    const std::uint32_t clockPeriodNs)
    : fpgaTarget_(enableFpgaTarget ? std::make_unique<FpgaTarget>(arch) : nullptr),
      simTarget_(
          enableSimTarget
              ? std::make_unique<SimTarget>(arch, enableWdb, haveAcceleratorImageFromLog, logSuffix, clockPeriodNs)
              : nullptr),
      goldenModelTarget_(nullptr),
      fileTarget_(nullptr) {
    logInit.println<InfoLow>(
        "Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}, FILETARGET: {}",
        enableFpgaTarget,
        enableSimTarget,
        enableGoldenModelTarget,
        fileTargetPath != "");
}

Targets::~Targets() = default;


void Targets::reset() const {
    if (fpgaTarget_) {
        fpgaTarget_->reset();
    }
    if (simTarget_) {
        simTarget_->reset();
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->reset();
    }
    if (fileTarget_) {
        fileTarget_->reset();
    }
}

std::uint32_t Targets::readRegister(const std::uint32_t address) const {
    if (fpgaTarget_) {
        return fpgaTarget_->readRegister(address);
    }
    if (simTarget_) {
        return simTarget_->readRegister(address);
    }
    if (goldenModelTarget_) {
        return goldenModelTarget_->readRegister(address);
    }
    if (fileTarget_) {
        return fileTarget_->readRegister(address);
    }

    return std::numeric_limits<std::uint32_t>::max();
}

void Targets::writeRegister(const std::uint32_t address, const std::uint32_t data) const {
    if (fpgaTarget_) {
        fpgaTarget_->writeRegister(address, data);
    }
    if (simTarget_) {
        simTarget_->writeRegister(address, data);
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->writeRegister(address, data);
    }
    if (fileTarget_) {
        fileTarget_->writeRegister(address, data);
    }
}

void Targets::readMatrixBefore(MatrixView& view) const {
    if (fpgaTarget_) {
        fpgaTarget_->readMatrixBefore(view);
    }
    if (simTarget_) {
        simTarget_->readMatrixBefore(view);
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->readMatrixBefore(view);
    }
    if (fileTarget_) {
        fileTarget_->readMatrixBefore(view);
    }
}

void Targets::readMatrixAfter(MatrixView& view) const {
    if (fpgaTarget_) {
        fpgaTarget_->readMatrixAfter(view);
    }
    if (simTarget_) {
        simTarget_->readMatrixAfter(view);
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->readMatrixAfter(view);
    }
    if (fileTarget_) {
        fileTarget_->readMatrixAfter(view);
    }
}

void Targets::writeMatrixBefore(const MatrixView& view) const {
    if (fpgaTarget_) {
        fpgaTarget_->writeMatrixBefore(view);
    }
    if (simTarget_) {
        simTarget_->writeMatrixBefore(view);
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->writeMatrixBefore(view);
    }
    if (fileTarget_) {
        fileTarget_->writeMatrixBefore(view);
    }
}

void Targets::writeMatrixAfter(const MatrixView& view) const {
    if (fpgaTarget_) {
        fpgaTarget_->writeMatrixAfter(view);
    }
    if (simTarget_) {
        simTarget_->writeMatrixAfter(view);
    }
    if (goldenModelTarget_) {
        goldenModelTarget_->writeMatrixAfter(view);
    }
    if (fileTarget_) {
        fileTarget_->writeMatrixAfter(view);
    }
}

void Targets::runClockCycle() const {
    if (simTarget_) {
        simTarget_->runClockCycle();
    }
}

void Targets::runClockCycles(const unsigned cycles) const {
    if (simTarget_) {
        simTarget_->runClockCycles(cycles);
    }
}

void Targets::setReportInterrupt(const bool reportInterrupt) const {
    if (simTarget_) {
        simTarget_->setReportInterrupt(reportInterrupt);
    }
}

void Targets::setInterruptCallback(const std::function<void()>& callback) const {
    if (simTarget_) {
        simTarget_->setInterruptCallback(callback);
    }
}

std::uint64_t Targets::getSimSteps() const {
    if (simTarget_) {
        return simTarget_->getSimSteps();
    }

    return std::numeric_limits<std::uint64_t>::max();
}

std::uint64_t Targets::getSimCycles() const {
    if (simTarget_) {
        return simTarget_->getSimCycles();
    }

    return std::numeric_limits<std::uint64_t>::max();
}

void Targets::setMaxSimSteps(const std::uint64_t steps) const {
    if (simTarget_) {
        simTarget_->setMaxSimSteps(steps);
    }
}

void Targets::setMaxSimCycles(const std::uint64_t cycles) const {
    if (simTarget_) {
        simTarget_->setMaxSimCycles(cycles);
    }
}

std::shared_ptr<AcceleratorImage> Targets::getAcceleratorImageFromLog() const {
    if (simTarget_) {
        return simTarget_->getAcceleratorImageFromLog();
    }

    return nullptr;
}
