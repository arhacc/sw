//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <common/arch/Arch.hpp>
#include <common/XrtException.h>
#include <string>
#include <vector>
#include <manager/libmanager/FunctionInfo.hpp>

class LibraryResolver {
    static const std::filesystem::path cLibPath;

    const Arch &arch;

public:
    LibraryResolver(const Arch &_arch);
    ~LibraryResolver() = default;

    std::filesystem::path resolve(const std::string &_name, LibLevel _level);

    std::vector<std::pair<std::filesystem::path, LibLevel>> getStandardLibrary();
};

