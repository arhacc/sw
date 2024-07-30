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
        std::string_view _logSuffix);

    ~Targets();

    void reset();

    void process(std::shared_ptr<Future> _future);

    uint64_t getTime();

    void runClockCycle();
    void runClockCycles(unsigned);
    std::shared_ptr<AcceleratorImage> getAcceleratorImageFromLog();
    void setReportInterrupt(bool _reportInterrupt);
};

//-------------------------------------------------------------------------------------
