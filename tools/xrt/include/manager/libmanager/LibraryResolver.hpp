//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <string>
#include <vector>

// forward declarations
struct Arch;
enum class LibLevel;

class LibraryResolver {
    static const std::filesystem::path cLibPath;

    const std::filesystem::path lowLevelLibraryPath;

  public:
    LibraryResolver(const Arch& _arch);
    ~LibraryResolver() = default;

    std::filesystem::path resolve(const std::string& _name, LibLevel _level);

    std::vector<std::pair<std::filesystem::path, LibLevel>> getStandardLibrary();
};
