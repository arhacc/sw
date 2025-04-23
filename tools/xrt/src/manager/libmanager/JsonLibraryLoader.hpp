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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#if 0

using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class JsonLibraryLoader {
  public:
    JsonLibraryLoader() = default;

    ~JsonLibraryLoader() = default;

    void load(const std::string& _path);

  private:
    void loadSegments(json _libxpu);

    void loadCrc(const json::iterator& _it);

    void loadFunction(auto& _code);

    void loadFeaturesSegment(const json::iterator& _it);

    void loadCodeSegment(const json::iterator& _it);

    void loadDataSegment(const json::iterator& _it);
};

#endif
//-------------------------------------------------------------------------------------
