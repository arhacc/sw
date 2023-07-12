//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <string>
#include <ostream>
#include <array>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "LowLevelFunctionInfo.hpp"

//-------------------------------------------------------------------------------------
class HexLibraryLoader {
    std::unordered_map<std::string, LowLevelFunctionInfo> functionMap;

    // private functions
    static LowLevelFunctionInfo parseFile(std::istream& _input, const std::string& _name);
    static std::array<uint32_t, 2> parseLine(const std::string& _line);

public:
    HexLibraryLoader() = default;

    ~HexLibraryLoader() = default;

    LowLevelFunctionInfo *resolve(const std::string& _name);

    void load(const std::string& _path, const std::string& _name = "");
};
//-------------------------------------------------------------------------------------
