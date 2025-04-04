//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Args.hpp>
#include <targets/Targets.hpp>

#include "common/allocator/SAllocator.hpp"
#include <exported/midlevel/Accelerator.hpp>

Accelerator* newAccelerator(std::span<const char* const> _args) {
    std::vector<std::string_view> _argsV;

    for (const char* const _arg : _args) {
        _argsV.push_back(_arg);
    }

    return newAccelerator(_argsV);
}

Accelerator* newAccelerator(std::span<std::string_view> _argv) {
    Args _args = parseArgs(_argv);

    // if (_args.enableCmdSource || _args.enableNetSource || _args.enableBatchSource
    //     || _args.enableFileSource) {
    //     throw std::runtime_error(
    //         "Source is not supported when using XRT as a library");
    // }

    if (!_args.enableFpgaTarget && !_args.enableSimTarget && !_args.enableGoldenModelTarget
        && !_args.enableFileTarget) {
        throw std::runtime_error("At least one target must be enabled");
    }

    initLogs(_args.logSuffix);

    if (_args.enableFpgaTarget) {
        initGSAllocator(SAllocatorType::UDma);
    } else {
        initGSAllocator(SAllocatorType::Malloc);
    }

    logInit.print<InfoHigh>("accelerator args:");
    for (auto& arg : _argv) {
      logInit.print<InfoHigh>(" {}", arg);
    }
    logInit.println<InfoHigh>("");
		

    auto _arch = std::make_shared<Arch>();

    if (!_args.enableFpgaTarget) {
        if (_args.archStr == "") {
            parseArchFile(*_arch);
        } else {
            parseArchFile(*_arch, _args.archStr);
        }
    }

    auto _targets = std::make_unique<Targets>(

        *_arch,
        _args.fileTargetPath,
        _args.enableFpgaTarget,
        _args.enableSimTarget,
        _args.enableGoldenModelTarget,
        _args.enableWdb,
        _args.haveAcceleratorImageFromLog,
        _args.logSuffix,
        _args.simClockPeriodNs);
    Accelerator* _acc = new Manager(std::move(_targets), std::move(_arch));
    _acc->initLowLevelStdlib();

    return _acc;
}

void deleteAccelerator(Accelerator *acc) {
    delete acc;

    destroyGSAllocator();
}
