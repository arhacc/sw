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
#include <any>
#include <vector>
#include "FunctionInfo.hpp"

//-------------------------------------------------------------------------------------
class InternalLibraryLoader {
    std::unordered_map<std::string, std::any> functionMap;

    std::vector<FunctionInfo> stickyFunctions;

    static std::vector<uint32_t> stickyHaltFunctionCode;
    static FunctionInfo stickyHaltFunction;
public:
    InternalLibraryLoader();

    ~InternalLibraryLoader() = default;

    std::vector<FunctionInfo>& stickyFunctionsToLoad();
    FunctionInfo *resolve(const std::string& _name);
};
//-------------------------------------------------------------------------------------
