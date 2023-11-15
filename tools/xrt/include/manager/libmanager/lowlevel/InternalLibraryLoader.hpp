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

#include <common/arch/Arch.hpp>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>

#include <algorithm>
#include <any>
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
#include <vector>

//-------------------------------------------------------------------------------------
class InternalLibraryLoader {
    [[maybe_unused]] const Arch& arch;

    std::unordered_map<std::string, std::any> functionMap;

    std::vector<std::vector<uint32_t>> stickyFunctionsCode;

    std::vector<LowLevelFunctionInfo> stickyFunctions;

    static std::vector<uint32_t> stickyHaltFunctionCode(const Arch& _arch);
    static LowLevelFunctionInfo stickyHaltFunction(
        const Arch& _arch, std::vector<std::vector<uint32_t>>& stickyFunctionsCode);

  public:
    InternalLibraryLoader(const Arch& _arch);

    ~InternalLibraryLoader() = default;

    std::vector<LowLevelFunctionInfo>& stickyFunctionsToLoad();
    LowLevelFunctionInfo* resolve(const std::string& _name);
};
//-------------------------------------------------------------------------------------
