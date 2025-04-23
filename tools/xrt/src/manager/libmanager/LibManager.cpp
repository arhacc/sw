//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/HexLibraryLoader.hpp>
#include <manager/libmanager/InternalLibraryLoader.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/LibManager.hpp>

#include <memory>
#include <stdexcept>
#include <string_view>

#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
LibManager::LibManager(const Arch& _arch) : arch(_arch), internalLibraryLoader() {}

//-------------------------------------------------------------------------------------
std::vector<std::filesystem::path> LibManager::getLowLevelStandardLibrary() {
    std::vector<std::filesystem::path> _libs;

    for (const std::filesystem::path& _path : std::filesystem::directory_iterator(
             getPath(ResourceDirectory::LowLevelLibrariesPrefix) / arch.IDString
             / getPath(ResourceDirectory::LowLevelLibrariesPostfix))) {
        if (_path.extension() == ".json" || _path.extension() == ".hex" || _path.extension() == ".obj") {
            _libs.push_back(_path);
        }
    }

    return _libs;
}

//-------------------------------------------------------------------------------------
void LibManager::initLowLevelStdlib() {
    logInit.println<InfoMedium>("Loading standard library");
    for (const auto& _path : getLowLevelStandardLibrary()) {
        logInit.println<InfoMedium>("Loading standard library file: {}", _path.string());

        load(_path, getFileNameFromPath(_path));
    }
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo& LibManager::resolve(std::string_view _name) const {
    // TODO PERF: Eliminate this string creation
    try {
        return *functionMap.at(std::string(_name));
    } catch (const std::out_of_range&) {
        throw std::runtime_error(fmt::format("Function resolved but not loaded: {}", _name));
    }
}

//-------------------------------------------------------------------------------------
void LibManager::load(const std::filesystem::path& _path, std::string_view _name) {
    //if (_path.extension() == ".hex") {
        auto _func{HexLibraryLoader::load(_path, _name)};
        auto& _funcName{_func->name};

        functionMap.insert({_funcName, std::move(_func)});
    //}
    // else if (_path.extension() == ".json") {
    //    throw std::runtime_error("json not yet implemented");
    // } else {
    //    throw std::runtime_error(fmt::format("no known extension for low level file {}", _path.string()));
    // }
}

//-------------------------------------------------------------------------------------
std::vector<std::unique_ptr<LowLevelFunctionInfo>> LibManager::stickyFunctionsToLoad() {
    return internalLibraryLoader->stickyFunctionsToLoad(arch);
}

//-------------------------------------------------------------------------------------
