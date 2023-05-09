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
#include <sstream>
#include <iomanip>
#include <fstream>
#include "FunctionInfo.hpp"

//-------------------------------------------------------------------------------------
class HexLibraryLoader {
    std::unordered_map<std::string, FunctionInfo> functionMap;

    // private functions
    static FunctionInfo parseFile(std::istream& _input, const std::string& _name);

public:
    HexLibraryLoader() = default;

    ~HexLibraryLoader() = default;

    FunctionInfo *resolve(const std::string& _name);

    void load(const std::string& _path, const std::string& _name = "");
};
//-------------------------------------------------------------------------------------
