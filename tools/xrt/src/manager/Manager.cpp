//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "common/cache/Cache.h"
#include "manager/libmanager/FunctionInfo.hpp"
#include "manager/memmanager/SymbolInfo.hpp"
#include "manager/modmanager/ModManager.h"
#include <cstdint>
#include <stdexcept>
#include <targets/Targets.h>
#include <manager/Manager.h>
#include <common/Utils.h>
#include <filesystem>

namespace fs = std::filesystem;

//-------------------------------------------------------------------------------------
Manager::Manager(Targets *_targets, Cache *_cache, const Arch& _arch)
    : cache(_cache), arch(_arch) {
    
    driver = new Driver(_targets);
    memManager = new MemManager(driver);
    libManager = new LibManager(memManager, arch);
    modManager = new ModManager(this, cache);

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
void Manager::reset() {
    driver->reset();
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
            ModFunction _modFunc = modManager->resolve(_name);

            if (_modFunc == nullptr)
                throw std::runtime_error("could not load function: " + _name);

            _modFunc();

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

    runRuntime(_symbol->address, 0, nullptr);
}

//-------------------------------------------------------------------------------------
void Manager::uploadFunction(const std::string &_name, int32_t _address) {
    /*FunctionInfo *_function = libManager->resolve(_name);


    _function->address = _address;

    writeCode(_function->address, _function->code, _function->length);*/
}

//-------------------------------------------------------------------------------------
FunctionInfo *Manager::lowLevel(const std::string& _name) {
    return libManager->resolve(_name);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t *_ramMatrix,
                               uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                               uint32_t _ramStartLine, uint32_t _ramStartColumn,
                               uint32_t _numLines, uint32_t _numColumns,
                               uint32_t _accMemStart) {
    
    driver->writeMatrixArray(_ramMatrix, 
                             _ramTotalLines,_ramTotalColumns,
                             _ramStartLine, _ramStartColumn,
                             _numLines, _numColumns,
                             _accMemStart);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(uint32_t _accMemStart,
                              uint32_t _numLines, uint32_t _numColumns,
                              bool     _accRequireResultReady,
                              uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn) {

    driver->readMatrixArray(_accMemStart,
                            _numLines, _numColumns,
                            _accRequireResultReady,
                            _ramMatrix,
                            _ramTotalLines, _ramTotalColumns,
                            _ramStartLine, _ramStartColumn);
}
//-------------------------------------------------------------------------------------
void Manager::load(const std::string &_givenPath) {
    std::string _resourcePath;

    if (fs::path(_givenPath).has_parent_path()) {
        std::cout << "Installing resource " << _givenPath << std::endl;

        cache->installResourceFromPath(_givenPath);

        // TODO: edge case for user requesting my.hex explicitly but my.so exists 
        _resourcePath = cache->getResource(fs::path(_givenPath).stem());

        std::cout << "Resource installed at " << _resourcePath << std::endl;
    }

    const std::string& _path = (_resourcePath.empty()) ? _givenPath : _resourcePath;

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
void Manager::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {
    driver->runDebug(_address, _args, _breakpointAddress);
}

//-------------------------------------------------------------------------------------
void Manager::readRegister(uint32_t _address, uint32_t _register) {
    driver->readRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Manager::writeRegister(uint32_t _address, uint32_t _register) {
    driver->writeRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Manager::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    driver->writeCode(_address, _code, _length);
}

//-------------------------------------------------------------------------------------
void Manager::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    driver->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    //  printf("Manager.loadCode @%d, length=%d\n", _address, _length);
    driver->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    driver->readArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    //  printf("Manager.loadCode @%d, length=%d\n", _address, _length);
    driver->writeArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::dump(const std::string &_address) {
    driver->dump(_address);
}


/*
//-------------------------------------------------------------------------------------
void Transformers::runFile(std::string _path) {
  std::cout << "Transformers::runFile: " << _path << std::endl;
  onnxTransformer -> load(_path);
  onnxTransformer -> process();
}

//-------------------------------------------------------------------------------------
void Transformers::dump(std::string _address) {
  directTransformer -> dump(_address);
}
*/
//-------------------------------------------------------------------------------------
