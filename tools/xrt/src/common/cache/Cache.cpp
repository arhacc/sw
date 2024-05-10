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
#include <common/log/Logger.hpp>
#include <common/cache/Cache.hpp>
#include <array>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <cassert>

#include <common/resources/ResourceIdentifier.hpp>
#include "fmt/core.h"
#include <openssl/md5.h>
#include <openssl/evp.h>

const std::filesystem::path Cache::cachePath = getXpuHome() / "tmp" / "cache";

//-------------------------------------------------------------------------------------
void Cache::init() {
    std::filesystem::create_directories(cachePath);
    clean();
}

//-------------------------------------------------------------------------------------
void Cache::clean() {
    bool _didSomething = false;

    logInit.print(fmt::format("Cleaning cache {}\n", cachePath.string()));

    for (const auto& _directoryEntry : std::filesystem::recursive_directory_iterator(cachePath)) {
        // This follows symlinks
        if (_directoryEntry.is_regular_file()) {
            try {
                ResourceIdentifier _ri = ResourceIdentifier::fromString(_directoryEntry.path().filename().string());

                if (!isResourceHashOk(_ri)) {
                    logInit.print(fmt::format("Removing dirty cache entry {}\n", _directoryEntry.path().filename().string()));
                }
            } catch (const BadResourceIdentifierFormatException& _e) {
                logInit.print(fmt::format("WARNING: Non-Resource regular file in Cache: {}\n", _directoryEntry.path().string()));
                _didSomething = true;
            }
        }
    }

    if (!_didSomething) {
        logInit.print("Cache is clean.\n");
    } else {
        logInit.print("Done cleaning cache.\n");
    }
}

//-------------------------------------------------------------------------------------
bool Cache::isCachePath(const std::filesystem::path& _path) {
    return _path.parent_path() == cachePath;
}

//-------------------------------------------------------------------------------------
bool Cache::haveResource(const ResourceIdentifier& _ri) {
    return std::filesystem::exists(getPathOfResource(_ri));
}

//-------------------------------------------------------------------------------------
// TODO LATER: Directories for packages
std::filesystem::path Cache::getPathOfResource(const ResourceIdentifier& _ri) {
    return cachePath / _ri.toString();
}

//-------------------------------------------------------------------------------------
bool Cache::isResourceHashOk(const ResourceIdentifier& _ri) {
    std::filesystem::path _path = getPathOfResource(_ri);

    if (!std::filesystem::exists(_path)) {
        return false;
    }

    auto _calculatedHash = md5Hash(_path);

    return std::equal(_ri.hash.begin(), _ri.hash.end(), _calculatedHash.begin());
}

//-------------------------------------------------------------------------------------
std::array<uint8_t, cMD5HashSize> Cache::md5Hash(const std::filesystem::path& _path) {
    EVP_MD_CTX*   _openSSLContext = EVP_MD_CTX_new();
    const EVP_MD* _openSSLMD5Algorithm = EVP_md5();
    

    EVP_DigestInit_ex2(_openSSLContext, _openSSLMD5Algorithm, NULL);

    std::ifstream _file(_path, std::ios::binary | std::ios::in);
    std::array<uint8_t, BUFSIZ> _buffer;
    do {
        _file.read(reinterpret_cast<char*>(_buffer.data()), _buffer.size());
        EVP_DigestUpdate(_openSSLContext, _buffer.data(), static_cast<size_t>(_file.gcount()));
    } while (!_file.eof());

    std::array<uint8_t, cMD5HashSize> _hash;
    unsigned _reportedSize;
    EVP_DigestFinal_ex(_openSSLContext, _hash.data(), &_reportedSize);
    assert(_reportedSize == cMD5HashSize);

    EVP_MD_CTX_free(_openSSLContext);

    return _hash;
}
