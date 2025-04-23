//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------

#include <common/Constants.hpp>
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <manager/libmanager/HexLibraryLoader.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------
std::unique_ptr<LowLevelFunctionInfo> HexLibraryLoader::load(const std::filesystem::path& _path, std::string_view _name) {
    logInit.println<InfoMedium>("Loading hex function {} from file {}", _name, _path.string());

    std::ifstream _file(_path);

    if (!_file.good()) {
        throw(std::runtime_error("Failed to open hex file " + _path.string()));
    }

    return parseFile(_file, _name);
}

//-------------------------------------------------------------------------------------
std::unique_ptr<LowLevelFunctionInfo> HexLibraryLoader::parseFile(std::istream& _input, std::string_view _name) {
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
        .name    = std::string(_name),
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
