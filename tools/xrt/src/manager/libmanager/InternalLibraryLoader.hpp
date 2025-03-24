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

#include <manager/libmanager/LowLevelFunctionInfo.hpp>

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
    const Arch& arch;

    static std::vector<uint32_t> stickyHaltFunctionCode(const Arch& _arch);
    static std::unique_ptr<LowLevelFunctionInfo> stickyHaltFunction(const Arch& _arch);

  public:
    InternalLibraryLoader() = delete;

    static std::vector<std::unique_ptr<LowLevelFunctionInfo>> stickyFunctionsToLoad(const Arch& _arch);
};
//-------------------------------------------------------------------------------------
