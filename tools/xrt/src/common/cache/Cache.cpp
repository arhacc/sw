//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/cache/Cache.h>
#include <common/Utils.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <common/cache/md5.h>

#include <fmt/printf.h>

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

const fs::path Cache::cachePath = getXpuHome() + "/xrt/tmp/cache";

//-------------------------------------------------------------------------------------
Cache::Cache() {
    fs::create_directories(cachePath);
}

//-------------------------------------------------------------------------------------
bool Cache::isCachePath(const std::string& _path) {
    return fs::path(_path).parent_path() == cachePath;
}

//-------------------------------------------------------------------------------------
bool Cache::getResourceCompareCandidates(const fs::path& _oldCandidate,
                                         const fs::path& _newCandidate) {
    
    if (_oldCandidate.empty())
        return true;

    int _oldCandidateType = getFileTypeFromGeneralPath(_oldCandidate);
    int _newCandidateType = getFileTypeFromGeneralPath(_newCandidate);

    auto _oldIt = std::find(extensionPriority.begin(), extensionPriority.end(), _oldCandidateType);
    auto _newIt = std::find(extensionPriority.begin(), extensionPriority.end(), _newCandidateType);

    return _newIt < _oldIt;
}

//-------------------------------------------------------------------------------------
std::string Cache::getResourceFromName(const std::string& _name) {
    fs::path _returnCandidate;

    for (const auto &entry : fs::directory_iterator(cachePath)) {
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
    for (const auto &entry : fs::directory_iterator(cachePath)) {
        if (entry.is_regular_file() && entry.path().stem() == _name) {
            return entry.path();
        }
    }

    return "";
}

//-------------------------------------------------------------------------------------
bool Cache::needInstallResource(const std::string& _filename, const std::string& _md5Hex) {
    std::cout << "Checking for resource " << _filename << std::endl;

    for (const auto &entry : fs::directory_iterator(cachePath)) {

        if (entry.is_regular_file() && entry.path().stem() == _filename) {
            std::string _extension = entry.path().extension().string();

            if (_extension.length() < 3 || std::string_view(_extension.begin(), _extension.begin() + 3) != ".0x")
                throw std::runtime_error("corrupted cache directory contains file with no MD5 extension");

            std::string_view _oldMd5Hex(_extension.begin() + 3, _extension.end());

            return _oldMd5Hex != _md5Hex;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
std::string Cache::installResource(const std::string& _filename, const std::string& _md5Hash, std::function<size_t(std::vector<uint8_t>&)> _read) {
    std::vector<uint8_t> _buf;
    _buf.resize(BUFSIZ);

    std::string _path = cachePath.string() + "/" + _filename + ".0x" + _md5Hash;
    std::ofstream _file(_path, std::ios::out | std::ios::trunc | std::ios::binary);

    fmt::println("Installing resource {} to {}", _filename, _path);

    ssize_t _bytesRead;

    while ((_bytesRead = _read(_buf)) > 0) {
        _file.write(reinterpret_cast<const char*>(_buf.data()), _bytesRead);
    }

    return _path;
}

//-------------------------------------------------------------------------------------
static
std::string md5FromContext(const MD5Context& _md5Context);

//-------------------------------------------------------------------------------------
std::string Cache::md5FromPath(const std::string& _path) {
    std::ifstream _in(_path);

    constexpr size_t cBufferSize = 1024;
    char _buffer[cBufferSize];

    MD5Context _md5context;
    md5Init(&_md5context);

    while (_in.good() && !_in.eof()) {
        _in.read(_buffer, cBufferSize);

        md5Update(&_md5context, (uint8_t *) _buffer, _in.gcount());
    }

    md5Finalize(&_md5context);

    return md5FromContext(_md5context);
}

//-------------------------------------------------------------------------------------
static
std::string md5FromContext(const MD5Context& _md5Context) {
    std::stringstream _ss;

    for (size_t i = 0; i < 16; ++i) {
        _ss << std::hex << std::setw(2) << std::setfill('0') << (int) _md5Context.digest[i];
    }

    return _ss.str();
}

//-------------------------------------------------------------------------------------
