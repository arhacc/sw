//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
//-------------------------------------------------------------------------------------

#include <common/Constants.hpp>
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/lowlevel/HexLibraryLoader.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo* HexLibraryLoader::resolve(std::string _name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()) {
        return nullptr;
    }

    return _iterator->second.get();
}

//-------------------------------------------------------------------------------------
void HexLibraryLoader::load(const std::string& _path, const std::string& _optionalName) {
    std::string _name = (_optionalName != "") ? _optionalName : getFileNameFromPath(_path);

    logInit.print(fmt::format("Loading hex function {} from file {}\n", _name, _path));

    std::ifstream _file(_path);

    if (!_file.good()) {
        throw(std::runtime_error("Failed to load hex file " + _path));
    }

    std::pair<std::string, std::unique_ptr<LowLevelFunctionInfo>> _functionEntry = {
        std::move(_name), parseFile(_file, _name)};
    functionMap.insert(std::move(_functionEntry));
}

//-------------------------------------------------------------------------------------
std::unique_ptr<LowLevelFunctionInfo> HexLibraryLoader::parseFile(std::istream& _input, const std::string& _name) {
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

    return std::make_unique<LowLevelFunctionInfo>(LowLevelFunctionInfo{
        .code    = std::move(_code),
        .name    = std::move(_name),
        .address = 0xFFFFFFFF,
    });
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
