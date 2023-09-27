//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/XrtException.h>
#include <common/arch/Arch.hpp>
#include <manager/libmanager/FunctionInfo.hpp>

#include <filesystem>
#include <string>
#include <vector>

class LibraryResolver {
    static const std::filesystem::path cLibPath;

    const std::filesystem::path lowLevelLibraryPath;

  public:
    LibraryResolver(const Arch& _arch);
    ~LibraryResolver() = default;

    std::filesystem::path resolve(const std::string& _name, LibLevel _level);

    std::vector<std::pair<std::filesystem::path, LibLevel>> getStandardLibrary();
};
