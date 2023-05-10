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
    MemManager *memManager;

    InternalLibraryLoader *internalLibraryLoader;
    HexLibraryLoader *hexLibraryLoader;
    JsonLibraryLoader *jsonLibraryLoader;
public:
    LibManager(MemManager *_memManager);

    ~LibManager() = default;

    FunctionInfo *resolve(const std::string& _name);
    std::vector<FunctionInfo>& stickyFunctionsToLoad();

    uint32_t uploadFunction(const std::string& _name);

    uint32_t uploadCode(uint32_t *_code, uint32_t _length);

    void load(const std::string& _path);

    uint32_t uploadData(uint32_t *_data, uint32_t _length);
};
//-------------------------------------------------------------------------------------
