//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/LibManager.h>

//-------------------------------------------------------------------------------------
LibManager::LibManager(const Arch& _arch, MemManager* _memManager, Manager* _manager)
    : libraryResolver(_arch),
      lowLevelLibManager(_memManager, _arch),
      modManager(_manager) {
    for (const auto& [_path, _level] : libraryResolver.getStandardLibrary()) {
        fmt::println("Loading standard library file: {}", _path.string());

        load(_path.string(), _level);
    }
}

//-------------------------------------------------------------------------------------
FunctionInfo LibManager::resolve(const std::string& _name, LibLevel _level) {
    switch (_level) {
        case LibLevel::LOW_LEVEL: {
            return lowLevelLibManager.resolve(_name);
        }
        case LibLevel::MID_LEVEL: {
            return modManager.resolve(_name);
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
void LibManager::load(const std::string& _path, LibLevel _level) {
    load(std::filesystem::path(_path), _level);
}

//-------------------------------------------------------------------------------------
void LibManager::load(const std::filesystem::path& _path, LibLevel _level) {
    switch (_level) {
        case LibLevel::LOW_LEVEL: {
            lowLevelLibManager.load(_path);
            break;
        }
        case LibLevel::MID_LEVEL: {
            modManager.load(_path);
            break;
        }
        case LibLevel::HIGH_LEVEL: {
            throw std::runtime_error("not implemented");
        }
        case LibLevel::ANY_LEVEL: {
            switch (getFileTypeFromGeneralPath(_path.string())) {
                case XPU_FILE_HEX:
                case XPU_FILE_JSON:
                case XPU_FILE_OBJ: {
                    load(_path, LibLevel::LOW_LEVEL);
                    break;
                }

                case XPU_FILE_C:
                case XPU_FILE_CPP:
                case XPU_FILE_SO: {
                    load(_path, LibLevel::MID_LEVEL);
                    break;
                }

                case XPU_FILE_ONNX: {
                    load(_path, LibLevel::HIGH_LEVEL);
                    break;
                }
            }

            break;
        }
    }
}

//-------------------------------------------------------------------------------------
