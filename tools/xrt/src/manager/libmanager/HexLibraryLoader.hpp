//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/libmanager/LowLevelFunctionInfo.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

//-------------------------------------------------------------------------------------
class HexLibraryLoader {
    // private functions
    static std::unique_ptr<LowLevelFunctionInfo> parseFile(std::istream& _input, std::string_view _name);
    static std::array<uint32_t, 2> parseLine(const std::string& _line);

  public:
    HexLibraryLoader() = delete;

    static std::unique_ptr<LowLevelFunctionInfo> load(const std::filesystem::path& _path, std::string_view _name);
};
//-------------------------------------------------------------------------------------
