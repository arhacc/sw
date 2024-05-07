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
#include <common/cache/Cache.hpp>

const std::filesystem::path Cache::cachePath = getXpuHome() / "tmp" / "cache";

//-------------------------------------------------------------------------------------
void Cache::init() {
    std::filesystem::create_directories(cachePath);
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
