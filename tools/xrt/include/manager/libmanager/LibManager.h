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


struct FunctionInfo {
//    std::vector<FunctionCode>    codes;
    uint32_t     length;
    uint32_t     address;
    uint32_t*    code;
};

#include <manager/libmanager/InternalLibraryLoader.h>
#include <manager/libmanager/JsonLibraryLoader.h>
#include <manager/memmanager/MemManager.h>

//-------------------------------------------------------------------------------------
class LibManager {

public:
  LibManager(MemManager* _memManager);
  ~LibManager();

  void resolve(std::string _name);
  void uploadFunction(std::string _name);
  void uploadData(void* _address, uint32_t _length);

private:
  MemManager* memManager;
  json libxpu;
  std::unordered_map<std::string, FunctionInfo> functionMap;
  std::unordered_map<std::string, std::any> internallyResolvedFunctionMap;

  InternalLibraryLoader* internalLibraryLoader;
  JsonLibraryLoader* jsonLibraryLoader;
};
//-------------------------------------------------------------------------------------
