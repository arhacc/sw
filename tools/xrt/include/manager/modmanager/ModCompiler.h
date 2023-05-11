//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <string>

class ModCompiler {
    static const std::string cc;
    static const std::string cxx;
    static const std::string cflags;
    static const std::string ldflags;
    static const std::string includes;
    static const std::string cfiles;

    std::string safeDirname(std::string);

public:
    ModCompiler() = default;
    ~ModCompiler() = default;

    std::string compile(std::string _sourcePath);
};
