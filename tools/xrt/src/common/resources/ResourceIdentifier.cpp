#include <common/Utils.hpp>
#include <common/resources/ResourceIdentifier.hpp>

#include <algorithm>
#include <cassert>
#include <charconv>
#include <sstream>
#include <stdexcept>
#include <string_view>

//-------------------------------------------------------------------------------------
std::string md5HashToString(const Md5Hash& _hash) {
    std::stringstream _builder;

    for (uint8_t byte : _hash) {
        _builder << fmt::format("{:02x}", byte);
    }

    return _builder.str();
}

//-------------------------------------------------------------------------------------
Md5Hash stringToMd5(std::string_view _string) {
    auto _xDigitToNumber = [](char xDigit) -> uint8_t {
        if (xDigit >= '0' && xDigit <= '9') {
            return xDigit - '0';
        } else if (xDigit >= 'a' && xDigit <= 'f') {
            return xDigit - 'a' + 10;
        } else if (xDigit >= 'A' && xDigit <= 'F') {
            return xDigit - 'A' + 10;
        } else {
            throw std::invalid_argument("non-hex digit in hex string");
        }
    };

    std::array<uint8_t, cMD5HashSize> _md5;

    if (_string.length() != 2 * cMD5HashSize) {
        throw std::invalid_argument("bad hash size");
    }

    for (size_t i = 0; i < _string.length(); i += 2) {
        _md5.at(i / 2) = (_xDigitToNumber(_string.at(i)) << 4) | (_xDigitToNumber(_string.at(i + 1)));
    }

    return _md5;
}

//-------------------------------------------------------------------------------------
std::string ResourceIdentifier::toString() const {
    std::stringstream builder;

    for (const std::string& _pkgpart : package) {
        builder << _pkgpart << ".";
    }

    builder << name;
    builder << fileTypeToString(fileType);
    builder << "@" << version.major << "." << version.minor << "." << version.patch;
    builder << "#" << md5HashToString(hash);

    return builder.str();
}

//-------------------------------------------------------------------------------------
ResourceIdentifier ResourceIdentifier::fromString(std::string_view _s) {
    ResourceIdentifier _ri;

    auto _parsePackages = [&_ri](std::string_view _name) -> void {
        auto _lastPointR = std::find(_name.rbegin(), _name.rend(), '.');
        if (_lastPointR == _name.rend()) {
            throw BadResourceIdentifierFormatException();
        }

        auto _lastPoint = (_lastPointR + 1).base();
        _ri.fileType    = fileTypeFromString({_lastPoint, _name.end()});

        _name = {_name.begin(), _lastPoint};
        do {
            auto _point = std::find(_name.begin(), _name.end(), '.');
            if (_point == _name.end()) {
                break;
            }

            std::string_view _pckg{_name.begin(), _point};

            if (_pckg.empty()) {
                throw BadResourceIdentifierFormatException();
            }

            _ri.package.emplace_back(_pckg);

            _name = {_point + 1, _name.end()};
        } while (1);

        if (_name.empty()) {
            throw BadResourceIdentifierFormatException();
        }

        _ri.name = _name;
    };

    auto _parseVersion = [&_ri](std::string_view _version) -> void {
        std::vector<std::string> _numbers{split(_version, ".")};
        if (_numbers.size() != 3) {
            throw BadResourceIdentifierFormatException();
        }

        auto parseStr = [](std::string_view s) -> unsigned {
            unsigned val;

            auto parseResult = std::from_chars(s.data(), s.data() + s.size(), val);

            if (parseResult.ec != std::errc()) {
                throw BadResourceIdentifierFormatException();
            } else if (parseResult.ptr != s.data() + s.size()) {
                throw BadResourceIdentifierFormatException();
            } else {
                return val;
            }
        };

        _ri.version.major = parseStr(_numbers[0]);
        _ri.version.minor = parseStr(_numbers[1]);
        _ri.version.patch = parseStr(_numbers[2]);
    };

    auto _at = std::find(_s.begin(), _s.end(), '@');
    if (_at == _s.end()) {
        throw BadResourceIdentifierFormatException();
    }

    _parsePackages({_s.begin(), _at});

    auto _hash = std::find(_at, _s.end(), '#');
    if (_hash == _s.end()) {
        throw BadResourceIdentifierFormatException();
    }

    _parseVersion({_at + 1, _hash});

    try {
        _ri.hash = stringToMd5({_hash + 1, _s.end()});
    } catch (std::invalid_argument&) {
        throw BadResourceIdentifierFormatException();
    }

    return _ri;
}

//-------------------------------------------------------------------------------------
std::string ResourceIdentifier::fileTypeToString(FileType _fileType) {
    switch (_fileType) {
        case FileType::Hex:
            return ".hex";
        case FileType::Onnx:
            return ".onnx";
        case FileType::Tensor:
            return ".xpu_tensor";
        case FileType::So:
            return ".so";
        default:
            throw std::runtime_error("invalid file type");
    }
}

//-------------------------------------------------------------------------------------
ResourceIdentifier::FileType ResourceIdentifier::fileTypeFromString(std::string_view _s) {
    if (_s == ".hex") {
        return FileType::Hex;
    } else if (_s == ".onnx") {
        return FileType::Onnx;
    } else if (_s == ".xpu_tensor") {
        return FileType::Tensor;
    } else if (_s == ".so") {
        return FileType::So;
    }

    throw BadResourceIdentifierFormatException();
}

//-------------------------------------------------------------------------------------
bool operator==(const ResourceIdentifier& _ri1, const ResourceIdentifier& _ri2) {
    if (_ri1.package.size() != _ri2.package.size()) {
        return false;
    }

    if (!std::equal(_ri1.package.begin(), _ri1.package.end(), _ri2.package.begin())) {
        return false;
    }

    if (_ri1.name != _ri2.name) {
        return false;
    }

    if (_ri1.fileType != _ri2.fileType) {
        return false;
    }

    if (_ri1.version != _ri2.version) {
        return false;
    }

    if (!std::equal(_ri1.hash.begin(), _ri1.hash.end(), _ri2.hash.begin())) {
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------
bool operator==(const ResourceIdentifier::Version& _v1, const ResourceIdentifier::Version& _v2) {
    return _v1.major == _v2.major && _v1.minor == _v2.minor && _v1.patch == _v2.patch;
}

//-------------------------------------------------------------------------------------
