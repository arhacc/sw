//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/cache/Cache.hpp>
#include <manager/libmanager/midlevel/ModCompiler.hpp>
#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>

#include <any>
#include <unordered_map>
#include <vector>

#include <dyncall.h>
#include <dynload.h>

class Manager;

class ModManager {
    ModCompiler* modCompiler;

    std::vector<DLLib*> modules;

    std::unordered_map<std::string, ModFunctionInfo> functions;

    void loadModule(const std::string& _path);
    void loadFunctionsFromModule(const std::string& _path, DLLib* _module);

    static void fillCallbackTable(DLLib* _module);
    static void fillCallbackEntry(DLLib* _module, const std::string& _functionName, void* _functionPtr);

    static void
    loadArgument(DCCallVM* _callVM, const ModFunctionArgument& _argInfo, std::vector<std::any> _args, size_t& _argsPos);

  public:
    ModManager();
    ~ModManager();

    void load(const std::string& _path);
    const ModFunctionInfo* resolve(std::string _name);
    inline const ModFunctionInfo* resolve(std::string_view _name) {
        return resolve(std::string(_name));
    };

    void run(const std::string& _name, std::vector<std::any> _args);
    void run(const ModFunctionInfo& _function, std::vector<std::any> _args);
};
