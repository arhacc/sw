//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Utils.h>
#include <common/XrtException.h>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/LibraryResolver.h>

#include <iostream>
#include <stdexcept>

#include <fmt/format.h>
#include <magic_enum.hpp>

const std::filesystem::path LibraryResolver::cLibPath = getXpuHome() + "/lib";

//-------------------------------------------------------------------------------------
LibraryResolver::LibraryResolver(const Arch& _arch) : arch(_arch) {
    if (!std::filesystem::exists(cLibPath)) {
        throw std::runtime_error(
            "library path does not exist in XPU_HOME: "
            "Make sure you have installed the XPU libraries correctly.");
    }

    std::cout << fmt::format("Reading libraries from {}", cLibPath.string()) << std::endl;
}

//-------------------------------------------------------------------------------------
std::filesystem::path
LibraryResolver::resolve(const std::string& _name, LibLevel _level) {
    std::cout << fmt::format(
        "Resolving library {} at level {}...", _name, magic_enum::enum_name(_level))
              << std::endl;

    switch (_level) {
        case LibLevel::ANY_LEVEL: {
            for (LibLevel _level :
                 {LibLevel::HIGH_LEVEL, LibLevel::MID_LEVEL, LibLevel::LOW_LEVEL}) {
                try {
                    std::cout << fmt::format(
                        "Attempting level {}...", magic_enum::enum_name(_level))
                              << std::endl;
                    return resolve(_name, _level);
                } catch (const LibNotFoundError&) {
                    continue;
                }
            }

            break;
        }

        case LibLevel::LOW_LEVEL: {
            std::filesystem::path _path = cLibPath / "lowlevel" / arch.IDString / _name;

            for (const char* _ext : {".json", ".hex", ".obj"}) {
                _path.replace_extension(_ext);
                if (std::filesystem::exists(_path)) {
                    std::cout << fmt::format(
                        "Found library {} at {}", _name, _path.string())
                              << std::endl;
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
                    std::cout << fmt::format(
                        "Found library {} at {}", _name, _path.string())
                              << std::endl;
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
         std::filesystem::directory_iterator(cLibPath / "lowlevel" / arch.IDString)) {
        if (_path.extension() == ".json" || _path.extension() == ".hex"
            || _path.extension() == ".obj") {
            _libs.emplace_back(_path, LibLevel::LOW_LEVEL);
        }
    }

    for (const std::filesystem::path& _path :
         std::filesystem::directory_iterator(cLibPath / "midlevel")) {
        if (_path.extension() == ".so" || _path.extension() == ".cpp"
            || _path.extension() == ".c") {
            _libs.emplace_back(_path, LibLevel::MID_LEVEL);
        }
    }

    return _libs;
}

//-------------------------------------------------------------------------------------
