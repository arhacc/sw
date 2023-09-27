//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/cache/Cache.h>

#include <filesystem>
#include <string>
#include <vector>

class ModCompiler {
    static const std::string cc;
    static const std::string cxx;
    static const std::vector<std::string> cflags;
    static const std::vector<std::string> cxxflags;
    static const std::vector<std::string> ldflags;
    static const std::vector<std::string> includes;
    static const std::vector<std::string> cfiles;

    static const std::filesystem::path cBuildPath;

    void runCompiler(
        const std::string& _compiler,
        const std::filesystem::path& _sourcePath,
        const std::filesystem::path& _outputPath);

  public:
    ModCompiler()  = default;
    ~ModCompiler() = default;

    std::string compile(const std::string& _sourcePath);
};
