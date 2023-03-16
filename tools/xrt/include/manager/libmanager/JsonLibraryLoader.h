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
using json = nlohmann::json;

//-------------------------------------------------------------------------------------
class JsonLibraryLoader {

public:
  JsonLibraryLoader();
  ~JsonLibraryLoader();

  void load(std::string _path);
  FunctionInfo* resolve(std::string _name);

  void loadSegments(json _libxpu);
  void loadFeaturesSegment(json::iterator _it);
  void loadCodeSegment(json::iterator _it);
  void loadDataSegment(json::iterator _it);
  void loadCrc(json::iterator _it);
  void loadFunction(auto& _code);

private:
  std::unordered_map<std::string, FunctionInfo> functionMap;
};
//-------------------------------------------------------------------------------------
