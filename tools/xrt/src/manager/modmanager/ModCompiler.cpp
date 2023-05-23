//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "common/Utils.h"
#include "common/cache/Cache.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <manager/modmanager/ModCompiler.h>
#include <stdexcept>
#include <fmt/format.h>
#include <common/Utils.h>
#include <filesystem>

namespace fs = std::filesystem;

const std::string ModCompiler::cc = "gcc";
const std::string ModCompiler::cxx = "g++";
const std::string ModCompiler::cflags = "-x c -shared -fPIC -g";
const std::string ModCompiler::ldflags = "";
const std::string ModCompiler::includes = "-I" + getXpuHome() + "/xrt/include";
const std::string ModCompiler::cfiles = getXpuHome() + "/xrt/src/callbackTable.c";

const std::string ModCompiler::buildPath = getXpuHome() + "/xrt/build";

//-------------------------------------------------------------------------------------
std::string ModCompiler::compile(const std::string& _sourcePathStr) {
    std::string _compiler;
    int _fileType = getFileTypeFromGeneralPath(_sourcePathStr);
    fs::path _sourcePath{_sourcePathStr};

    fs::create_directories(buildPath);

    if (_sourcePath.extension().string().length() >= 3 &&
        std::string_view(_sourcePath.extension().string().begin(),
                         _sourcePath.extension().string().begin() + 3) == ".0x")
        _sourcePath = _sourcePath.replace_extension();

    if (_fileType == XPU_FILE_C) {
        _compiler = cc;
    } else if (_fileType == XPU_FILE_CPP) {
        _compiler = cxx;
    } else {
        assert(((void) "attempting to compile non-C/C++ file", false));
    }

    if (Cache::isCachePath(_sourcePath))
        _sourcePath = _sourcePath.stem();

    std::string _outputPath = buildPath + "/" + _sourcePath.stem().string() + ".so";
    // TODO: log compiler stderr output
    int _ret = std::system((_compiler + " " + cflags + " " + includes + " \"" + _sourcePathStr + "\" " + cfiles + " " + ldflags + " -o \"" + _outputPath + "\"").c_str());

    if (_ret == -1)
        throw std::runtime_error("could not lunch sub-shell");

    if (!WIFEXITED(_ret))
        throw std::runtime_error("compiler did not exit normally");

    if (WEXITSTATUS(_ret) != 0)
        throw std::runtime_error(fmt::format("compiler exited with error code {}", WEXITSTATUS(_ret)));

    

    return _outputPath;
}

//-------------------------------------------------------------------------------------
