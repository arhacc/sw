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

#include <filesystem>
#include <vector>

#include <openssl/evp.h>

class Cache {
    static const std::vector<FileType> extensionPriority;

    static const std::filesystem::path cachePath;

  public:
    Cache() = delete;

    static void init();
    static bool isCachePath(const std::filesystem::path&);
    static bool haveResource(const ResourceIdentifier&);
    static std::filesystem::path getPathOfResource(const ResourceIdentifier&);
};
