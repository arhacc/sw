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
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/printf.h>

static constexpr size_t cMD5HashSize = 16;
typedef std::array<uint8_t, cMD5HashSize> Md5Hash;
std::string md5HashToString(const Md5Hash&);
Md5Hash stringToMd5(std::string_view _string);

// For std::map and std::unordered_map
// This type is templated because the static_assert fails if not in a templated context
template<typename Int>
struct Md5HasherGeneric {
    Int operator()(const Md5Hash& a) const {
        if constexpr (sizeof(std::size_t) == 8) {
            return
                static_cast<Int>(a[0]) |
                static_cast<Int>(a[1]) << 8 |
                static_cast<Int>(a[2]) << 16 |
                static_cast<Int>(a[3]) << 24 |
                static_cast<Int>(a[4]) << 32 |
                static_cast<Int>(a[5]) << 40 |
                static_cast<Int>(a[6]) << 48 |
                static_cast<Int>(a[7]) << 56
            ;
        } else if constexpr (sizeof(std::size_t) == 4) {
            return
                static_cast<Int>(a[0]) |
                static_cast<Int>(a[1]) << 8 |
                static_cast<Int>(a[2]) << 16 |
                static_cast<Int>(a[3]) << 24
            ;
        } else {
            // This should be a static_assert but compiler on Debian 11 disagrees
            throw std::runtime_error("Md5Hasher Int size is neither 32 nor 64 bits");
        }
    }   
};

typedef Md5HasherGeneric<std::size_t> Md5Hasher;

class BadResourceIdentifierFormatException : public XrtException {
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
