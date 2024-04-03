//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/Reader.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>

FileReader::FileReader(const std::filesystem::path& _path)
    : input(std::make_unique<std::ifstream>(_path, std::ios_base::in | std::ios_base::binary)) {}

FileReader::FileReader(std::unique_ptr<std::istream> _input) : input(std::move(_input)) {}

size_t FileReader::read(std::span<uint8_t> _buf) {
    if (input->eof()) {
        return 0;
    }

    input->read(reinterpret_cast<char*>(_buf.data()), _buf.size());

    if (!input->eof() && !input->bad() && !input->fail()) {
        return _buf.size();
    } else if (input->eof() && !input->bad()) {
        return input->gcount();
    } else {
        throw std::runtime_error("error reading from file");
    }
}
