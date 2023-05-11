//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "manager/libmanager/FunctionInfo.hpp"
#include "targets/fpga/FpgaTarget.h"
#include <dlfcn.h>
#include <manager/modmanager/Callbacks.h>
#include <cassert>
#include <manager/modmanager/ModCompiler.h>
#include <manager/modmanager/ModManager.h>
#include <stdexcept>
#include <common/Utils.h>

//-------------------------------------------------------------------------------------
ModManager::ModManager(Manager *_manager) {
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
            std::string _modulePath = modCompiler->compile(_path);
            loadModule(_modulePath);
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
    auto _callbackLoadPtr = static_cast<void (**)(const char *)>(dlsym(_module, "load"));

    if (_callbackLoadPtr == nullptr)
        throw std::runtime_error("function load not found in callback table");

    *_callbackLoadPtr = callbackLoad;

    auto _callbackRunRuntimePtr = static_cast<void (**)(void *)>(dlsym(_module, "runRuntime"));

    if (_callbackRunRuntimePtr == nullptr)
        throw std::runtime_error("function runRuntime not found in callback table");

    *_callbackRunRuntimePtr = callbackRunRuntime;

    auto _callbackLowLevelPtr = static_cast<void *(**)(const char *)>(dlsym(_module, "lowLevel"));

    if (_callbackRunRuntimePtr == nullptr)
        throw std::runtime_error("function lowLevel not found in callback table");

    *_callbackLowLevelPtr = callbackLowLevel;
}

//-------------------------------------------------------------------------------------
