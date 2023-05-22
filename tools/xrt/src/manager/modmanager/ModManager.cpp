//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "common/cache/Cache.h"
#include "manager/libmanager/FunctionInfo.hpp"
#include "targets/fpga/FpgaTarget.h"
#include <dlfcn.h>
#include <manager/modmanager/Callbacks.h>
#include <cassert>
#include <manager/modmanager/ModCompiler.h>
#include <manager/modmanager/ModManager.h>
#include <stdexcept>
#include <common/Utils.h>
#include <filesystem>

//-------------------------------------------------------------------------------------
ModManager::ModManager(Manager *_manager, Cache *_cache)
    : cache(_cache) {
    modCompiler = new ModCompiler;

    assert(callbackManager == nullptr);
    callbackManager = _manager;
}

//-------------------------------------------------------------------------------------
ModManager::~ModManager() {
    delete modCompiler;

    for (void *_module : modules) {
        dlclose(_module);
    }
}

//-------------------------------------------------------------------------------------
ModFunction ModManager::resolve(const std::string &_name) {
    ModFunction _function = nullptr;

    for (void *_module : modules) {
        if ((_function = reinterpret_cast<ModFunction>(dlsym(_module, _name.c_str()))) != nullptr)
            break;
    }

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
    void *_module = dlopen(_path.c_str(), RTLD_LAZY | RTLD_LOCAL);

    if (_module == nullptr)
        throw std::runtime_error(std::string("failed to load module ") + _path + ": " + std::string(dlerror()));

    fillCallbackTable(_module);

    modules.push_back(_module);
}

//-------------------------------------------------------------------------------------
void ModManager::fillCallbackTable(void *_module) {
    fillCallbackEntry(_module, "load", reinterpret_cast<void *>(callbackLoad));
    fillCallbackEntry(_module, "lowLevel", reinterpret_cast<void *>(callbackLowLevel));
    fillCallbackEntry(_module, "runRuntime", reinterpret_cast<void *>(callbackRunRuntime));
    fillCallbackEntry(_module, "readMatrixArray", reinterpret_cast<void *>(callbackReadMatrixArray));
    fillCallbackEntry(_module, "writeMatrixArray", reinterpret_cast<void *>(callbackWriteMatrixArray));
}

//-------------------------------------------------------------------------------------
void ModManager::fillCallbackEntry(void *_module, const std::string& _functionName, void *_functionPtr) {
    void **_functionPtrLocation = static_cast<void **>(dlsym(_module, _functionName.c_str()));

    if (_functionPtrLocation == nullptr) {
        throw std::runtime_error("function " + _functionName + " not found in callback table");
    }
    
    *_functionPtrLocation = _functionPtr;
}
