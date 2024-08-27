#include <common/cache/Cache.hpp>
#include <common/resources/ResourceIdentifier.hpp>
#include <manager/Manager.hpp>
#include <transformers/common/resourcefetcher/StdlibResourceFetcher.hpp>
#include <transformers/common/resourcefetcher/OnnxOutputCacheResourceFetcher.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/onnx/OnnxTransformer.hpp>
#include <memory>
#include "common/log/Logger.hpp"
#include "fmt/core.h"

ResourceLoader::ResourceLoader(const Arch& _arch) : stdlibResourceFetcher(_arch) {
    resourceFetchers.push_back(std::make_unique<OnnxOutputCacheResourceFetcher>());
}

void ResourceLoader::loadStdlib() {
    auto _stdlibResources = stdlibResourceFetcher.getResources();

    for (auto _resource : _stdlibResources) {
        load(_resource, true);
    }
}

void ResourceLoader::setManager(Manager& _manager) {
  manager = &_manager;
}

void ResourceLoader::setMidlevelTransformer(MidLevelTransformer& _midlevelTransformer) {
    midlevelTransformer = &_midlevelTransformer;
}

void ResourceLoader::setOnnxTransformer(OnnxTransformer& _onnxTransformer) {
    onnxTransformer = &_onnxTransformer;
}

void ResourceLoader::registerFetcher(std::unique_ptr<ResourceFetcher> _resourceFetcher) {
    resourceFetchers.push_back(std::move(_resourceFetcher));
}

void ResourceLoader::load(const ResourceIdentifier& _ri, bool _plainNameLowLevel) {
    // Skip already loaded resources
    auto _already = std::find(loadedResources.begin(), loadedResources.end(), _ri);
    if (_already != loadedResources.end()) {
        return;
    }

    // Get resource cache path
    auto _targetPath = Cache::getPathOfResource(_ri);

    // Resource may already exist in cache from previous invocation
    // If it doesn't we need to obtain it from a fetcher
    // In either case we then need to load it in our memory
    if (!Cache::haveResource(_ri)) {
        bool _fetchOk = false;

        // try the standard lib first
        try {
            stdlibResourceFetcher.fetchResource(_ri, _targetPath);
            _fetchOk = true;
        } catch (const ResourceNotFoundException&) {
            for (auto& _fetcher : resourceFetchers) {
                try {
                    _fetcher->fetchResource(_ri, _targetPath);

                    if (!Cache::isResourceHashOk(_ri)) {
                        logWork.print(fmt::format("WARNING: Resource fetcher {} provided resource {}, but the hash is wrong. Ignoring", _fetcher->name(), _ri.toString()));
                        std::filesystem::remove(_targetPath);
                        continue;
                    }

                    _fetchOk = true;
                } catch (const ResourceNotFoundException&) {
                    continue;
                }
            }
        }

        if (!_fetchOk) {
            throw ResourceNotFoundException("Not found resource in cache or at any fetcher");
        }
    }

    // Finally, load resource in memory.
    // If the resource has dependencies we will be called back to load them by the transformers
    switch (_ri.fileType) {
        case ResourceIdentifier::FileType::Hex: {
            assert(manager != nullptr);
            manager->loadLowLevel(_targetPath, _plainNameLowLevel ? _ri.name : _ri.toString());
            break;
        }

        case ResourceIdentifier::FileType::So: {
            assert(midlevelTransformer != nullptr);
            midlevelTransformer->loadLibrary(_targetPath);
            break;
        }

        case ResourceIdentifier::FileType::Onnx: {
            assert(onnxTransformer != nullptr);
            onnxTransformer->loadGraph(_ri.hash, _targetPath);
            break;
        }

        case ResourceIdentifier::FileType::Tensor: {
            assert(onnxTransformer != nullptr);
            onnxTransformer->loadTensor(_ri.hash, _targetPath);
            break;
        }
    }
}
