//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/cache/Cache.h>
#include "manager/libmanager/FunctionInfo.hpp"
#include "manager/memmanager/SymbolInfo.hpp"
#include "manager/modmanager/ModManager.h"
#include <cstdint>
#include <stdexcept>
#include <targets/Targets.h>
#include <manager/Manager.h>
#include <common/Utils.h>
#include <filesystem>

//-------------------------------------------------------------------------------------
Manager::Manager(Targets *_targets, Cache *_cache, const Arch& _arch)
    : cache(_cache), arch(_arch) {
    
    driver = new Driver(_targets);
    memManager = new MemManager(driver, _arch);
    libManager = new LibManager(memManager, _arch);
    modManager = new ModManager(this, cache);
    libraryResolver = new LibraryResolver(_arch);

    for (FunctionInfo& _stickyFunction : libManager->stickyFunctionsToLoad()) {
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
void Manager::run(const std::string &_name) {
    SymbolInfo *_symbol = memManager->resolve(_name);

    if (_symbol == nullptr) {
        FunctionInfo *_function = libManager->resolve(_name);

        // if (_function == nullptr)
        //    throw std::runtime_error("could not load function: " + _name);

        // TODO: better solution for all this
        if (_function == nullptr) {
            const ModFunctionInfo* _modFunc = modManager->resolve(_name);

            if (_modFunc == nullptr)
                throw std::runtime_error("could not load function: " + _name);

            modManager->run(*_modFunc, {});

            return;
        }

        memManager->loadFunction(*_function);
        _symbol = memManager->resolve(_name);
        
        assert(_symbol != nullptr);
    }

    runRuntime(_symbol->address, 0, nullptr);
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(FunctionInfo *_function, uint32_t _argc, uint32_t *_argv) {
    SymbolInfo *_symbol = memManager->resolve(_function->name);

    if (_symbol == nullptr) {
        memManager->loadFunction(*_function);
        _symbol = memManager->resolve(_function->name);
        
        assert(_symbol != nullptr);
    }

    runRuntime(_symbol->address, _argc, _argv);
}

//-------------------------------------------------------------------------------------
FunctionInfo *Manager::lowLevel(const std::string& _name) {
    return libManager->resolve(_name);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart,
                               uint32_t *_ramMatrix,
                               uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                               uint32_t _ramStartLine, uint32_t _ramStartColumn,
                               uint32_t _numLines, uint32_t _numColumns) {
    
    driver->writeMatrixArray(_accMemStart, _ramMatrix, _ramTotalLines, _ramTotalColumns,
                             _ramStartLine, _ramStartColumn, _numLines, _numColumns);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(uint32_t _accMemStart,
                              uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLines, uint32_t _numColumns,
                              bool     _accRequireResultReady) {

    driver->readMatrixArray(_accMemStart, _ramMatrix, _ramTotalLines, _ramTotalColumns,
                            _ramStartLine, _ramStartColumn, _numLines, _numColumns, _accRequireResultReady);
}
//-------------------------------------------------------------------------------------
void Manager::load(const std::string &_givenPath) {
    // TODO: separate load into 2 functions?

    std::cout << "Loading: " << _givenPath << std::endl;

    // given path can be a a path (if it contains a / or an extension) or a name
    const std::string& _path =
        (_givenPath.find("/") == std::string::npos
        && _givenPath.find(".") == std::string::npos)
        
        ? libraryResolver->resolve(_givenPath, LibLevel::ANY_LEVEL).string()
        : _givenPath;

    int _fileType = getFileTypeFromGeneralPath(_path);

    switch (_fileType) {
        case XPU_FILE_HEX:
        case XPU_FILE_JSON:
        case XPU_FILE_OBJ: {
            libManager->load(_path);

            break;
        }

        case XPU_FILE_C:
        case XPU_FILE_CPP:
        case XPU_FILE_SO: {
            modManager->load(_path);

            break;
        }

        default: {
            throw std::runtime_error("Unknown file extension");
        }
    }
    
}

//-------------------------------------------------------------------------------------
// PURE DRIVER ENCAPSULATION
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void Manager::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
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
