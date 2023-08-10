//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/common/Target.h>

#include <cstdint>
#include <cstdio>

//-------------------------------------------------------------------------------------
void Target::writeInstructions(std::span<const uint32_t> _instructions) {
    printf("Target.writeInstructions, length=%zu\n", _instructions.size());

    for (uint32_t _instruction : _instructions) {
        writeInstruction(_instruction);
    }
}

//-------------------------------------------------------------------------------------
