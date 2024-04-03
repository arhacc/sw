//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Reader.hpp>
#include <common/Utils.hpp>
#include <common/XrtException.hpp>

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include <openssl/evp.h>

class ResourceNotFoundException : XrtException {
  public:
    ResourceNotFoundException(const std::string& message) : XrtException(message, XrtErrorNumber::RESOURCE_NOT_FOUND) {}
    ResourceNotFoundException(std::string&& message)
        : XrtException(std::move(message), XrtErrorNumber::RESOURCE_NOT_FOUND) {}
};

class Cache {
  public:
    static constexpr size_t cMD5HashSize = 16;

  private:
    static const std::vector<FileType> extensionPriority;

    static const std::filesystem::path cachePath;

    EVP_MD_CTX* md5Ctx;

    static std::string md5ToString(std::span<const uint8_t, cMD5HashSize> _data);
    static std::array<uint8_t, cMD5HashSize> stringToMd5(std::string_view _string);

  public:
    Cache();
    ~Cache();

    static bool isCachePath(const std::filesystem::path& _path);

    bool needPutResource(std::string_view _filename, std::span<const uint8_t, cMD5HashSize> _md5);
    std::filesystem::path
    putResource(std::string_view _filename, std::span<const uint8_t, cMD5HashSize> _md5, ByteReader& data);
    std::array<uint8_t, cMD5HashSize> getResourceHash(std::string_view _filename);
    std::filesystem::path getResource(std::string_view _filename);
};
