//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "common/cache/Cache.h"
#include "manager/libmanager/FunctionInfo.hpp"
#include "manager/modmanager/ModFunctionInfo.hpp"
#include "targets/fpga/FpgaTarget.h"
#include <any>
#include <cstdint>
#include <dlfcn.h>
#include <dynload.h>
#include <manager/modmanager/Callbacks.h>
#include <cassert>
#include <manager/modmanager/ModCompiler.h>
#include <manager/modmanager/ModManager.h>
#include <stdexcept>
#include <common/Utils.h>
#include <filesystem>
#include <dyncall.h>
#include <fmt/format.h>
#include <common/Defer.h>

//-------------------------------------------------------------------------------------
ModManager::ModManager(Manager *_manager, Cache *_cache)
    : manager(_manager), cache(_cache) {
    modCompiler = new ModCompiler;

    auto _knownFunctions = getKnownModFunctions();

    for (ModFunctionInfo& _function : _knownFunctions) {
        functions.insert({_function.name, _function});
    }
}

//-------------------------------------------------------------------------------------
ModManager::~ModManager() {
    delete modCompiler;

    for (DLLib *_module : modules) {
        dlFreeLibrary(_module);
    }
}

//-------------------------------------------------------------------------------------
void ModManager::run(const std::string& _name, std::vector<std::any> _args) {
    const ModFunctionInfo* _function = resolve(_name);

    if (_function == nullptr) {
        throw std::runtime_error(fmt::format("function {} could not be resolved", _name));
    }

    run(*_function, _args);
}

//-------------------------------------------------------------------------------------
void ModManager::run(const ModFunctionInfo& _function, std::vector<std::any> _args) {
    std::cout << fmt::format("Runing module function {}...", _function.name) << std::endl;

    void *_addr = _function.addr;
    if (_addr == nullptr) {
        throw std::runtime_error(fmt::format("attempting to run unloaded module function {}", _function.name));
    }

    DCCallVM* _dcCall = dcNewCallVM(4096);
    defer(dcFree(_dcCall));
    
    dcMode(_dcCall, DC_CALL_C_DEFAULT);
    dcReset(_dcCall);

    XrtContext _context(manager);

    dcArgPointer(_dcCall, &_context);

    try {
        size_t _argsIterator = 0;
        for (const ModFunctionArgument& _arg : _function.args) {
            loadArgument(_dcCall, _arg, _args, _argsIterator);
        }
    } catch (std::bad_any_cast& ex) {
        throw std::runtime_error(fmt::format("bad function argument for function {}", _function.name));
    } catch (std::out_of_range& ex) {
        throw std::runtime_error(fmt::format("bad number of arguments for function {}", _function.name));
    }

    dcCallVoid(_dcCall, _function.addr);
}

// Ensure sane C ABI type sizes
static_assert(sizeof(char) == 1, "Size of char must be 1");
static_assert(sizeof(short) == 2, "Size of char must be 2");
static_assert(sizeof(int) == 4, "Size of char must be 4");
static_assert(sizeof(long long) == 8, "Size of char must be 8");


//-------------------------------------------------------------------------------------
template<class T, class U>
static void loadArgumentPrimitive(DCCallVM *_dcCall, std::any _arg, void (*_loadFunction)(DCCallVM*,U)) {
    T _a = std::any_cast<T>(_arg);
    _loadFunction(_dcCall, static_cast<U>(_a));
}

//-------------------------------------------------------------------------------------
void ModManager::loadArgument(DCCallVM *_dcCall, const ModFunctionArgument& _argInfo, std::vector<std::any> _args, size_t &_argsPos) {
    switch (_argInfo.type) {
        case ModFunctionArgumentType::UInteger: {
            switch (_argInfo.primitive.size) {
                case 1: {
                    loadArgumentPrimitive<uint8_t>(_dcCall, _args.at(_argsPos++), dcArgChar);
                    break;
                }
                case 2: {
                    loadArgumentPrimitive<uint16_t>(_dcCall, _args.at(_argsPos++), dcArgShort);
                    break;
                }
                case 4: {
                    loadArgumentPrimitive<uint32_t>(_dcCall, _args.at(_argsPos++), dcArgInt);
                    break;
                }
                case 8: {
                    loadArgumentPrimitive<uint64_t>(_dcCall, _args.at(_argsPos++), dcArgLongLong);
                    break;
                }
            }
            break;
        }
        case ModFunctionArgumentType::SInteger: {
            switch (_argInfo.primitive.size) {
                case 1: {
                    loadArgumentPrimitive<int8_t>(_dcCall, _args.at(_argsPos++), dcArgChar);
                    break;
                }
                case 2: {
                    loadArgumentPrimitive<int16_t>(_dcCall, _args.at(_argsPos++), dcArgShort);
                    break;
                }
                case 4: {
                    loadArgumentPrimitive<int32_t>(_dcCall, _args.at(_argsPos++), dcArgInt);
                    break;
                }
                case 8: {
                    loadArgumentPrimitive<int64_t>(_dcCall, _args.at(_argsPos++), dcArgLongLong);
                    break;
                }
            }
            break;
        }
    }
}

