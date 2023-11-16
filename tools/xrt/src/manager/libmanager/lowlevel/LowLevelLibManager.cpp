//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <manager/libmanager/LibErrors.hpp>
#include <manager/libmanager/lowlevel/LowLevelLibManager.hpp>

#include <any>
#include <cstdint>
#include <iostream>
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

    fmt::println("Loading library file {}", _path);

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
LowLevelFunctionInfo* LowLevelLibManager::resolve(const std::string& _name) {
    fmt::println("Resolving function {}", _name);

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

    throw FunctionNotFoundError(_name);
}

//-------------------------------------------------------------------------------------
std::vector<LowLevelFunctionInfo>& LowLevelLibManager::stickyFunctionsToLoad() {
    return internalLibraryLoader->stickyFunctionsToLoad();
}

//-------------------------------------------------------------------------------------
