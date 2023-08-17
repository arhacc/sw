//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Reader.h>
#include <common/Utils.h>
#include <common/XrtException.h>
#include <common/cache/Cache.h>

#include <algorithm>
#include <array>
#include <cassert>
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

#include <fmt/printf.h>
#include <openssl/evp.h>
#include <sys/types.h>

namespace fs = std::filesystem;

//-------------------------------------------------------------------------------------
const std::vector<int> Cache::extensionPriority{
    // High level
    XPU_FILE_ONNX,

    // Mid level
    XPU_FILE_SO, // .so MUST be above .c and .cpp
    XPU_FILE_CPP,
    XPU_FILE_C,

    // Low level
    XPU_FILE_JSON,
    XPU_FILE_HEX,
    XPU_FILE_OBJ,
};

const fs::path Cache::cachePath = getXpuHome() + "/tmp/cache";

//-------------------------------------------------------------------------------------
Cache::Cache() : md5Ctx(EVP_MD_CTX_new()) {
    fs::create_directories(cachePath);

#ifdef LIBRESSL_VERSION_NUMBER
    if (!EVP_DigestInit_ex(md5Ctx, EVP_md5(), nullptr)) {
#else
    if (!EVP_DigestInit_ex2(md5Ctx, EVP_md5(), nullptr)) {
#endif
        throw std::runtime_error("Failed to initialize MD5 context");
    }
}

//-------------------------------------------------------------------------------------
Cache::~Cache() {
    EVP_MD_CTX_free(md5Ctx);
}

//-------------------------------------------------------------------------------------
bool Cache::isCachePath(const std::string& _path) {
    return fs::path(_path).parent_path() == cachePath;
}

//-------------------------------------------------------------------------------------
bool Cache::getResourceCompareCandidates(
    const fs::path& _oldCandidate, const fs::path& _newCandidate) {
    if (_oldCandidate.empty())
        return true;

    int _oldCandidateType = getFileTypeFromGeneralPath(_oldCandidate);
    int _newCandidateType = getFileTypeFromGeneralPath(_newCandidate);

    auto _oldIt =
        std::find(extensionPriority.begin(), extensionPriority.end(), _oldCandidateType);
    auto _newIt =
        std::find(extensionPriority.begin(), extensionPriority.end(), _newCandidateType);

    return _newIt < _oldIt;
}

//-------------------------------------------------------------------------------------
std::string Cache::getResourceFromName(const std::string& _name) {
    fs::path _returnCandidate;

    for (const auto& entry : fs::directory_iterator(cachePath)) {
        if (entry.is_regular_file() && entry.path().stem().stem() == _name) {
            if (getResourceCompareCandidates(_returnCandidate, entry.path())) {
                _returnCandidate = entry.path();
            }
        }
    }

    return _returnCandidate;
}

//-------------------------------------------------------------------------------------
std::string Cache::getResourceFromFilename(const std::string& _name) {
    for (const auto& entry : fs::directory_iterator(cachePath)) {
        if (entry.is_regular_file() && entry.path().stem() == _name) {
            return entry.path();
        }
    }

    return "";
}

//-------------------------------------------------------------------------------------
bool Cache::needInstallResource(
    const std::string& _filename, const std::string& _md5Hex) {
    fmt::println("Checking for resource {}", _filename);

    for (const auto& entry : fs::directory_iterator(cachePath)) {
        if (entry.is_regular_file() && entry.path().stem() == _filename) {
            std::string _extension = entry.path().extension().string();

            if (_extension.length() < 3
                || std::string_view(_extension.begin(), _extension.begin() + 3) != ".0x")
                throw std::runtime_error(
                    "corrupted cache directory contains file with no MD5 extension");

            std::string_view _oldMd5Hex(_extension.begin() + 3, _extension.end());

            return _oldMd5Hex != _md5Hex;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
std::string Cache::installResource(
    const std::string& _filename, const std::string& _md5Hash, ByteReader& _reader) {
    std::array<uint8_t, BUFSIZ> _buf;
    std::array<uint8_t, cMD5HashSize> _md5HashRecalcBytes;

    fs::path _path    = cachePath / (_filename + ".0x" + _md5Hash);
    fs::path _tmpPath = cachePath / (_filename + ".0x" + _md5Hash + ".tmp");
    std::ofstream _file(_tmpPath, std::ios::out | std::ios::trunc | std::ios::binary);

    fmt::println("Installing resource {} to {}", _filename, _path.string());

    /*if (!EVP_MD_CTX_reset(md5Ctx)) {
        throw std::runtime_error("Failed to reset MD5 context");
    }*/

    size_t _bytesRead;

    while ((_bytesRead = _reader.read(_buf)) > 0) {
        _file.write(reinterpret_cast<const char*>(_buf.data()), _bytesRead);

        if (!EVP_DigestUpdate(md5Ctx, _buf.data(), _bytesRead)) {
            throw std::runtime_error("Failed to update MD5 context");
        }
    }

    unsigned int _md5Size;
    if (!EVP_DigestFinal_ex(md5Ctx, _md5HashRecalcBytes.data(), &_md5Size)) {
        throw std::runtime_error("Failed to finalize MD5 context");
    }
    assert(_md5Size == cMD5HashSize);

    std::string _md5HashRecalc = md5String(_md5HashRecalcBytes);

    if (_md5HashRecalc != _md5Hash) {
#ifdef NDEBUG
        std::filesystem::remove(_tmpPath);
#endif

        throw XrtException(
            fmt::format(
                "MD5 hash for file is {}, but {} was promised", _md5HashRecalc, _md5Hash),
            XrtErrorNumber::BAD_MD5);
    }

    std::filesystem::rename(_tmpPath, _path);

    return _path;
}

std::string Cache::md5String(std::span<const uint8_t, cMD5HashSize> _data) {
    std::string _md5Hash;

    for (size_t i = 0; i < cMD5HashSize; ++i) {
        fmt::format_to(std::back_inserter(_md5Hash), "{:02x}", _data[i]);
    }

    return _md5Hash;
}

//-------------------------------------------------------------------------------------
