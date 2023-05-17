//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <array>
#include <cstdint>
#include <string>

constexpr size_t cArchIDSize = 16;

struct Arch {
    // Architecture signature
    std::array<uint8_t, cArchIDSize> ID;

    // Values from configuration file
    unsigned ISA_pload;
    unsigned ISA_prun;
    unsigned ISA_ctl;
    unsigned ISA_val;
    unsigned ISA_jmp;
    unsigned ISA_trun;
    unsigned ISA_bwor;
    unsigned INSTR_OPCODE_NR_BITS;
    unsigned INSTR_OPERAND_NR_BITS;
    unsigned INSTR_VALUE_NR_BITS;
    unsigned INSTR_JMP_FUNCTION_NR_BITS;
    unsigned INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS;
    unsigned INSTR_JMP_VALUE_NR_BITS;
    unsigned INSTR_JMP_FUNCTION_JMP;
    unsigned INSTR_TRANSFER_ARRAY_MEM_IN;
    unsigned INSTR_TRANSFER_VALUE_LOC_LOWER;
    unsigned INSTR_TRANSFER_ARRAY_MEM_OUT_wo_RESULT_READY;
    unsigned INSTR_TRANSFER_ARRAY_MEM_OUT_w_RESULT_READY;

    // Computed values at Arch load
    uint8_t INSTRB_pload;
    uint8_t INSTRB_prun;
    uint8_t INSTRB_nop;
    uint8_t INSTRB_cjmp;
    uint8_t INSTRB_send_matrix_array_header;
    uint8_t INSTRB_get_matrix_array_header;

    uint32_t INSTR_nop;
    uint32_t INSTR_chalt;
    uint32_t INSTR_send_matrix_array;
    uint32_t INSTR_get_matrix_array_wo_result_ready;
    uint32_t INSTR_get_matrix_array_w_result_ready;
};

const Arch& parseArchFile(const std::string& _path);
const Arch& parseArchFile();
