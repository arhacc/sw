//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Constants.hpp>
#include <common/Utils.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/printf.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
bool hasPrefixCStr(const char* str, const char* pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

//-------------------------------------------------------------------------------------
std::string getFileNameFromPath(std::filesystem::path _path) {
    std::filesystem::path _filename = _path.filename();

    fmt::println("extension is {}", _path.extension().string());

    if (beginsWith(_path.extension().string(), ".0x")) {
        _filename = _filename.stem();
    }

    return _filename.stem();
}
//-------------------------------------------------------------------------------------
std::vector<std::string> split(std::string_view _value, std::string_view _separator) {
    std::vector<std::string> _out;

    size_t _pos;

    while ((_pos = _value.find(_separator)) != std::string::npos) {
        _out.push_back(std::string(_value.substr(0, _pos)));

        _value = _value.substr(_pos + _separator.size());
    }

    _out.push_back(std::string(_value));

    return _out;
}

//-------------------------------------------------------------------------------------
bool endsWith(std::string_view value, std::string_view ending) {
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

//-------------------------------------------------------------------------------------
bool beginsWith(std::string_view value, std::string_view beginning) {
    if (beginning.size() > value.size())
        return false;
    return std::equal(beginning.begin(), beginning.end(), value.begin());
}

//-------------------------------------------------------------------------------------
std::filesystem::path getXpuHome() {
    const char* _xpuHomePtr = std::getenv("XPU_HOME");

    if (_xpuHomePtr != nullptr) {
        return _xpuHomePtr;
    }

    const char* _home = std::getenv("HOME");

    fmt::println(
        "Warning: XPU_HOME is not set; defaulting to {}{}.xpu", _home, std::filesystem::path::preferred_separator);

    if (_home != nullptr) {
        return std::filesystem::path(_home) / ".xpu";
    }

    throw std::runtime_error("Neither XPU_HOME nor HOME is set");
}

//-------------------------------------------------------------------------------------
std::filesystem::path getPath(ResourceDirectory _resourceDirectory) {
    std::filesystem::path _path;

    switch (_resourceDirectory) {
        case ResourceDirectory::ArchitectureImplementations: {
            const char* _envHwPath = getenv("XPU_HW_PATH");

            if (_envHwPath != nullptr) {
                _path = std::filesystem::path(_envHwPath) / "architecture_implementations";
            } else {
                fmt::println(
                    "Can not find XPU_HW_PATH enviornment, reverting to XPU_HOME "
                    "behaviour");

                _path = std::filesystem::path(getXpuHome()) / "etc" / "architecture_implementations";
            }

            fmt::println("Looking for architecture implementations in: {}", _path.string());

            return _path;
        }
        case ResourceDirectory::LowLevelLibrariesPrefix: {
            //            const char *_envXPULibrariesPath = getenv("XPU_LIBRARIES_PATH");
            const char* _envLibrariesPath = getenv("XPU_LIBRARIES_PATH");

            if (_envLibrariesPath != nullptr) {
                _path = std::filesystem::path(_envLibrariesPath) / "low_level" / "libraries";
            } else {
                fmt::println(
                    "Can not find XPU_LIBRARIES_PATH enviornment, reverting to XPU_HOME "
                    "behaviour");

                _path = std::filesystem::path(getXpuHome()) / "lib" / "lowlevel";
            }

            fmt::println("Low level libraries prefix: {}", _path.string());

            return _path;
        }
        case ResourceDirectory::LowLevelLibrariesPostfix: {
            const char* _envLibrariesPath = getenv("XPU_LIBRARIES_PATH");

            if (_envLibrariesPath != nullptr) {
                _path = "hexes";

                fmt::println("Low level libraries postfix: {}", _path.string());
            } else {
                fmt::println(
                    "Can not find XPU_LIBRARIES_PATH enviornment, reverting to XPU_HOME "
                    "behaviour");

                _path = "";
            }

            return _path;
        }
        case ResourceDirectory::MidLevelLibraries: {
            std::filesystem::path _xpuHome = getXpuHome();

            return _xpuHome / "lib" / "midlevel";
        }
    }

    throw std::runtime_error("ResourceDirectory enum class out of bounds in getPath()");
}

//-------------------------------------------------------------------------------------
void createDirIfNotExists(const std::filesystem::path& _path) {
#if 0
    // This is correct but fails with a weird error on zig compiler.

    if (!std::filesystem::exists(_path)) {

#endif
    // Use old POSIX API.
    if (access(_path.c_str(), F_OK) != 0) {
        fmt::println("Creating directory: {}", _path.string());

        if (!std::filesystem::create_directories(_path)) {
            throw std::runtime_error(fmt::format("Could not create directory: {}", _path.string()));
        }
    }
}
