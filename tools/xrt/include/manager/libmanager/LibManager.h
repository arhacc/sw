//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
#include <manager/libmanager/HexLibraryLoader.h>
#include <manager/libmanager/InternalLibraryLoader.h>
#include <manager/libmanager/JsonLibraryLoader.h>
#include <manager/memmanager/MemManager.h>

using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class LibManager {
    const Arch& arch;

    MemManager *memManager;

    InternalLibraryLoader *internalLibraryLoader;
    HexLibraryLoader *hexLibraryLoader;
    JsonLibraryLoader *jsonLibraryLoader;
public:
    LibManager(MemManager *_memManager, const Arch& _arch);

    ~LibManager() = default;

    FunctionInfo *resolve(const std::string& _name);
    std::vector<FunctionInfo>& stickyFunctionsToLoad();

    void load(const std::string& _path);
};
//-------------------------------------------------------------------------------------
