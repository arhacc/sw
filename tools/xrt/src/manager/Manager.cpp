//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.h>
#include <common/cache/Cache.h>
#include <manager/Manager.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>
#include <manager/memmanager/SymbolInfo.hpp>
#include <targets/Targets.h>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <span>
#include <stdexcept>

//-------------------------------------------------------------------------------------
Manager::Manager(Targets* _targets, const Arch& _arch) {
    driver     = new Driver(_targets, _arch);
    memManager = new MemManager(driver, _arch);
    libManager = new LibManager(_arch, memManager, this);

    for (LowLevelFunctionInfo& _stickyFunction : libManager->stickyFunctionsToLoad()) {
        memManager->loadFunction(_stickyFunction, true);
    }
}

//-------------------------------------------------------------------------------------
Manager::~Manager() {
    delete libManager;
    delete memManager;
    delete driver;
}

//-------------------------------------------------------------------------------------
void Manager::run(const std::string& _name) {
    SymbolInfo* _symbol = memManager->resolve(_name);

    if (_symbol != nullptr) {
        runRuntime(_symbol->address, 0, nullptr);
        return;
    }

    FunctionInfo _function = libManager->resolve(_name, LibLevel::ANY_LEVEL);

    run(_function);
}

//-------------------------------------------------------------------------------------
void Manager::run(FunctionInfo _function) {
    switch (_function.level) {
        case LibLevel::LOW_LEVEL: {
            runRuntime(_function.lowLevel, 0, nullptr);
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
    LowLevelFunctionInfo* _function, uint32_t _argc, uint32_t* _argv) {
    SymbolInfo* _symbol = memManager->resolve(_function->name);

    if (_symbol == nullptr) {
        memManager->loadFunction(*_function);
        _symbol = memManager->resolve(_function->name);

        assert(_symbol != nullptr);
    }

    runRuntime(_symbol->address, _argc, _argv);
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo* Manager::lowLevel(const std::string& _name) {
    FunctionInfo _function = libManager->resolve(_name, LibLevel::LOW_LEVEL);

    assert(_function.level == LibLevel::LOW_LEVEL);

    return _function.lowLevel;
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    driver->writeMatrixArray(
        _accMemStart,
        _ramMatrix,
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady) {
    driver->readMatrixArray(
        _accMemStart,
        _ramMatrix,
        _ramTotalLines,
        _ramTotalColumns,
        _ramStartLine,
        _ramStartColumn,
        _numLines,
        _numColumns,
        _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
void Manager::load(const std::string& _givenPath, LibLevel _level) {
    libManager->load(_givenPath, _level);
}

//-------------------------------------------------------------------------------------
// PURE DRIVER ENCAPSULATION
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void Manager::runRuntime(uint32_t _address, uint32_t _argc, uint32_t* _args) {
    driver->runRuntime(_address, _argc, _args);
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
void Manager::writeRawInstructions(
    const uint32_t* _instructions, uint32_t _numInstructions) {
    writeRawInstructions({_instructions, _numInstructions});
}

//-------------------------------------------------------------------------------------
