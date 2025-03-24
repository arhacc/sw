//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/generated/ArchConstants.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

constexpr size_t cArchIDSize = 16;

struct Arch {
    // Architecture signature location
    static constexpr unsigned IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR = 2000;
    static constexpr unsigned IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR = 2004;
    static constexpr unsigned IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR = 2008;
    static constexpr unsigned IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR = 2012;

    // Architecture signature
    std::array<uint8_t, cArchIDSize> ID;
    std::string IDString; // xpu_HEX

    // Values from configuration file
    std::array<unsigned, cArchNumConstants> constants;
    std::array<bool, cArchNumConstants> setConstants;

    // Computed values at Arch load
    uint8_t INSTRB_pload;
    uint8_t INSTRB_prun;
    uint8_t INSTRB_nop;
    uint8_t INSTRB_cjmp;
    uint8_t INSTRB_data_transfer_header;

    uint32_t INSTR_nop;
    uint32_t INSTR_chalt;
    uint32_t INSTR_send_matrix_array;
    uint32_t INSTR_get_matrix_array_wo_result_ready;
    uint32_t INSTR_get_matrix_array_w_result_ready;

    uint32_t INSTR_send_ctrl_array;
    uint32_t INSTR_get_ctrl_array_wo_result_ready;
    uint32_t INSTR_get_ctrl_array_w_result_ready;

    unsigned get(ArchConstant _constant) const;

    void set(ArchConstant _constant, unsigned _value);
};

void parseArchFile(Arch& _arch, const std::string& _path);
void parseArchFile(Arch& _arch);
