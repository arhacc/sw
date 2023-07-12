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
#include <manager/libmanager/lowlevel/HexLibraryLoader.h>
#include <manager/libmanager/lowlevel/InternalLibraryLoader.h>
#include <manager/libmanager/lowlevel/JsonLibraryLoader.h>
#include <manager/memmanager/MemManager.h>

using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class LowLevelLibManager {
    const Arch& arch;

    MemManager *memManager;

    InternalLibraryLoader *internalLibraryLoader;
    HexLibraryLoader *hexLibraryLoader;
    JsonLibraryLoader *jsonLibraryLoader;
public:
    LowLevelLibManager(MemManager *_memManager, const Arch& _arch);

    ~LowLevelLibManager() = default;

    LowLevelFunctionInfo *resolve(const std::string& _name);
    std::vector<LowLevelFunctionInfo>& stickyFunctionsToLoad();

    void load(const std::string& _path);
};
//-------------------------------------------------------------------------------------
