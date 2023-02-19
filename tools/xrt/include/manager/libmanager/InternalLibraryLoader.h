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


//-------------------------------------------------------------------------------------
class InternalLibraryLoader {

public:
  InternalLibraryLoader();
  ~InternalLibraryLoader();

//  void loadFunction(auto& _code);
  FunctionInfo* resolve(std::string _name);

private:
  std::unordered_map<std::string, std::any> functionMap;
};
//-------------------------------------------------------------------------------------
