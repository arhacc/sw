//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.hpp>
#include <common/log/Logger.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/LibManager.hpp>
#include <manager/libmanager/LibraryResolver.hpp>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>
#include <manager/memmanager/MemManager.hpp>
#include <manager/memmanager/SymbolInfo.hpp>
#include <targets/Targets.hpp>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <stdexcept>
#include <string_view>

#include <fmt/core.h>

//-------------------------------------------------------------------------------------
Manager::Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch)
    : arch(std::move(_arch)), targets(std::move(_targets)) {
    driver     = new Driver(targets.get(), *arch);
    memManager = new MemManager(driver, *arch);
    libManager = new LibManager(*arch, memManager, this);

    for (std::unique_ptr<LowLevelFunctionInfo>& _stickyFunction :
         libManager->stickyFunctionsToLoad()) {
        memManager->loadFunction(*_stickyFunction, true);
    }
}

//-------------------------------------------------------------------------------------
Manager::~Manager() {
    delete libManager;
    delete memManager;
    delete driver;
}

//-------------------------------------------------------------------------------------
void Manager::run(std::string_view _name) {
    FunctionInfo _function = libManager->resolve(_name, LibLevel::ANY_LEVEL);

    run(_function);
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::span<const uint32_t> _args) {
    runRuntime(lowLevel(_name).lowLevel, _args);
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(FunctionInfo _function, std::span<const uint32_t> _args) {
    if (_function.level != LibLevel::LOW_LEVEL) {
        throw std::runtime_error("Higher class function passed to runLowLevel");
    }
    runRuntime(_function.lowLevel, _args);
}

//-------------------------------------------------------------------------------------
void Manager::run(FunctionInfo _function) {
    switch (_function.level) {
        case LibLevel::LOW_LEVEL: {
            runRuntime(_function.lowLevel);
            break;
        }

        case LibLevel::MID_LEVEL: {
            libManager->runMidLevel(*_function.midLevel, {});
            break;
        }

        case LibLevel::HIGH_LEVEL: {
            throw std::runtime_error("High level functions are not supported yet");
        }

        case LibLevel::ANY_LEVEL: {
            throw std::runtime_error(
                "Internal error; unexpected ANY_LEVEL in function info");
        }
    }
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(
    LowLevelFunctionInfo* _function, std::span<const uint32_t> _args) {
    SymbolInfo* _symbol = memManager->resolve(_function->name);

    if (_symbol == nullptr) {
        memManager->loadFunction(*_function);
        _symbol = memManager->resolve(_function->name);

        assert(_symbol != nullptr);

        logWork.print(fmt::format(
            "Loaded lowlevel function {} at {} size {}\n",
            _function->name,
            _symbol->address,
            _function->memLength()));
    }

    logWork.print(fmt::format("Running lowlevel function {}(", _function->name));
    for (size_t _argIndex = 0; _argIndex < _args.size(); ++_argIndex) {
        uint32_t _arg = _args[_argIndex];

        logWork.print(fmt::format("{}", _arg));

        if (_argIndex != _args.size() - 1) {
            logWork.print(", ");
        }
    }
    logWork.print(fmt::format(") loaded at {}\n", _symbol->address));

    runRuntime(_symbol->address, _args);
}

//-------------------------------------------------------------------------------------
FunctionInfo Manager::lowLevel(std::string_view _name) {
    FunctionInfo _function = libManager->resolve(_name, LibLevel::LOW_LEVEL);

    assert(_function.level == LibLevel::LOW_LEVEL);

    return _function;
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart, const MatrixView* _matrixView) {
    driver->writeMatrixArray(_accMemStart, _matrixView);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(
    uint32_t _accMemStart, MatrixView* _matrixView, bool _accRequireResultReady) {
    driver->readMatrixArray(_accMemStart, _matrixView, _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView) {
    writeMatrixArray(_accMemStart, &_matrixView);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(
    uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady) {
    readMatrixArray(_accMemStart, &_matrixView, _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
void Manager::load(const std::filesystem::path& _givenPath, LibLevel _level) {
    libManager->load(_givenPath, _level);
}

//-------------------------------------------------------------------------------------
unsigned Manager::constant(ArchConstant _constant) const {
    return arch->get(_constant);
}

//-------------------------------------------------------------------------------------
// PURE DRIVER ENCAPSULATION
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void Manager::runRuntime(uint32_t _address, std::span<const uint32_t> _args) {
    driver->run(_address, _args);
}

//-------------------------------------------------------------------------------------
uint32_t Manager::readRegister(uint32_t _address) {
    return driver->readRegister(_address);
}

//-------------------------------------------------------------------------------------
void Manager::writeRegister(uint32_t _address, uint32_t _value) {
    driver->writeRegister(_address, _value);
}

//-------------------------------------------------------------------------------------
void Manager::writeRawInstruction(uint32_t _instruction) {
    driver->writeInstruction(_instruction);
}

//-------------------------------------------------------------------------------------
void Manager::writeRawInstructions(std::span<const uint32_t> _instructions) {
    driver->writeInstructions(_instructions);
}

//-------------------------------------------------------------------------------------
