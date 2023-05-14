//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include "common/cache/Cache.h"
#include <manager/modmanager/ModCompiler.h>

#include <vector>

typedef void (*ModFunction)();

class Manager;

class ModManager {
    ModCompiler *modCompiler;

    Cache *cache;

    // TODO: investigate possiblity of one hash map for function names
    std::vector<void *> modules;

    void loadModule(const std::string& _path);
    static void fillCallbackTable(void *module);
public:
    ModManager(Manager *_manager, Cache *_cache);
    ~ModManager();

    void load(const std::string& _path);
    ModFunction resolve(const std::string& _name);
};
