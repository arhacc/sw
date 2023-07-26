//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <array>
#include <cinttypes>
#include <cstdint>
#include <span>
#include <sys/types.h>
#include <vector>

class ByteReader {
public:
    virtual ~ByteReader() = default;

    virtual size_t read(std::span<uint8_t> _buf) = 0;
};

template<typename BR, typename T, T (*Tctor)(T*)>
class Reader {
    BR byteReader;

    std::array<uint8_t, sizeof(T) - 1> incompleteObjectBuffer;
public:
    ~Reader() = default;

    size_t read(std::span<T> _buf) = 0;
};
