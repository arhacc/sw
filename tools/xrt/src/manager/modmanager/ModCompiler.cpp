//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "common/Utils.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <manager/modmanager/ModCompiler.h>
#include <stdexcept>
#include <fmt/format.h>
#include <libgen.h>
#include <common/Utils.h>

const std::string ModCompiler::cc = "gcc";
const std::string ModCompiler::cxx = "g++";
const std::string ModCompiler::cflags = "-shared -fPIC -g";
const std::string ModCompiler::ldflags = "";
const std::string ModCompiler::includes = "-I/home/grffn/work/xpu/sw/tools/xrt/api/include";
const std::string ModCompiler::cfiles = "/home/grffn/work/xpu/sw/tools/xrt/api/src/callbackTable.c";

#undef basename

//-------------------------------------------------------------------------------------
std::string ModCompiler::safeDirname(std::string _path) {
    char *_unsafePath = new char[_path.size() + 1];
    std::strcpy(_unsafePath, _path.c_str());

    char *_unsafeDirname = dirname(_unsafePath);

    std::string _safePath{_unsafeDirname};

    delete [] _unsafePath;

    return _safePath;
}

//-------------------------------------------------------------------------------------
std::string ModCompiler::compile(std::string _sourcePath) {
    std::string _compiler;
    int _fileType = getFileTypeFromGeneralPath(_sourcePath);

    if (_fileType == XPU_FILE_C) {
        _compiler = cc;
    } else if (_fileType == XPU_FILE_CPP) {
        _compiler = cxx;
    } else {
        assert(((void) "attempting to compile non-C/C++ file", false));
    }

    std::string _outputPath = safeDirname(_sourcePath) + "/" + basename(_sourcePath) + ".so";
    // TODO: log compiler stderr output
    int _ret = std::system((_compiler + " " + cflags + " " + includes + " \"" + _sourcePath + "\" " + cfiles + " " + ldflags + " -o \"" + _outputPath + "\"").c_str());

    if (_ret == -1)
        throw std::runtime_error("could not lunch sub-shell");

    if (!WIFEXITED(_ret))
        throw std::runtime_error("compiler did not exit normally");

    if (WEXITSTATUS(_ret) != 0)
        throw std::runtime_error(fmt::format("compiler exited with error code {}", WEXITSTATUS(_ret)));

    return _outputPath;
}

//-------------------------------------------------------------------------------------
