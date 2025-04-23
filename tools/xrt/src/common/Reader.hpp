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
#include <filesystem>
#include <iostream>
#include <istream>
#include <span>
#include <vector>

#include <stdio.h>
#include <sys/types.h>

class ByteReader {
  public:
    virtual ~ByteReader() = default;

    virtual size_t read(std::span<uint8_t> _buf) = 0;
};

class FileReader : public ByteReader {
    std::unique_ptr<std::istream> input;

  public:
    FileReader(const std::filesystem::path& _path);
    explicit FileReader(std::unique_ptr<std::istream> _input);
    ~FileReader() override = default;

    size_t read(std::span<uint8_t> _buf) override;
};
