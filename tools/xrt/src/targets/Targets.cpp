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


//-------------------------------------------------------------------------------------
Targets::Targets(
    Arch& _arch,
    std::string_view _fileTargetPath,
    bool _enableFpgaTarget,
    bool _enableSimTarget,
    bool _enableGoldenModelTarget,
    bool _enableWdb,
    bool _haveAcceleratorImageFromLog,
    std::string_view _logSuffix,
    uint32_t _clockPeriodNs)
    : fpgaTarget(nullptr), simTarget(nullptr), goldenModelTarget(nullptr), fileTarget(nullptr) {
    logInit.println<InfoLow>(
        "Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}, FILETARGET: {}",
        _enableFpgaTarget,
        _enableSimTarget,
        _enableGoldenModelTarget,
        _fileTargetPath != "");

    enableFpgaTarget        = _enableFpgaTarget;
    enableSimTarget         = _enableSimTarget;
    enableGoldenModelTarget = _enableGoldenModelTarget;

    if (_enableFpgaTarget) {
        fpgaTarget = new FpgaTarget(_arch);
    }
    if (_enableSimTarget) {
        simTarget = new SimTarget(_arch, _enableWdb, _haveAcceleratorImageFromLog, _logSuffix, _clockPeriodNs);
    }
    if (_enableGoldenModelTarget) {
        // goldenModelTarget = new GoldenModelTarget();
    }

    if (_fileTargetPath != "") {
        // fileTarget = new FileTarget(_fileTargetPath, _arch);
    }
}

//-------------------------------------------------------------------------------------
Targets::~Targets() {
    if (fpgaTarget) {
        delete (fpgaTarget);
    }
    if (simTarget) {
        delete (simTarget);
    }
    if (goldenModelTarget) {
        delete (goldenModelTarget);
    }
    if (fileTarget) {
        delete (fileTarget);
    }
}

//-------------------------------------------------------------------------------------
void Targets::reset() {
    if (enableFpgaTarget) {
        fpgaTarget->reset();
    }
    if (enableSimTarget) {
        simTarget->reset();
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->reset();
    }
    if (fileTarget) {
        fileTarget->reset();
    }
}

std::shared_ptr<Future> Targets::readRegisterAsync(const std::uint32_t address, std::uint32_t* dataLocation) {
    if (enableFpgaTarget) {
        return fpgaTarget->readRegisterAsync(address, dataLocation);
    }
    if (enableSimTarget) {
        return simTarget->readRegisterAsync(address, dataLocation);
    }
    if (enableGoldenModelTarget) {
        return goldenModelTarget->readRegisterAsync(address, dataLocation);
    }
    if (fileTarget) {
        return fileTarget->readRegisterAsync(address, dataLocation);
    }

    return nullptr;
}

std::shared_ptr<Future> Targets::writeRegisterAsync(std::uint32_t address, std::uint32_t data) {
    if (enableFpgaTarget) {
        return fpgaTarget->writeRegisterAsync(address, data);
    }
    if (enableSimTarget) {
        return simTarget->writeRegisterAsync(address, data);
    }
    if (enableGoldenModelTarget) {
        return goldenModelTarget->writeRegisterAsync(address, data);
    }
    if (fileTarget) {
        return fileTarget->writeRegisterAsync(address, data);
    }

    return nullptr;
}

std::shared_ptr<Future> Targets::readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) {
    if (enableFpgaTarget) {
        return fpgaTarget->readMatrixArrayAsync(view);
    }
    if (enableSimTarget) {
        return simTarget->readMatrixArrayAsync(view);
    }
    if (enableGoldenModelTarget) {
        return goldenModelTarget->readMatrixArrayAsync(view);
    }
    if (fileTarget) {
        return fileTarget->readMatrixArrayAsync(view);
    }

    return nullptr;
}

std::shared_ptr<Future> Targets::writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) {
    if (enableFpgaTarget) {
        return fpgaTarget->writeMatrixArrayAsync(view);
    }
    if (enableSimTarget) {
        return simTarget->writeMatrixArrayAsync(view);
    }
    if (enableGoldenModelTarget) {
        return goldenModelTarget->writeMatrixArrayAsync(view);
    }
    if (fileTarget) {
        return fileTarget->writeMatrixArrayAsync(view);
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------
void Targets::runClockCycle() {
    if (enableSimTarget) {
        simTarget->runClockCycle();
    }
}

//-------------------------------------------------------------------------------------
void Targets::runClockCycles(unsigned _n) {
    if (enableSimTarget) {
        simTarget->runClockCycles(_n);
    }
}

//-------------------------------------------------------------------------------------
void Targets::setReportInterrupt(bool _reportInterrupt) {
    if (enableSimTarget) {
        simTarget->setReportInterrupt(_reportInterrupt);
    }
}

void Targets::setInterruptCallback(const std::function<void()>& callback) {
    if (enableSimTarget) {
        simTarget->setInterruptCallback(callback);
    }
}

//-------------------------------------------------------------------------------------
uint64_t Targets::getSimSteps() const {
    if (enableSimTarget) {
        return simTarget->getSimSteps();
    }

    return -1;
}

//-------------------------------------------------------------------------------------
uint64_t Targets::getSimCycles() const {
    if (enableSimTarget) {
        return simTarget->getSimCycles();
    }

    return -1;
}

void Targets::setMaxSimSteps(uint64_t _max) {
	if (enableSimTarget) {
		simTarget->setMaxSimSteps(_max);
	}
}

void Targets::setMaxSimCycles(uint64_t _max) {
	if (enableSimTarget) {
		simTarget->setMaxSimCycles(_max);
	}
}

//-------------------------------------------------------------------------------------
std::shared_ptr<AcceleratorImage> Targets::getAcceleratorImageFromLog() {
  if (enableSimTarget) {
    return simTarget->getAcceleratorImageFromLog();
  }

  return nullptr;
}

//-------------------------------------------------------------------------------------
