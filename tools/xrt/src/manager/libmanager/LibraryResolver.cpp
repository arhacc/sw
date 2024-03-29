//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <common/XrtException.hpp>
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/LibraryResolver.hpp>

#include <iostream>
#include <stdexcept>

#include <fmt/format.h>
#include <magic_enum.hpp>

const std::filesystem::path LibraryResolver::cLibPath = getXpuHome() / "lib";

//-------------------------------------------------------------------------------------
LibraryResolver::LibraryResolver(const Arch& _arch)
    : lowLevelLibraryPath(
        getPath(ResourceDirectory::LowLevelLibrariesPrefix) / _arch.IDString
        / getPath(ResourceDirectory::LowLevelLibrariesPostfix)) {
    if (!std::filesystem::exists(lowLevelLibraryPath)) {
        throw std::runtime_error(
            "library path does not exist: "
            "Make sure you have installed the XPU libraries correctly.");
    }

    logInit.print(fmt::format("Reading libraries from {}", cLibPath.string()));
}

//-------------------------------------------------------------------------------------
std::filesystem::path
LibraryResolver::resolve(const std::string& _name, LibLevel _level) {
    logWork.print(fmt::format(
        "Resolving library {} at level {}...\n", _name, magic_enum::enum_name(_level)));

    switch (_level) {
        case LibLevel::ANY_LEVEL: {
            for (LibLevel _level :
                 {LibLevel::HIGH_LEVEL, LibLevel::MID_LEVEL, LibLevel::LOW_LEVEL}) {
                try {
                    logWork.print(fmt::format(
                        "Attempting level {}...\n", magic_enum::enum_name(_level)));
                    return resolve(_name, _level);
                } catch (const LibNotFoundError&) {
                    continue;
                }
            }

            break;
        }

        case LibLevel::LOW_LEVEL: {
            std::filesystem::path _path = lowLevelLibraryPath / _name;

            for (const char* _ext : {".json", ".hex", ".obj"}) {
                _path.replace_extension(_ext);
                if (std::filesystem::exists(_path)) {
                    logWork.print(
                        fmt::format("Found library {} at {}\n", _name, _path.string()));
                    return _path;
                }
            }

            break;
        }

        case LibLevel::MID_LEVEL: {
            std::filesystem::path _path = cLibPath / "midlevel" / _name;

            for (const char* _ext : {".so", ".cpp", ".c"}) {
                _path.replace_extension(_ext);
                if (std::filesystem::exists(_path)) {
                    logWork.print(
                        fmt::format("Found library {} at {}\n", _name, _path.string()));
                    return _path;
                }
            }

            break;
        }

        case LibLevel::HIGH_LEVEL: {
            // TODO: LibLevel::HIGH_LEVEL
            break;
        }
    }

    throw LibNotFoundError(_name);
}

//-------------------------------------------------------------------------------------
std::vector<std::pair<std::filesystem::path, LibLevel>>
LibraryResolver::getStandardLibrary() {
    std::vector<std::pair<std::filesystem::path, LibLevel>> _libs;

    for (const std::filesystem::path& _path :
         std::filesystem::directory_iterator(lowLevelLibraryPath)) {
        if (_path.extension() == ".json" || _path.extension() == ".hex"
            || _path.extension() == ".obj") {
            _libs.emplace_back(_path, LibLevel::LOW_LEVEL);
        }
    }

    // for (const std::filesystem::path& _path :
    //      std::filesystem::directory_iterator(cLibPath / "midlevel")) {
    //     if (_path.extension() == ".so" || _path.extension() == ".cpp"
    //         || _path.extension() == ".c") {
    //         _libs.emplace_back(_path, LibLevel::MID_LEVEL);
    //     }
    // }

    return _libs;
}

//-------------------------------------------------------------------------------------
