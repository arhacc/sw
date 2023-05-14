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
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <common/cache/md5.h>

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
std::string Cache::getResource(const std::string& _name) {
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
bool Cache::needInstallResource(const std::string& _filename, const std::string& _md5Hex) {
    for (const auto &entry : fs::directory_iterator(cachePath)) {

        if (entry.is_regular_file() && entry.path().stem() == _filename) {
            std::string _extension = entry.path().extension().string();

            if (_extension.length() < 3 || std::string_view(_extension.begin(), _extension.begin() + 3) != ".0x")
                throw std::runtime_error("corrupted cache directory contains file with no MD5 extension");

            std::string_view _oldMd5Hex(_extension.begin() + 3, _extension.end());

            return _oldMd5Hex == _md5Hex;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
void Cache::installResourceFromPath(const std::string& _originalPath) {
    std::string _md5Hash = md5FromPath(_originalPath);

    std::string _filename = fs::path(_originalPath).filename();

    if (needInstallResource(_filename, _md5Hash))
        fs::copy(_originalPath, cachePath.string() + "/" + _filename + ".0x" + _md5Hash, fs::copy_options::overwrite_existing);
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
