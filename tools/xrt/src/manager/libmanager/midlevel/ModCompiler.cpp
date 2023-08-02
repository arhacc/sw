//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Defer.h>
#include <common/Utils.h>
#include <common/cache/Cache.h>
#include <manager/libmanager/midlevel/ModCompiler.h>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <reproc++/run.hpp>

namespace fs = std::filesystem;

const std::string ModCompiler::cc                  = "gcc";
const std::string ModCompiler::cxx                 = "g++";
const std::vector<std::string> ModCompiler::cflags = {
    "-x", "c", "-shared", "-fPIC", "-g"};
const std::vector<std::string> ModCompiler::cxxflags = {
    "-x", "c++", "-shared", "-fPIC", "-g"};
const std::vector<std::string> ModCompiler::ldflags  = {};
const std::vector<std::string> ModCompiler::includes = {
    "-I" + getXpuHome() + "/xrt/include"};
const std::vector<std::string> ModCompiler::cfiles = {
    getXpuHome() + "/xrt/src/callbackTable.c"};

const fs::path ModCompiler::cBuildPath = fs::path(getXpuHome()) / "tmp" / "build";

//-------------------------------------------------------------------------------------
std::string ModCompiler::compile(const std::string& _sourcePathStr) {
    std::string _compiler;
    int _fileType = getFileTypeFromGeneralPath(_sourcePathStr);
    fs::path _sourcePath{_sourcePathStr};

    fs::create_directories(cBuildPath);

    if (_sourcePath.extension().string().length() >= 3
        && std::string_view(
               _sourcePath.extension().string().begin(),
               _sourcePath.extension().string().begin() + 3)
               == ".0x")
        _sourcePath = _sourcePath.replace_extension();

    if (_fileType == XPU_FILE_C) {
        _compiler = cc;
    } else if (_fileType == XPU_FILE_CPP) {
        _compiler = cxx;
    } else {
        assert(((void) "attempting to compile non-C/C++ file", false));
    }

    // Trim MD5 hash from path
    if (Cache::isCachePath(_sourcePath))
        _sourcePath = _sourcePath.stem();

    fs::path _outputPath = cBuildPath / _sourcePath;
    _outputPath.replace_extension(".so");

    runCompiler(_compiler, _sourcePath, _outputPath);

    return _outputPath;
}

//-------------------------------------------------------------------------------------
template<typename T>
void addToArgs(std::vector<std::string>& _args, T&& _arg) {
    _args.push_back(_arg);
}

//-------------------------------------------------------------------------------------
template<>
void addToArgs(std::vector<std::string>& _args, const std::vector<std::string>& _arg) {
    _args.insert(_args.end(), _arg.begin(), _arg.end());
}

//-------------------------------------------------------------------------------------
template<>
void addToArgs(std::vector<std::string>& _args, std::vector<std::string>&& _arg) {
    _args.insert(_args.end(), _arg.begin(), _arg.end());
}

//-------------------------------------------------------------------------------------
template<typename... Args>
std::vector<std::string> buildArgs(Args&&... _args) {
    std::vector<std::string> _argv;

    (addToArgs(_argv, _args), ...);

    return _argv;
}

//-------------------------------------------------------------------------------------
void ModCompiler::runCompiler(
    const std::string& _compiler,
    const fs::path& _sourcePath,
    const fs::path& _outputPath) {
    std::vector<std::string> _args = buildArgs(
        _compiler,
        _compiler == cc ? cflags : cxxflags,
        includes,
        _sourcePath.string(),
        cfiles,
        ldflags,
        "-o",
        _outputPath.string());

    fmt::println("{}", fmt::join(_args, " "));

    reproc::process _process;

    std::error_code _ec = _process.start(_args);
    if (_ec) {
        throw std::system_error(_ec);
    }

    std::string _output;
    reproc::sink::string sink(_output);

    _ec = reproc::drain(_process, sink, sink);
    if (_ec) {
        throw std::system_error(_ec);
    }

    defer(fmt::print("{}", _output));

    int _status;
    std::tie(_status, _ec) = _process.wait(reproc::infinite);
    if (_ec) {
        throw std::system_error(_ec);
    } else if (_status != 0) {
        throw std::runtime_error(
            fmt::format("compilation failed with error code {}", _status));
    }
}

//-------------------------------------------------------------------------------------
