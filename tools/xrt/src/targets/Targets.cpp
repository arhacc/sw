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

#include <string>
#include <string_view>

//-------------------------------------------------------------------------------------
Targets::Targets(
    Arch& _arch,
    std::string_view _fileTargetPath,
    bool _enableFpgaTarget,
    bool _enableSimTarget,
    bool _enableGoldenModelTarget,
    bool _enableWdb,
    std::string_view _logSuffix)
    : fpgaTarget(nullptr), simTarget(nullptr), goldenModelTarget(nullptr), fileTarget(nullptr) {
    logInit.print(fmt::format(
        "Targets: FPGA: {}, SIM: {}, GOLDENMODEL: {}, FILETARGET: {}\n",
        _enableFpgaTarget,
        _enableSimTarget,
        _enableGoldenModelTarget,
        _fileTargetPath != ""));

    enableFpgaTarget        = _enableFpgaTarget;
    enableSimTarget         = _enableSimTarget;
    enableGoldenModelTarget = _enableGoldenModelTarget;

    if (_enableFpgaTarget) {
        fpgaTarget = new FpgaTarget(_arch);
    }
    if (_enableSimTarget) {
        simTarget = new SimTarget(_arch, _enableWdb, _logSuffix);
    }
    if (_enableGoldenModelTarget) {
        goldenModelTarget = new GoldenModelTarget();
    }

    if (_fileTargetPath != "") {
        fileTarget = new FileTarget(_fileTargetPath, _arch);
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

//-------------------------------------------------------------------------------------
void Targets::process(std::shared_ptr<Future> _future) {
    if (enableFpgaTarget) {
        fpgaTarget->process(_future);
    }
    if (enableSimTarget) {
        simTarget->process(_future);
    }
    if (enableGoldenModelTarget) {
        goldenModelTarget->process(_future);
    }
    if (fileTarget) {
        fileTarget->process(_future);
    }
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
