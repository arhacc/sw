//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "common/Utils.h"
#include <array>
#include <common/arch/Arch.hpp>
#include <common/CodeGen.h>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <string>
#include <istream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

const fs::path cArchDirectory = fs::path(getXpuHome()) / "xrt/arch"; 
const std::string cDefaultArch = "xpu_46FA4B1F360DA9A43C262193294DC4CD";

//-------------------------------------------------------------------------------------
std::unique_ptr<Arch> parseArchFile() {
    std::cout << "Defaulting to architecture " << cDefaultArch << std::endl;
    
    return parseArchFile(cDefaultArch);
}

//-------------------------------------------------------------------------------------
static
uint8_t parseHexDigit(char _c) {
    if (_c >= '0' && _c <= '9')
        return _c - '0';
    else if (_c >= 'A' && _c <= 'F')
        return _c - 'A' + 10;
    else
        throw std::runtime_error("Xpu architecture id contains a non-hexadecimal character");
}

//-------------------------------------------------------------------------------------
static
void parseLines(std::istream& _in, std::unordered_map<std::string_view, unsigned*>& _wantedConfigs) {
    size_t _configsSet = 0;

    while (_in.good() && !_in.eof()) {
        std::string _line;
        std::getline(_in, _line);

        std::stringstream _ss(_line + "\n");

        std::string _config;
        unsigned _value;

        _ss >> _config >> _value;

        // skip empty lines:
        // also skips other things -- TODO: better verification
        if (!_ss.good())
            continue;

        // check if config is wanted
        auto _configIt = _wantedConfigs.find(_config);

        if (_configIt == _wantedConfigs.end())
            continue;

        // set config if wanted
        *_configIt->second = _value;
        _configsSet++;
    }

    if (_configsSet != _wantedConfigs.size())
        throw std::runtime_error("missing wanted configs");
}

//-------------------------------------------------------------------------------------
std::array<uint8_t, cArchIDSize>
parseAndCompleteArchPath(fs::path& _path) {
    if (!_path.has_parent_path()) {
        _path += ".def";
        _path = cArchDirectory / _path;
    }

    std::string _stem = _path.stem();

    if (_stem.size() != cArchIDSize * 2 + 4 // 2 hex digits per byte + 4 chars for "xpu_"
        || std::string_view(_stem.begin(), _stem.begin() + 4) != "xpu_") {

        throw std::runtime_error("Architecture names must be of form xpu_<ID>");
    }

    std::cout << "Loading architectures for " << _stem << " from file " << _path << std::endl;

    std::array<uint8_t, cArchIDSize> _id;

    auto _strIt = _stem.begin() + 4;

    for (size_t _i = 0; _i < cArchIDSize; ++_i) {
        _id[_i] = parseHexDigit(*_strIt++);
        _id[_i] = _id[_i] << 4 | parseHexDigit(*_strIt++);
    }

    return _id;
}

