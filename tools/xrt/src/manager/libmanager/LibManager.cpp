//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <any>
#include <iostream>
#include <cstdint>
#include <string>
#include <ostream>
#include <manager/libmanager/LibManager.h>
#include <common/Utils.h>
#include <fmt/format.h>

//-------------------------------------------------------------------------------------
LibManager::LibManager(MemManager *_memManager, const Arch& _arch) :
    arch(_arch), memManager(_memManager) {
    std::cout << "Loading libraries..." << std::endl;

    internalLibraryLoader = new InternalLibraryLoader(_arch);
    hexLibraryLoader = new HexLibraryLoader();
    jsonLibraryLoader = new JsonLibraryLoader();
}

//-------------------------------------------------------------------------------------
void LibManager::load(const std::string &_path) {
    int _fileType = getFileTypeFromGeneralPath(_path);

    std::cout << fmt::format("Loading library file {}", _path) << std::endl;

    switch (_fileType) {
        case XPU_FILE_HEX: {
            hexLibraryLoader->load(_path);
            break;
        }

        case XPU_FILE_JSON: {
            jsonLibraryLoader->load(_path);
            break;
        }

        case XPU_FILE_OBJ: {
            throw std::runtime_error("Unimplemented OBJ file loader");
            break;
        }

        case XPU_FILE_ONNX: {
            throw std::runtime_error("Unimplemented ONNX library");
            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
FunctionInfo *LibManager::resolve(const std::string &_name) {
    std::cout << "Resolving function " << _name << std::endl;

    FunctionInfo *_functionInfo = internalLibraryLoader->resolve(_name);
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

    return nullptr;
}

//-------------------------------------------------------------------------------------
std::vector<FunctionInfo>& LibManager::stickyFunctionsToLoad()
{
    return internalLibraryLoader->stickyFunctionsToLoad();
}

//-------------------------------------------------------------------------------------
