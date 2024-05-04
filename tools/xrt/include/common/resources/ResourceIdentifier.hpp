//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/XrtException.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/printf.h>

static constexpr size_t cMD5HashSize = 16;
typedef std::array<uint8_t, cMD5HashSize> Md5Hash;
std::string md5HashToString(const Md5Hash&);
Md5Hash stringToMd5(std::string_view _string);

class BadResourceIdentifierFormatException : XrtException {
  public:
    BadResourceIdentifierFormatException()
        : XrtException("bad resource identifier format", XrtErrorNumber::BAD_RESOURCE_IDENTIFIER) {}
};

struct ResourceIdentifier {
    struct Version {
        unsigned major;
        unsigned minor;
        unsigned patch;
    };

    enum class FileType {
        Hex,
        So,
        Onnx,
        Tensor,
    };

    static std::string fileTypeToString(FileType _fileType);
    static FileType fileTypeFromString(std::string_view _s);

    std::vector<std::string> package;
    std::string name;
    FileType fileType;

    Version version;

    Md5Hash hash;

    std::string toString() const;
    static ResourceIdentifier fromString(std::string_view);
};

bool operator==(const ResourceIdentifier& _ri1, const ResourceIdentifier& _ri2);
bool operator==(const ResourceIdentifier::Version& _v1, const ResourceIdentifier::Version& _v2);
