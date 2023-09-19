//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Reader.h>

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include <openssl/evp.h>

class Cache {
    static constexpr size_t cMD5HashSize = 16;

    static const std::vector<int> extensionPriority;

    static const std::filesystem::path cachePath;

    EVP_MD_CTX* md5Ctx;

    static bool getResourceCompareCandidates(
        const std::filesystem::path& _oldCandidate,
        const std::filesystem::path& _newCandidate);

    static std::string md5String(std::span<const uint8_t, cMD5HashSize> _data);

  public:
    Cache();
    ~Cache();

    static bool isCachePath(const std::string& _path);

    std::string getResourceFromName(const std::string& _name);
    std::string getResourceFromFilename(const std::string& _name);
    bool needInstallResource(const std::string& _filename, const std::string& _md5Hex);
    std::string installResource(
        const std::string& _filename, const std::string& _md5Hash, ByteReader& _read);
};
