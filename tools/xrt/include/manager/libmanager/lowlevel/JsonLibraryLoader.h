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

//-------------------------------------------------------------------------------------
class JsonLibraryLoader {
  public:
    JsonLibraryLoader() = default;

    ~JsonLibraryLoader() = default;

    LowLevelFunctionInfo* resolve(const std::string& _name);

    void loadFeaturesSegment(const json::iterator& _it);

    void loadCodeSegment(const json::iterator& _it);

    void loadDataSegment(const json::iterator& _it);

    void load(const std::string& _path);

    void loadSegments(json _libxpu);

    void loadCrc(const json::iterator& _it);

    void loadFunction(auto& _code);

  private:
    std::unordered_map<std::string, LowLevelFunctionInfo> functionMap;
};
//-------------------------------------------------------------------------------------
