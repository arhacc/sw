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

#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>

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

// forward declarations
class InternalLibraryLoader;
class HexLibraryLoader;
class JsonLibraryLoader;
class MemManager;
struct Arch;

//-------------------------------------------------------------------------------------
class LowLevelLibManager {
    InternalLibraryLoader* internalLibraryLoader;
    HexLibraryLoader* hexLibraryLoader;
    JsonLibraryLoader* jsonLibraryLoader;

  public:
    LowLevelLibManager(MemManager* _memManager, const Arch& _arch);

    ~LowLevelLibManager() = default;

    LowLevelFunctionInfo* resolve(const std::string& _name);
    std::vector<std::unique_ptr<LowLevelFunctionInfo>>& stickyFunctionsToLoad();

    void load(const std::string& _path);
};
//-------------------------------------------------------------------------------------
