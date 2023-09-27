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

#include <manager/libmanager/lowlevel/HexLibraryLoader.h>
#include <manager/libmanager/lowlevel/InternalLibraryLoader.h>
#include <manager/libmanager/lowlevel/JsonLibraryLoader.h>
#include <manager/memmanager/MemManager.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class LowLevelLibManager {
    InternalLibraryLoader* internalLibraryLoader;
    HexLibraryLoader* hexLibraryLoader;
    JsonLibraryLoader* jsonLibraryLoader;

  public:
    LowLevelLibManager(MemManager* _memManager, const Arch& _arch);

    ~LowLevelLibManager() = default;

    LowLevelFunctionInfo* resolve(const std::string& _name);
    std::vector<LowLevelFunctionInfo>& stickyFunctionsToLoad();

    void load(const std::string& _path);
};
//-------------------------------------------------------------------------------------
