//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <string>

struct FreeSpace {
    uint32_t address; /// starting point of space
    uint32_t length;  /// length of space

    inline
    bool operator<(FreeSpace& fs2) {
        return this->length < fs2.length;
    }
};
