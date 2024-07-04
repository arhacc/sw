//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "common/arch/generated/ArchConstants.hpp"
#include <fmt/printf.h>
#include <magic_enum.hpp>

namespace fs = std::filesystem;

const fs::path cArchDirectory  = getPath(ResourceDirectory::ArchitectureImplementations);
const std::string cDefaultArch = "xpu_A27B92D7ACC4DDF4BD7F57F515919DBD";

//-------------------------------------------------------------------------------------
unsigned Arch::get(ArchConstant _constant) const {
    std::size_t _index = static_cast<std::size_t>(_constant);
    if (!setConstants.at(_index)) {
        throw std::runtime_error(fmt::format(
            "Accessing arch constant {} ({}), which is unset for the current "
            "architecture",
            magic_enum::enum_name(_constant),
            _index));
    }
    return constants.at(_index);
}

inline void Arch::set(ArchConstant _constant, unsigned _value) {
    std::size_t _index      = static_cast<std::size_t>(_constant);
    setConstants.at(_index) = true;
    constants.at(_index)    = _value;
}

//-------------------------------------------------------------------------------------
void parseArchFile(Arch& _arch) {
    fmt::println("Defaulting to architecture {}", cDefaultArch);

    parseArchFile(_arch, cDefaultArch);
}

//-------------------------------------------------------------------------------------
static uint8_t parseHexDigit(char _c) {
    if (_c >= '0' && _c <= '9')
        return _c - '0';
    else if (_c >= 'A' && _c <= 'F')
        return _c - 'A' + 10;
    else
        throw std::runtime_error("Xpu architecture id contains a non-hexadecimal character");
}

//-------------------------------------------------------------------------------------
static void parseLines(std::istream& _in, Arch& _arch) {
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

        // Convert string to enum value
        try {
            ArchConstant _constant = archConstantLookup.at(_config);

            _arch.set(_constant, _value);
        } catch (std::out_of_range&) {
            throw std::runtime_error(fmt::format("unknown arch constant {}", _config));
        }
    }
}

//-------------------------------------------------------------------------------------
std::array<uint8_t, cArchIDSize> parseAndCompleteArchPath(fs::path& _path) {
    if (!_path.has_parent_path()) {
        _path = cArchDirectory / _path / _path;
        _path += ".def";
    }

    std::string _stem = _path.stem();

    if (_stem.size() != cArchIDSize * 2 + 4 // 2 hex digits per byte + 4 chars for "xpu_"
        || std::string_view(_stem.begin(), _stem.begin() + 4) != "xpu_") {
        throw std::runtime_error("Architecture names must be of form xpu_<ID>");
    }

    fmt::println("Loading architectures for {} from file {}", _stem, _path.string());

    std::array<uint8_t, cArchIDSize> _id;

    auto _strIt = _stem.begin() + 4;

    for (size_t _i = 0; _i < cArchIDSize; ++_i) {
        _id[_i] = parseHexDigit(*_strIt++);
        _id[_i] = _id[_i] << 4 | parseHexDigit(*_strIt++);
    }

    return _id;
}

//-------------------------------------------------------------------------------------
void parseArchFile(Arch& _arch, const std::string& _pathStr) {
    fs::path _path(_pathStr);

    _arch.ID       = parseAndCompleteArchPath(_path);
    _arch.IDString = _path.stem().string();

    std::ifstream _in(_path);

    if (!_in.good())
        throw std::runtime_error("\n\tArchitecture file " + _path.string() + " not found\n"
                                 "\tPlease install the architecture file there or provide the full path");

    parseLines(_in, _arch);

    _arch.INSTRB_pload =
        makeInstructionByte(_arch, _arch.get(ArchConstant::ISA_pload), _arch.get(ArchConstant::ISA_ctl));
    _arch.INSTRB_prun = makeInstructionByte(_arch, _arch.get(ArchConstant::ISA_prun), _arch.get(ArchConstant::ISA_ctl));
    _arch.INSTRB_nop  = makeInstructionByte(_arch, _arch.get(ArchConstant::ISA_bwor), _arch.get(ArchConstant::ISA_val));
    _arch.INSTR_nop   = makeInstruction(_arch, _arch.INSTRB_nop, 0);
    _arch.INSTRB_cjmp = makeInstructionByte(_arch, _arch.get(ArchConstant::ISA_jmp), _arch.get(ArchConstant::ISA_ctl));
    _arch.INSTR_chalt =
        makeJumpInstruction(_arch, _arch.INSTRB_cjmp, _arch.get(ArchConstant::INSTR_JMP_FUNCTION_JMP), 0, 0);
    _arch.INSTRB_data_transfer_header =
        makeInstructionByte(_arch, _arch.get(ArchConstant::ISA_trun), _arch.get(ArchConstant::ISA_ctl));

    _arch.INSTR_send_matrix_array = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_ARRAY_MEM_IN)
            << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));

    _arch.INSTR_get_matrix_array_wo_result_ready = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_ARRAY_MEM_OUT_wo_RESULT_READY)
            << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));

    _arch.INSTR_get_matrix_array_w_result_ready = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_ARRAY_MEM_OUT_w_RESULT_READY)
            << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));

    _arch.INSTR_send_ctrl_array = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_CTRL_MEM_IN) << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));

    _arch.INSTR_get_ctrl_array_wo_result_ready = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_CTRL_MEM_OUT_wo_RESULT_READY)
            << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));

    _arch.INSTR_get_ctrl_array_w_result_ready = makeInstruction(
        _arch,
        _arch.INSTRB_data_transfer_header,
        _arch.get(ArchConstant::INSTR_TRANSFER_CTRL_MEM_OUT_w_RESULT_READY)
            << _arch.get(ArchConstant::INSTR_TRANSFER_VALUE_LOC_LOWER));
}

//-------------------------------------------------------------------------------------
