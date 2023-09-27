//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Globals.h>
#include <common/Utils.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/lowlevel/HexLibraryLoader.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo* HexLibraryLoader::resolve(const std::string& _name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()) {
        return nullptr;
    }

    return (LowLevelFunctionInfo*) (&(_iterator->second));
}

//-------------------------------------------------------------------------------------
void HexLibraryLoader::load(const std::string& _path, const std::string& _optionalName) {
    std::string _name =
        (_optionalName != "") ? _optionalName : getFileStemFromGeneralPath(_path);

    std::cout << fmt::format("Loading hex function {} from file {}", _name, _path)
              << std::endl;

    std::ifstream _file(_path);

    if (!_file.good()) {
        throw(std::runtime_error("Failed to load hex file " + _path));
    }

    LowLevelFunctionInfo _functionInfo = parseFile(_file, _name);

    std::pair<std::string, LowLevelFunctionInfo> _functionEntry = {
        std::move(_name), std::move(_functionInfo)};
    functionMap.insert(std::move(_functionEntry));
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo
HexLibraryLoader::parseFile(std::istream& _input, const std::string& _name) {
    std::vector<uint32_t> _code;

    while (_input.good() && !_input.eof()) {
        std::string _line;

        std::getline(_input, _line);

        if (_line.length() == 0) {
            continue;
        }

        std::array<uint32_t, 2> _lineInstructions = parseLine(_line);

        _code.push_back(_lineInstructions[0]);
        _code.push_back(_lineInstructions[1]);
    }

    uint32_t* _code_ptr = new uint32_t[_code.size()];

    std::memcpy(_code_ptr, _code.data(), _code.size() * sizeof(uint32_t));

    return {
        .length  = static_cast<uint32_t>(_code.size()),
        .name    = std::move(_name),
        .address = 0xFFFFFFFF,
        .code    = _code_ptr,
    };
}

//-------------------------------------------------------------------------------------
std::array<uint32_t, 2> HexLibraryLoader::parseLine(const std::string& _line) {
    std::stringstream _ss(_line);

    std::array<uint32_t, 2> _instructions;

    _ss >> std::hex >> _instructions[0];
    _ss.ignore(1, '_');
    _ss >> std::hex >> _instructions[1];

    return _instructions;
}

//-------------------------------------------------------------------------------------
