//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/LibManager.hpp>
#include <manager/libmanager/LibraryResolver.hpp>
#include <manager/libmanager/lowlevel/LowLevelLibManager.hpp>
#include <manager/libmanager/midlevel/ModManager.hpp>

#include <memory>

#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
LibManager::LibManager(const Arch& _arch, MemManager* _memManager, Manager* _manager)
    : libraryResolver(new LibraryResolver(_arch)),
      lowLevelLibManager(new LowLevelLibManager(_memManager, _arch)),
      modManager(new ModManager(_manager)) {
    for (const auto& [_path, _level] : libraryResolver->getStandardLibrary()) {
        logInit.print(fmt::format("Loading standard library file: {}\n", _path.string()));

        load(_path.string(), _level);
    }
}

//-------------------------------------------------------------------------------------
LibManager::~LibManager() {
    delete libraryResolver;
    delete lowLevelLibManager;
    delete modManager;
}

//-------------------------------------------------------------------------------------
FunctionInfo LibManager::resolve(std::string_view _name, LibLevel _level) {
    switch (_level) {
        case LibLevel::LOW_LEVEL: {
            return lowLevelLibManager->resolve(_name);
        }
        case LibLevel::MID_LEVEL: {
            return modManager->resolve(_name);
        }
        case LibLevel::HIGH_LEVEL: {
            throw std::runtime_error("not implemented");
        }
        case LibLevel::ANY_LEVEL: {
            try {
                return resolve(_name, LibLevel::LOW_LEVEL);
            } catch (const FunctionNotFoundError&) {
                return resolve(_name, LibLevel::MID_LEVEL);
            }
        }
    }

    throw std::runtime_error("LibManager::resolve - unreachable");
}

//-------------------------------------------------------------------------------------
void LibManager::load(const std::filesystem::path& _path, LibLevel _level) {
    switch (_level) {
        case LibLevel::LOW_LEVEL: {
            lowLevelLibManager->load(_path);
            break;
        }
        case LibLevel::MID_LEVEL: {
            modManager->load(_path);
            break;
        }
        case LibLevel::HIGH_LEVEL: {
            throw std::runtime_error("not implemented");
        }
        case LibLevel::ANY_LEVEL: {
            switch (getFileTypeFromPath(_path.string())) {
                case FileType::Hex:
                case FileType::Json:
                case FileType::Obj: {
                    load(_path, LibLevel::LOW_LEVEL);
                    break;
                }

                case FileType::C:
                case FileType::Cpp:
                case FileType::So: {
                    load(_path, LibLevel::MID_LEVEL);
                    break;
                }

                case FileType::Onnx: {
                    load(_path, LibLevel::HIGH_LEVEL);
                    break;
                }
            }

            break;
        }
    }
}

//-------------------------------------------------------------------------------------
std::vector<std::unique_ptr<LowLevelFunctionInfo>>& LibManager::stickyFunctionsToLoad() {
    return lowLevelLibManager->stickyFunctionsToLoad();
}

//-------------------------------------------------------------------------------------
void LibManager::runMidLevel(
    const ModFunctionInfo& _function, std::vector<std::any> _args) {
    modManager->run(_function, _args);
}

//-------------------------------------------------------------------------------------
