//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Reader.hpp>
#include <common/Utils.hpp>
#include <common/resources/ResourceIdentifier.hpp>

#include <cstdint>
#include <filesystem>
#include <vector>

#include <openssl/evp.h>

class Cache {
    static const std::vector<FileType> extensionPriority;

    static const std::filesystem::path cachePath;


  public:
    Cache() = delete;

    static void init();
    static void clean();
    static bool isCachePath(const std::filesystem::path&);
    static bool haveResource(const ResourceIdentifier&);
    static std::filesystem::path getPathOfResource(const ResourceIdentifier&);
    static bool isResourceHashOk(const ResourceIdentifier&);
    static std::array<uint8_t, cMD5HashSize> md5Hash(const std::filesystem::path&);
};
