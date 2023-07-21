//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <functional>

#include <common/Reader.h>

class Cache {
    static const std::vector<int> extensionPriority;

    static const std::filesystem::path cachePath;

    static bool getResourceCompareCandidates(const std::filesystem::path& _oldCandidate,
                                             const std::filesystem::path& _newCandidate);

    static std::string md5FromPath(const std::string& _path);
    static std::string md5FromByteReader(ByteReader& _reader);

public:
    Cache();
    ~Cache() = default;
    
    static bool isCachePath(const std::string& _path);

    std::string getResourceFromName(const std::string& _name);
    std::string getResourceFromFilename(const std::string& _name);
    bool needInstallResource(const std::string& _filename, const std::string &_md5Hex);
    std::string installResource(const std::string& _filename, const std::string& _md5Hash, ByteReader& _read);
};
