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

class Cache {
    static const std::vector<int> extensionPriority;

    static const std::filesystem::path cachePath;

    static bool getResourceCompareCandidates(const std::filesystem::path& _oldCandidate,
                                             const std::filesystem::path& _newCandidate);

    static std::string md5FromPath(const std::string& _path);

public:
    Cache();
    ~Cache() = default;

    std::string getResource(const std::string& _name);
    bool needInstallResource(const std::string& _filename, const std::string &_md5Hex);
    void installResourceFromPath(const std::string& _originalPath);
};
