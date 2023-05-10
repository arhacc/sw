//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include "Architecture.h"

#include <cstdint>

// TODO: generalize these


//-------------------------------------------------------------------------------------
static inline
constexpr uint8_t makeInstructionByte(uint8_t _opcode, uint8_t _operand) {
    static_assert(XPU_ARCH_INSTR_OPCODE_NR_BITS == 5);
    static_assert(XPU_ARCH_INSTR_OPERAND_NR_BITS == 3);

    return (_opcode << 3) | (   _operand & 0x7);
}

//-------------------------------------------------------------------------------------
static inline
constexpr uint32_t makeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    static_assert(XPU_ARCH_INSTR_OPCODE_NR_BITS + XPU_ARCH_INSTR_OPERAND_NR_BITS == 8);
    static_assert(XPU_ARCH_INSTR_VALUE_NR_BITS == 24);

    return static_cast<uint32_t>(_instructionByte) << 24 | (_argument & 0xFF'FF'FF);
}

//-------------------------------------------------------------------------------------
static inline
constexpr uint32_t makeJumpInstruction(uint8_t _instructionByte, uint8_t _jmpFunction, uint8_t _valReg, uint16_t _value) {
    static_assert(XPU_ARCH_INSTR_JMP_FUNCTION_NR_BITS == 6);
    static_assert(XPU_ARCH_INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS == 3);
    static_assert(XPU_ARCH_INSTR_JMP_VALUE_NR_BITS == 15);

    uint32_t _argument = static_cast<uint32_t>(_jmpFunction & 0x3F) << 18 |
                            static_cast<uint32_t>(_valReg & 0x7) << 15 |
                            static_cast<uint32_t>(_value & 0xFFF);

    return makeInstruction(_instructionByte, _argument);
}

//-------------------------------------------------------------------------------------
static constexpr
uint8_t INSTRB_pload = makeInstructionByte(XPU_ARCH_ISA_pload, XPU_ARCH_ISA_ctl);
static constexpr
uint8_t INSTRB_prun  = makeInstructionByte(XPU_ARCH_ISA_prun, XPU_ARCH_ISA_ctl);
static constexpr
uint8_t INSTRB_nop   = makeInstructionByte(XPU_ARCH_ISA_bwor, XPU_ARCH_ISA_val);
static constexpr
uint32_t INSTR_nop = makeInstruction(INSTRB_nop, 0);


static constexpr
uint8_t INSTRB_cjmp = makeInstructionByte(XPU_ARCH_ISA_jmp, XPU_ARCH_ISA_ctl);


static constexpr
uint32_t INSTR_chalt = makeJumpInstruction(INSTRB_cjmp, XPU_ARCH_INSTR_JMP_FUNCTION_JMP, 0, 0);

//-------------------------------------------------------------------------------------


