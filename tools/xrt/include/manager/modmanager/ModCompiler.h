//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include "common/cache/Cache.h"
#include <string>

class ModCompiler {
    static const std::string cc;
    static const std::string cxx;
    static const std::string cflags;
    static const std::string ldflags;
    static const std::string includes;
    static const std::string cfiles;

    static const std::string buildPath;

public:
    ModCompiler() = default;
    ~ModCompiler() = default;

    std::string compile(const std::string& _sourcePath);
};
