//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>

#include <algorithm>
#include <array>
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

//-------------------------------------------------------------------------------------
class HexLibraryLoader {
    std::unordered_map<std::string, std::unique_ptr<LowLevelFunctionInfo>> functionMap;

    // private functions
    static std::unique_ptr<LowLevelFunctionInfo>
    parseFile(std::istream& _input, const std::string& _name);
    static std::array<uint32_t, 2> parseLine(const std::string& _line);

  public:
    HexLibraryLoader() = default;

    ~HexLibraryLoader() = default;

    LowLevelFunctionInfo* resolve(std::string _name);
    inline LowLevelFunctionInfo* resolve(std::string_view _name) {
        return resolve(std::string(_name));
    };

    void load(const std::string& _path, const std::string& _name = "");
};
//-------------------------------------------------------------------------------------
