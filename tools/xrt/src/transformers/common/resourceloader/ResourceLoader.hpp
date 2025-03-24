//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/StdlibResourceFetcher.hpp>
#include <common/resources/ResourceIdentifier.hpp>
#include <memory>
#include "transformers/common/resourcefetcher/ResourceFetcher.hpp"

struct Arch;
class Manager;
class MidLevelTransformer;
class OnnxTransformer;
class SdkResourceFetcher;

class ResourceLoader {
    StdlibResourceFetcher stdlibResourceFetcher;
    std::vector<std::unique_ptr<ResourceFetcher>> resourceFetchers;

    // TOOD PERF: store map from hash to index in this vector
    std::vector<ResourceIdentifier> loadedResources;

    // The following are called to load the resources
    // Since some of them in turn require a reference to ResourceLoader, they can not be required in the constructor.
    // The initialization sequence is as follows:
    //
    // 1. Create a std::shared_ptr<ResourceLoader>
    // 2. Create the transformers with the pointer
    // 3. Set the transformers using the ResourceLoader::set* functions
    // 4. Call ResourceLoader::loadStdlib()

    // To load lowlevel functions
    Manager* manager = nullptr;
    // To load midlevel functions
    MidLevelTransformer* midlevelTransformer = nullptr;
    // To load graphs
    OnnxTransformer* onnxTransformer = nullptr;

  public:
    ResourceLoader(const Arch& _arch);
    ~ResourceLoader() = default;

    void setManager(Manager& _m);
    void setMidlevelTransformer(MidLevelTransformer& _midlevelTransformer);
    void setOnnxTransformer(OnnxTransformer& _onnxTransformer);

    void loadStdlib();

    // load should be called on every resource and all it's dependencies before running it
    // it will not load already loaded resources, so it can be (and should be) called every time
    void load(const ResourceIdentifier& _ri, bool _plainNameLowLevel = false);

    void registerFetcher(std::unique_ptr<ResourceFetcher> _resourceFetcher);
};
