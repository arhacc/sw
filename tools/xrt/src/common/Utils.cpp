//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Globals.h>
#include <common/Utils.h>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/printf.h>

namespace fs = std::filesystem;

//-------------------------------------------------------------------------------------
int getFileTypeFromGeneralPath(const std::string& _path) {
    std::vector<std::string> _dirs = split(_path, ".0x");
    switch (_dirs.size()) {
        case 1: {
            return getFileTypeFromPath(_path);
        }
        case 2: {
            return getFileTypeFromPath(_dirs.at(0));
        }
        default: {
            throw std::runtime_error("Unrecognizable path: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
std::string getFileStemFromGeneralPath(const std::string& _pathStr) {
    fs::path _path{_pathStr};

    fmt::println("{}", _path.string());

    std::string _extension = _path.extension();

    if (_path.extension().string().length() >= 3
        && std::string_view(_extension.begin(), _extension.begin() + 3) == ".0x") {
        _path = _path.replace_extension();
    }

    return _path.stem();
}

//-------------------------------------------------------------------------------------
int getFileTypeFromPath(const std::string& _path) {
    if (endsWith(_path, ".hex")) {
        return XPU_FILE_HEX;
    } else if (endsWith(_path, ".json")) {
        return XPU_FILE_JSON;
    } else if (endsWith(_path, ".obj")) {
        return XPU_FILE_OBJ;
    } else if (endsWith(_path, ".onnx")) {
        return XPU_FILE_ONNX;
    } else if (endsWith(_path, ".c")) {
        return XPU_FILE_C;
    } else if (
        endsWith(_path, ".C") || endsWith(_path, ".cxx") || endsWith(_path, ".cc")
        || endsWith(_path, ".cpp")) {
        return XPU_FILE_CPP;
    } else if (endsWith(_path, ".so")) {
        return XPU_FILE_SO;
    } else {
        throw std::runtime_error("Unrecognizable extension: " + _path);
    }
}

//-------------------------------------------------------------------------------------
std::vector<std::string> split(std::string _value, const std::string& _separator) {
    std::regex _regex(_separator);

    std::vector<std::string> _out(
        std::sregex_token_iterator(_value.begin(), _value.end(), _regex, -1),
        std::sregex_token_iterator());

    return _out;
}

//-------------------------------------------------------------------------------------
inline bool endsWith(std::string const& value, std::string const& ending) {
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

//-------------------------------------------------------------------------------------
std::string basename(const std::string& _path) {
    size_t _nameStartIndex = _path.find_last_of(FS_DELIMITERS) + 1;
    size_t _nameStopIndex  = _path.find_last_of('.');

    if (_nameStartIndex == std::string::npos)
        _nameStartIndex = 0;
    if (_nameStopIndex == std::string::npos)
        _nameStopIndex = _path.size();

    return {_path.begin() + _nameStartIndex, _path.begin() + _nameStopIndex};
}

//-------------------------------------------------------------------------------------
std::string getXpuHome() {
    const char* _xpuHomePtr = std::getenv("XPU_HOME");

    if (_xpuHomePtr != nullptr) {
        return _xpuHomePtr;
    }

    const char* _home = std::getenv("HOME");

    if (_home != nullptr) {
        return std::string(_home) + "/.xpu";
    }

    throw std::runtime_error("Neither XPU_HOME nor HOME is set");
}

//-------------------------------------------------------------------------------------
fs::path getPath(ResourceDirectory _resourceDirectory) {
    fs::path _path;

    switch (_resourceDirectory) {
        case ResourceDirectory::ArchitectureImplementations: {
            const char *_envXPUHardwarePath = getenv("XPU_HW_PATH");

            if (_envXPUHardwarePath != nullptr) {
                _path = fs::path(_envXPUHardwarePath) / "architecture_implementations";
            } else {
                fmt::println("Can not find XPU_HW_PATH enviornment, reverting to XPU_HOME behaviour");

                _path = fs::path(getXpuHome()) / "etc" / "architecture_implementations";
            }

            fmt::println("Looking for architecture implementations in: {}", _path.string());

            return _path;
        }
        case ResourceDirectory::LowLevelLibrariesPrefix: {
            const char *_envXPULibrariesPath = getenv("XPU_LIBRARIES_PATH");

            if (_envXPULibrariesPath != nullptr) {
                _path = fs::path(_envXPULibrariesPath) / "low_level" / "libraries";
            } else {
                fmt::println("Can not find XPU_LIBRARIES_PATH enviornment, reverting to XPU_HOME behaviour");

                _path = fs::path(getXpuHome()) / "lib" / "lowlevel";
            }

            fmt::println("Low level libraries prefix: {}", _path.string());

            return _path;
        }
        case ResourceDirectory::LowLevelLibrariesPostfix: {
            const char *_envXPULibrariesPath = getenv("XPU_LIBRARIES_PATH");

            if (_envXPULibrariesPath != nullptr) {
                _path = "hexes";

                fmt::println("Low level libraries postfix: {}", _path.string());
            } else {
                fmt::println("Can not find XPU_LIBRARIES_PATH enviornment, reverting to XPU_HOME behaviour");

                _path = "";
            }

            return _path;
        }
    }

    throw std::runtime_error("ResourceDirectory enum class out of bounds in getPath()");
}

//-------------------------------------------------------------------------------------
