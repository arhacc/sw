//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "manager/libmanager/FunctionInfo.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <manager/libmanager/HexLibraryLoader.h>
#include <vector>
#include <common/Globals.h>
#include <common/Utils.h>

//-------------------------------------------------------------------------------------
FunctionInfo *HexLibraryLoader::resolve(const std::string &_name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()) {
        return nullptr;
    }

    return (FunctionInfo *) (&(_iterator->second));
}

//-------------------------------------------------------------------------------------
void HexLibraryLoader::load(const std::string& _path, const std::string& _optionalName) {
    std::string _name = (_optionalName != "") ? _optionalName : basename(_path);

    std::cout << "Loading hex function " << _name << std::endl;

    std::ifstream _file(_path);
    FunctionInfo _functionInfo = parseFile(_file, _name);

    std::pair<std::string, FunctionInfo> _functionEntry = {std::move(_name), std::move(_functionInfo)};
    functionMap.insert(std::move(_functionEntry));
}

//-------------------------------------------------------------------------------------
FunctionInfo HexLibraryLoader::parseFile(std::istream& _input, const std::string& _name) {
    std::vector<uint32_t> _code;
    uint32_t _instruction;

    while (_input >> std::hex >> _instruction) {
        _code.push_back(_instruction);
    }

    uint32_t* _code_ptr = new uint32_t[_code.size()];

    memcpy(_code_ptr, _code.data(), _code.size() * sizeof(uint32_t));

    return {
        .length = static_cast<uint32_t>(_code.size()),
        .name = std::move(_name),
        .address = 0xFFFFFFFF,
        .code = _code_ptr,
    };
}
//-------------------------------------------------------------------------------------
