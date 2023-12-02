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
#include <any>
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
#include <vector>

// forward declarations
struct Arch;

//-------------------------------------------------------------------------------------
class InternalLibraryLoader {
    [[maybe_unused]] const Arch& arch;

    std::unordered_map<std::string, std::any> functionMap;

    std::vector<std::vector<uint32_t>> stickyFunctionsCode;

    std::vector<std::unique_ptr<LowLevelFunctionInfo>> stickyFunctions;

    static std::vector<uint32_t> stickyHaltFunctionCode(const Arch& _arch);
    static std::unique_ptr<LowLevelFunctionInfo> stickyHaltFunction(
        const Arch& _arch, std::vector<std::vector<uint32_t>>& stickyFunctionsCode);

  public:
    InternalLibraryLoader(const Arch& _arch);

    ~InternalLibraryLoader() = default;

    std::vector<std::unique_ptr<LowLevelFunctionInfo>>& stickyFunctionsToLoad();
    LowLevelFunctionInfo* resolve(const std::string& _name);
};
//-------------------------------------------------------------------------------------
