//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>

#include <cassert>
#include <cstdint>

// TODO: generalize these

//-------------------------------------------------------------------------------------
static inline constexpr uint8_t
makeInstructionByte(const Arch& _arch, uint8_t _opcode, uint8_t _operand) {
    assert(_arch.INSTR_OPCODE_NR_BITS == 5);
    assert(_arch.INSTR_OPERAND_NR_BITS == 3);

    return (_opcode << 3) | (_operand & 0x7);
}

//-------------------------------------------------------------------------------------
static inline constexpr uint32_t
makeInstruction(const Arch& _arch, uint8_t _instructionByte, uint32_t _argument) {
    assert(_arch.INSTR_OPCODE_NR_BITS + _arch.INSTR_OPERAND_NR_BITS == 8);
    assert(_arch.INSTR_VALUE_NR_BITS == 24);

    return static_cast<uint32_t>(_instructionByte) << 24 | (_argument & 0xFF'FF'FF);
}

//-------------------------------------------------------------------------------------
static inline constexpr uint32_t makeJumpInstruction(
    const Arch& _arch,
    uint8_t _instructionByte,
    uint8_t _jmpFunction,
    uint8_t _valReg,
    uint16_t _value) {
    assert(_arch.INSTR_JMP_FUNCTION_NR_BITS == 6);
    assert(_arch.INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS == 3);
    assert(_arch.INSTR_JMP_VALUE_NR_BITS == 15);

    uint32_t _argument = static_cast<uint32_t>(_jmpFunction & 0x3F) << 18
                         | static_cast<uint32_t>(_valReg & 0x7) << 15
                         | static_cast<uint32_t>(_value & 0xFFF);

    return makeInstruction(_arch, _instructionByte, _argument);
}

//-------------------------------------------------------------------------------------
