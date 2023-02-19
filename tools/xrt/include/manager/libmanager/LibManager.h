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
    uint32_t     length; // length of machine code
    uint32_t     address; //address in HW accelerator
    uint32_t*    code; //machine code
};

#include <manager/libmanager/InternalLibraryLoader.h>
#include <manager/libmanager/JsonLibraryLoader.h>
#include <manager/memmanager/MemManager.h>

//-------------------------------------------------------------------------------------
class LibManager {

public:
  LibManager(MemManager* _memManager);
  ~LibManager();

  FunctionInfo* resolve(std::string _name);
  uint32_t uploadFunction(std::string _name);
  uint32_t uploadCode(uint32_t* _code, uint32_t _length);
  uint32_t uploadData(uint32_t* _data, uint32_t _length);

private:
  MemManager* memManager;
//  json libxpu;
//  std::unordered_map<std::string, FunctionInfo> functionMap;
//  std::unordered_map<std::string, std::any> internallyResolvedFunctionMap;

  InternalLibraryLoader* internalLibraryLoader;
  JsonLibraryLoader* jsonLibraryLoader;
};
//-------------------------------------------------------------------------------------
