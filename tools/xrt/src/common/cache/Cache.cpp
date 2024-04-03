//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Reader.hpp>
#include <common/Utils.hpp>
#include <common/XrtException.hpp>
#include <common/cache/Cache.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <bits/types/FILE.h>
#include <fmt/printf.h>
#include <openssl/evp.h>
#include <sockpp/inet_address.h>
#include <sys/types.h>

const std::filesystem::path Cache::cachePath = getXpuHome() / "tmp" / "cache";

//-------------------------------------------------------------------------------------
Cache::Cache() : md5Ctx(EVP_MD_CTX_new()) {
    std::filesystem::create_directories(cachePath);

    if (!EVP_DigestInit_ex2(md5Ctx, EVP_md5(), nullptr)) {
        throw std::runtime_error("Failed to initialize MD5 context");
    }
}

//-------------------------------------------------------------------------------------
Cache::~Cache() {
    EVP_MD_CTX_free(md5Ctx);
}

//-------------------------------------------------------------------------------------
bool Cache::isCachePath(const std::filesystem::path& _path) {
    return _path.parent_path() == cachePath;
}

//-------------------------------------------------------------------------------------
std::string Cache::md5ToString(std::span<const uint8_t, cMD5HashSize> _data) {
    std::string _md5Hash;

    for (size_t i = 0; i < cMD5HashSize; ++i) {
        fmt::format_to(std::back_inserter(_md5Hash), "{:02x}", _data[i]);
    }

    return _md5Hash;
}

//-------------------------------------------------------------------------------------
std::array<uint8_t, Cache::cMD5HashSize> Cache::stringToMd5(std::string_view _string) {
    auto _xDigitToNumber = [](char xDigit) -> uint8_t {
        if (xDigit >= '0' && xDigit <= '9') {
            return xDigit - '0';
        } else if (xDigit >= 'a' && xDigit <= 'f') {
            return xDigit - 'a' + 10;
        } else if (xDigit >= 'A' && xDigit <= 'F') {
            return xDigit - 'A' + 10;
        } else {
            throw std::runtime_error("non-hex digit in hex string");
        }
    };

    std::array<uint8_t, cMD5HashSize> _md5;

    assert(_string.length() == 2 * cMD5HashSize);

    for (size_t i = 0; i < _string.length(); i += 2) {
        assert(std::isxdigit(_string.at(i)));
        assert(std::isxdigit(_string.at(i + 1)));

        _md5.at(i / 2) = (_xDigitToNumber(_string.at(i)) << 4) | (_xDigitToNumber(_string.at(i)) << 4);
    }

    return _md5;
}

//-------------------------------------------------------------------------------------
std::filesystem::path Cache::getResource(std::string_view _filename) {
    for (const auto& entry : std::filesystem::directory_iterator(cachePath)) {
        if (entry.is_regular_file() && entry.path().stem() == _filename) {
            return entry;
        }
    }

    throw ResourceNotFoundException(fmt::format("Resource not found in cache: {}", _filename));
}

//-------------------------------------------------------------------------------------
std::array<uint8_t, Cache::cMD5HashSize> Cache::getResourceHash(std::string_view _filename) {
    std::filesystem::path _path = getResource(_filename);

    assert(_path.extension().string().size() == 2 * cMD5HashSize + 3); // 2 characters per byte (hex) + ".0x"
    assert(beginsWith(_path.extension().string(), ".0x"));

    return stringToMd5(_path.extension().string().substr(3)); // skip ".0x"
}

//-------------------------------------------------------------------------------------
bool Cache::needPutResource(std::string_view _filename, std::span<const uint8_t, cMD5HashSize> _md5) {
    try {
        std::array<uint8_t, Cache::cMD5HashSize> _oldMd5 = getResourceHash(_filename);

        return !std::equal(_md5.begin(), _md5.end(), _oldMd5.begin());
    } catch (const ResourceNotFoundException&) {
        return true;
    }
}

//-------------------------------------------------------------------------------------
std::filesystem::path
Cache::putResource(std::string_view _filename, std::span<const uint8_t, cMD5HashSize> _md5, ByteReader& _reader) {
    std::array<uint8_t, BUFSIZ> _buf;
    std::array<uint8_t, cMD5HashSize> _md5HashRecalcBytes;

    std::filesystem::path _path    = cachePath / (std::string(_filename) + ".0x" + md5ToString(_md5));
    std::filesystem::path _tmpPath = cachePath / (std::string(_filename) + ".0x" + md5ToString(_md5));
    std::ofstream _file(_tmpPath, std::ios::out | std::ios::trunc | std::ios::binary);

    if (!EVP_MD_CTX_reset(md5Ctx)) {
        throw std::runtime_error("Failed to reset MD5 context");
    }

    fmt::println("Putting resource {} at {}", _filename, _path.string());

    size_t _bytesRead;

    while ((_bytesRead = _reader.read(_buf)) > 0) {
        _file.write(reinterpret_cast<const char*>(_buf.data()), _bytesRead);

        if (!EVP_DigestUpdate(md5Ctx, _buf.data(), _bytesRead)) {
            throw std::runtime_error("Failed to update MD5 context");
        }
    }

    _file.close();

    unsigned int _md5Size = cMD5HashSize;
    if (!EVP_DigestFinal_ex(md5Ctx, _md5HashRecalcBytes.data(), &_md5Size)) {
        throw std::runtime_error("Failed to finalize MD5 context");
    }
    assert(_md5Size == cMD5HashSize);

    if (!std::equal(_md5.begin(), _md5.end(), _md5HashRecalcBytes.begin())) {
#ifdef NDEBUG
        std::filesystem::remove(_tmpPath);
#endif

        throw XrtException(
            fmt::format(
                "MD5 hash for file is {}, but {} was promised", md5ToString(_md5HashRecalcBytes), md5ToString(_md5)),
            XrtErrorNumber::BAD_MD5);
    }

    std::filesystem::rename(_tmpPath, _path);

    return _path;
}

//-------------------------------------------------------------------------------------
