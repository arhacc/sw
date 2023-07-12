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
#include "LowLevelFunctionInfo.hpp"

using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class JsonLibraryLoader {

public:
    JsonLibraryLoader() = default;

    ~JsonLibraryLoader() = default;

    LowLevelFunctionInfo *resolve(const std::string& _name);

    void loadFeaturesSegment(const json::iterator& _it);

    void loadCodeSegment(const json::iterator& _it);

    void loadDataSegment(const json::iterator& _it);

    void load(const std::string& _path);

    void loadSegments(json _libxpu);

    void loadCrc(const json::iterator& _it);

    void loadFunction(auto &_code);

private:
    std::unordered_map<std::string, LowLevelFunctionInfo> functionMap;

};
//-------------------------------------------------------------------------------------
