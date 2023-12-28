//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/lowlevel/HexLibraryLoader.hpp>
#include <manager/libmanager/lowlevel/InternalLibraryLoader.hpp>
#include <manager/libmanager/lowlevel/JsonLibraryLoader.hpp>
#include <manager/libmanager/lowlevel/LowLevelLibManager.hpp>
#include <manager/memmanager/MemManager.hpp>

#include <any>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------
LowLevelLibManager::LowLevelLibManager(MemManager* _memManager, const Arch& _arch) {
    std::cout << "Loading libraries..." << std::endl;

    internalLibraryLoader = new InternalLibraryLoader(_arch);
    hexLibraryLoader      = new HexLibraryLoader();
    jsonLibraryLoader     = new JsonLibraryLoader();
}

//-------------------------------------------------------------------------------------
void LowLevelLibManager::load(const std::string& _path) {
    FileType _fileType = getFileTypeFromPath(_path);

    switch (_fileType) {
        case FileType::Hex: {
            hexLibraryLoader->load(_path);
            break;
        }

        case FileType::Json: {
            jsonLibraryLoader->load(_path);
            break;
        }

        case FileType::Obj: {
            throw std::runtime_error("Unimplemented OBJ file loader");
            break;
        }

        case FileType::Onnx: {
            throw std::runtime_error("Unimplemented ONNX library");
            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo* LowLevelLibManager::resolve(std::string_view _name) {
    logWork.print(fmt::format("Resolving function {}\n", _name));

    LowLevelFunctionInfo* _functionInfo = internalLibraryLoader->resolve(_name);
    if (_functionInfo != nullptr) {
        return _functionInfo;
    }

    _functionInfo = hexLibraryLoader->resolve(_name);
    if (_functionInfo != nullptr) {
        return _functionInfo;
    }

    _functionInfo = jsonLibraryLoader->resolve(_name);
    if (_functionInfo != nullptr) {
        return _functionInfo;
    }

    throw FunctionNotFoundError(std::string(_name));
}

//-------------------------------------------------------------------------------------
std::vector<std::unique_ptr<LowLevelFunctionInfo>>&
LowLevelLibManager::stickyFunctionsToLoad() {
    return internalLibraryLoader->stickyFunctionsToLoad();
}

//-------------------------------------------------------------------------------------