//-------------------------------------------------------------------------------------
const ModFunctionInfo* ModManager::resolve(const std::string &_name) {
    auto _iterator = functions.find(_name);
    if (_iterator == functions.end()) {
        throw std::runtime_error(fmt::format("function {} not found", _name));
    }

    const ModFunctionInfo* _function = &_iterator->second;
    if (_function->addr == nullptr) {
        throw std::runtime_error(fmt::format("function {} not loaded", _name));
    }

    std::cout << fmt::format("Found module function {} at {}", _name, _function->addr) << std::endl;

    return _function;
}

//-------------------------------------------------------------------------------------
void ModManager::load(const std::string& _path) {
    int _fileType = getFileTypeFromGeneralPath(_path);

    switch (_fileType) {
        case XPU_FILE_C:
        case XPU_FILE_CPP: {
            std::string _compiledPath = modCompiler->compile(_path);

            std::string _resourcePath = cache->installResourceFromPath(_compiledPath);
            
            loadModule(_resourcePath);
            break;
        }

        case XPU_FILE_SO: {
            loadModule(_path);
            break;
        }

        default: {
            throw std::runtime_error("Unknown file type");
        }
    }
}

//-------------------------------------------------------------------------------------
void ModManager::loadModule(const std::string& _path) {
    std::cout << fmt::format("Loading module {}...", _path) << std::endl;

    DLLib *_module = dlLoadLibrary(_path.c_str());

    if (_module == nullptr)
        throw std::runtime_error(std::string("failed to load module ") + _path + ": " + std::string(dlerror()));

    fillCallbackTable(_module);
    loadFunctionsFromModule(_path, _module);

    modules.push_back(_module);
}

//-------------------------------------------------------------------------------------
void ModManager::loadFunctionsFromModule(const std::string& _path, DLLib *_module) {
    DLSyms *_symbols = dlSymsInit(_path.c_str());
    defer(dlSymsCleanup(_symbols));

    int _symbolsCount = dlSymsCount(_symbols);

    for (int _i = 0; _i < _symbolsCount; ++_i) {
        const char *_name = dlSymsName(_symbols, _i);

        auto _iterator = functions.find(_name);
        if (_iterator == functions.end()) {
            continue;
        }

        void *_addr = dlFindSymbol(_module, _name);
        if (_addr == nullptr) {
            throw std::runtime_error(fmt::format("odd error loading module"
                "{}: function {} exists in symbol list but not in module", _path, _name));
        }

        std::cout << fmt::format("Found module function {} at {}", _name, _addr) << std::endl;

        _iterator->second.addr = _addr;
    }
}

//-------------------------------------------------------------------------------------
void ModManager::fillCallbackTable(DLLib *_module) {
    fillCallbackEntry(_module, "xpu_load", reinterpret_cast<void *>(xpu_load));
    fillCallbackEntry(_module, "xpu_lowLevel", reinterpret_cast<void *>(xpu_lowLevel));
    fillCallbackEntry(_module, "xpu_runRuntime", reinterpret_cast<void *>(xpu_runRuntime));
    fillCallbackEntry(_module, "xpu_readRegister", reinterpret_cast<void *>(xpu_readRegister));
    fillCallbackEntry(_module, "xpu_readMatrixArray", reinterpret_cast<void *>(xpu_readMatrixArray));
    fillCallbackEntry(_module, "xpu_writeMatrixArray", reinterpret_cast<void *>(xpu_writeMatrixArray));
}

//-------------------------------------------------------------------------------------
void ModManager::fillCallbackEntry(DLLib *_module, const std::string& _functionName, void *_functionPtr) {
    void **_functionPtrLocation = static_cast<void **>(dlFindSymbol(_module, _functionName.c_str()));

    if (_functionPtrLocation == nullptr) {
        throw std::runtime_error("function " + _functionName + " not found in callback table");
    }
    
    *_functionPtrLocation = _functionPtr;
}

//-------------------------------------------------------------------------------------

