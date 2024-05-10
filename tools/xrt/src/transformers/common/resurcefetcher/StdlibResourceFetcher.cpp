#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <transformers/common/resourcefetcher/StdlibResourceFetcher.hpp>
#include <common/log/Logger.hpp>

#include <filesystem>
#include <stdexcept>
#include "common/cache/Cache.hpp"

StdlibResourceFetcher::StdlibResourceFetcher(const Arch& _arch) : arch(_arch) {
    logWork.print("Loading stanadrd library\n");

    initLoadStdlib();
}

void StdlibResourceFetcher::initLoadStdlib() {
    initLoadStdlibLowLevel();
}

void StdlibResourceFetcher::initLoadStdlibLowLevel() {
    std::filesystem::path _lowLevelLibraryPath{
        getPath(ResourceDirectory::LowLevelLibrariesPrefix) / arch.IDString
        / getPath(ResourceDirectory::LowLevelLibrariesPostfix)};

    for (const std::filesystem::path& _path : std::filesystem::directory_iterator(_lowLevelLibraryPath)) {
        if (_path.extension() == ".json" || _path.extension() == ".hex" || _path.extension() == ".obj") {
            resources.push_back(ResourceIdentifier{
                .package  = {"stdlib"},
                .name     = _path.stem(),
                .fileType = ResourceIdentifier::fileTypeFromString(_path.extension().string()),
                .version =
                    ResourceIdentifier::Version{
                        .major = 0,
                        .minor = 0,
                        .patch = 0,
                    },
                .hash = Cache::md5Hash(_path),
            });

            resourcePaths.push_back(_path);
        }
    }
}

void StdlibResourceFetcher::fetchResource(const ResourceIdentifier& _ri, const std::filesystem::path& _targetPath) {
    auto _sourceResource = std::find(resources.begin(), resources.end(), _ri);
    if (_sourceResource == resources.end()) {
        throw ResourceNotFoundException("Not in standard library");
    }
    auto _sourceIndex       = std::distance(resources.begin(), _sourceResource);
    const auto& _sourcePath = resourcePaths[_sourceIndex];

    if (std::filesystem::exists(_targetPath) && !std::filesystem::is_symlink(_targetPath)) {
        throw std::runtime_error(
            fmt::format("Want to create symlink but file at {} but file exists", _targetPath.string()));
    }

    std::filesystem::remove(_targetPath);
    std::filesystem::create_symlink(_sourcePath, _targetPath);
}

const std::vector<ResourceIdentifier>& StdlibResourceFetcher::getResources() const {
    return resources;
}

const std::vector<std::filesystem::path>& StdlibResourceFetcher::getResourcePaths() const {
    return resourcePaths;
}

std::string_view StdlibResourceFetcher::name() const {
    return myName;
}