//-------------------------------------------------------------------------------------
std::unique_ptr<Arch> parseArchFile(const std::string& _pathStr) {
    std::unique_ptr<Arch> _arch = std::make_unique<Arch>();

    fs::path _path(_pathStr);

    _arch->ID = parseAndCompleteArchPath(_path);
    _arch->IDString = _path.stem().string();

    std::unordered_map<std::string_view, unsigned *> _wantedConfigs{
        {"CONTROLLER_INSTR_MEM_SIZE", &_arch->CONTROLLER_INSTR_MEM_SIZE},
        {"ISA_pload", &_arch->ISA_pload},
        {"ISA_prun", &_arch->ISA_prun},
        {"ISA_ctl", &_arch->ISA_ctl},
        {"ISA_val", &_arch->ISA_val},
        {"ISA_jmp", &_arch->ISA_jmp},
        {"ISA_trun", &_arch->ISA_trun},
        {"ISA_bwor", &_arch->ISA_bwor},
        {"INSTR_OPCODE_NR_BITS", &_arch->INSTR_OPCODE_NR_BITS},
        {"INSTR_OPERAND_NR_BITS", &_arch->INSTR_OPERAND_NR_BITS},
        {"INSTR_VALUE_NR_BITS", &_arch->INSTR_VALUE_NR_BITS},
        {"INSTR_JMP_FUNCTION_NR_BITS", &_arch->INSTR_JMP_FUNCTION_NR_BITS},
        {"INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS", &_arch->INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS},
        {"INSTR_JMP_VALUE_NR_BITS", &_arch->INSTR_JMP_VALUE_NR_BITS},
        {"INSTR_JMP_FUNCTION_JMP", &_arch->INSTR_JMP_FUNCTION_JMP},
        {"INSTR_TRANSFER_ARRAY_MEM_IN", &_arch->INSTR_TRANSFER_ARRAY_MEM_IN},
        {"INSTR_TRANSFER_VALUE_LOC_LOWER", &_arch->INSTR_TRANSFER_VALUE_LOC_LOWER},
        {"INSTR_TRANSFER_ARRAY_MEM_OUT_wo_RESULT_READY", &_arch->INSTR_TRANSFER_ARRAY_MEM_OUT_wo_RESULT_READY},
        {"INSTR_TRANSFER_ARRAY_MEM_OUT_w_RESULT_READY", &_arch->INSTR_TRANSFER_ARRAY_MEM_OUT_w_RESULT_READY},
    };

    std::ifstream _in(_path);

    if (!_in.good())
        throw std::runtime_error("\n\tArchitecture file " + _path.string() + " not found\n"
                                 "\tPlease install the architecture file there or provide the full path");

    parseLines(_in, _wantedConfigs);

    _arch->INSTRB_pload = makeInstructionByte(*_arch, _arch->ISA_pload, _arch->ISA_ctl);
    _arch->INSTRB_prun  = makeInstructionByte(*_arch, _arch->ISA_prun, _arch->ISA_ctl);
    _arch->INSTRB_nop   = makeInstructionByte(*_arch, _arch->ISA_bwor, _arch->ISA_val);
    _arch->INSTR_nop = makeInstruction(*_arch, _arch->INSTRB_nop, 0);
    _arch->INSTRB_cjmp = makeInstructionByte(*_arch, _arch->ISA_jmp, _arch->ISA_ctl);
    _arch->INSTR_chalt = makeJumpInstruction(*_arch, _arch->INSTRB_cjmp, _arch->INSTR_JMP_FUNCTION_JMP, 0, 0);
    _arch->INSTRB_send_matrix_array_header = makeInstructionByte(*_arch, _arch->ISA_trun, _arch->ISA_ctl);
    _arch->INSTRB_get_matrix_array_header  = makeInstructionByte(*_arch, _arch->ISA_trun, _arch->ISA_ctl);
    
    _arch->INSTR_send_matrix_array
    = makeInstruction(*_arch,
                      _arch->INSTRB_send_matrix_array_header, 
                      _arch->INSTR_TRANSFER_ARRAY_MEM_IN << _arch->INSTR_TRANSFER_VALUE_LOC_LOWER);

    _arch->INSTR_get_matrix_array_wo_result_ready
    = makeInstruction(*_arch,
                      _arch->INSTRB_get_matrix_array_header, 
                      _arch->INSTR_TRANSFER_ARRAY_MEM_OUT_wo_RESULT_READY << _arch->INSTR_TRANSFER_VALUE_LOC_LOWER);

    _arch->INSTR_get_matrix_array_w_result_ready
    = makeInstruction(*_arch,
                      _arch->INSTRB_get_matrix_array_header, 
                      _arch->INSTR_TRANSFER_ARRAY_MEM_OUT_w_RESULT_READY << _arch->INSTR_TRANSFER_VALUE_LOC_LOWER);

    return _arch;
}

//-------------------------------------------------------------------------------------
