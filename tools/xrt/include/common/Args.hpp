//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <span>
#include <string>
#include <vector>
#include <cstdint>

struct Args {
    bool enableFpgaTarget            = false;
    bool enableSimTarget             = false;
    bool enableWdb                   = true;
    bool enableGoldenModelTarget     = false;
    bool enableFileTarget            = false;
    bool haveAcceleratorImageFromLog = true;
    std::string fileTargetPath   = "";

    bool enableCmdSource;
    bool enableNetSource;
    std::string serverPort = "";
    bool enableBatchSource;
    std::vector<std::string> batchFiles;
    bool enableFileSource;
    std::vector<std::string> sourceFiles;

    std::string archStr   = "";
    std::string logSuffix = "";

    uint32_t simClockPeriodNs = 10;
};

Args parseArgs(std::span<std::string_view> _args);
Args parseArgs(int argc, const char* const* argv);
