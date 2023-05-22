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
#include <common/arch/Arch.hpp>

//-------------------------------------------------------------------------------------
class InternalLibraryLoader {
    const Arch& arch;

    std::unordered_map<std::string, std::any> functionMap;

    std::vector<std::vector<uint32_t>> stickyFunctionsCode;

    std::vector<FunctionInfo> stickyFunctions;


    static
    std::vector<uint32_t> stickyHaltFunctionCode(const Arch& _arch);
    static
    FunctionInfo stickyHaltFunction(const Arch& _arch, std::vector<std::vector<uint32_t>>& stickyFunctionsCode);
public:
    InternalLibraryLoader(const Arch& _arch);

    ~InternalLibraryLoader() = default;

    std::vector<FunctionInfo>& stickyFunctionsToLoad();
    FunctionInfo *resolve(const std::string& _name);
};
//-------------------------------------------------------------------------------------
