//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Args.h>
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.h>
#include <manager/Manager.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/midlevel/Callbacks.h>
#include <targets/Targets.h>

#include <cstdint>
#include <cstdio>
#include <memory>
#include <utility>
#include <vector>

#include <fmt/core.h>

#define REGISTER_CALLBACK(F) \
    { #F, reinterpret_cast < void*>(F) }

const std::vector<std::pair<const char*, void*>> xpu_allFunctions{
    REGISTER_CALLBACK(xpu_load),
    REGISTER_CALLBACK(xpu_runRuntime),
    REGISTER_CALLBACK(xpu_readRegister),
    REGISTER_CALLBACK(xpu_writeRegister),
    REGISTER_CALLBACK(xpu_lowLevel),
    REGISTER_CALLBACK(xpu_writeRawInstruction),
    REGISTER_CALLBACK(xpu_writeRawInstructions),
    REGISTER_CALLBACK(xpu_readMatrixArray),
    REGISTER_CALLBACK(xpu_writeMatrixArray),
};

//-------------------------------------------------------------------------------------
extern "C" XrtContext* xpu_init(int _argc, const char* const* _argv) {
    try {
        fmt::println("Callback xpu_init()");

        Args _args = parseArgs(_argc, _argv);

        if (_args.enableCmdSource || _args.enableNetSource || _args.enableBatchSource
            || _args.enableFileSource) {
            throw std::runtime_error(
                "Source is not supported when using XRT as a library");
        }

        if (!_args.enableFpgaTarget && !_args.enableSimTarget
            && !_args.enableGoldenModelTarget && !_args.enableFileTarget) {
            throw std::runtime_error("At least one target must be enabled");
        }

        auto _arch = std::make_unique<Arch>();

        if (!_args.enableFpgaTarget) {
            parseArchFile(*_arch);
        }

        auto _targets = std::make_unique<Targets>(
            *_arch,
            _args.fileTargetPath,
            _args.enableFpgaTarget,
            _args.enableSimTarget,
            _args.enableGoldenModelTarget);
        auto _manager = std::make_unique<Manager>(_targets.get(), *_arch);

        return new XrtContext(std::move(_arch), std::move(_manager), std::move(_targets));
    } catch (std::exception& e) {
        std::cout << "Exception in init callback: " << e.what() << std::endl;

        return nullptr;
    } catch (...) {
        std::cout << "Unidentified exception in init callback" << std::endl;

        return nullptr;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_close(XrtContext* _ctx) {
    try {
        fmt::println("Callback xpu_close()");

        delete _ctx;
    } catch (std::exception& e) {
        std::cout << "Exception in close callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in close callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_load(XrtContext* _ctx, const char* _path) {
    fmt::println("Callback xpu_load({})", _path);

    try {
        _ctx->getManager()->load(std::string(_path));
    } catch (std::exception& e) {
        std::cout << "Exception in load callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in load callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_runRuntime(
    XrtContext* _ctx, LowLevelFunctionInfo* _function, uint32_t _argc, uint32_t* _argv) {
    fmt::println(
        "Callback xpu_runRuntime({}, {})",
        (_function == nullptr) ? "NULL" : _function->name,
        _argc);

    try {
        _ctx->getManager()->runRuntime(_function, _argc, _argv);
    } catch (std::exception& e) {
        std::cout << "Exception in runRuntime callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in runRuntime callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" LowLevelFunctionInfo* xpu_lowLevel(XrtContext* _ctx, const char* _path) {
    fmt::println("Callback xpu_lowLevel({})", _path);

    try {
        // TODO: change low level type to string view to avoid extra copy here
        return _ctx->getManager()->lowLevel(std::string(_path));
    } catch (std::exception& e) {
        std::cout << "Exception in lowLevel callback: " << e.what() << std::endl;

        return nullptr;
    } catch (...) {
        std::cout << "Unidentified exception in lowLevel callback" << std::endl;

        return nullptr;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_writeRawInstruction(XrtContext* _ctx, uint32_t _instruction) {
    fmt::println("Callback xpu_writeRawInstruction({})", _instruction);

    try {
        _ctx->getManager()->writeRawInstruction(_instruction);
    } catch (std::exception& e) {
        std::cout << "Exception in writeRawInstruction callback: " << e.what()
                  << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in writeRawInstruction callback"
                  << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_writeRawInstructions(
    XrtContext* _ctx, const uint32_t* _instructions, uint32_t _length) {
    fmt::println(
        "Callback xpu_writeRawInstructions({}, {})", (void*) _instructions, _length);

    try {
        _ctx->getManager()->writeRawInstructions(_instructions, _length);
    } catch (std::exception& e) {
        std::cout << "Exception in writeRawInstructions callback: " << e.what()
                  << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in writeRawInstructions callback"
                  << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_readMatrixArray(
    XrtContext* _ctx,
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady) {
    fmt::println(
        "Callback xpu_readMatrixArray({}, {}, {}, {}, {}, {}, {}, {}, {})",
        _accMemStart,
        static_cast<void*>(_ramMatrix),
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns,
        _accRequireResultReady);

    try {
        _ctx->getManager()->readMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns,
            _accRequireResultReady);
    } catch (std::exception& e) {
        std::cout << "Exception in readMatrixArray callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in readMatrixArray callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_writeMatrixArray(
    XrtContext* _ctx,
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    fmt::println(
        "Callback xpu_writeMatrixArray({}, {}, {}, {}, {}, {}, {}, {})",
        _accMemStart,
        static_cast<void*>(_ramMatrix),
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns);

    try {
        _ctx->getManager()->writeMatrixArray(
            _accMemStart,
            _ramMatrix,
            _ramTotalLines,
            _ramTotalColumns,
            _ramStartLine,
            _ramStartColumn,
            _numLines,
            _numColumns);
    } catch (std::exception& e) {
        std::cout << "Exception in writeMatrixArray callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in writeMatrixArray callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C" uint32_t xpu_readRegister(XrtContext* _ctx, uint32_t _address) {
    fmt::println("Callback xpu_readRegister({}))", _address);

    try {
        return _ctx->getManager()->readRegister(_address);
    } catch (std::exception& e) {
        std::cout << "Exception in readRegister callback: " << e.what() << std::endl;

        return 0xdeadbeef;
    } catch (...) {
        std::cout << "Unidentified exception in readRegister callback" << std::endl;

        return 0xdeadbeef;
    }
}

//-------------------------------------------------------------------------------------
extern "C" void xpu_writeRegister(XrtContext* _ctx, uint32_t _address, uint32_t _value) {
    fmt::println("Callback xpu_writeRegister({}, {}))", _address, _value);

    try {
        _ctx->getManager()->writeRegister(_address, _value);
    } catch (std::exception& e) {
        std::cout << "Exception in writeRegister callback: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unidentified exception in writeRegister callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
