//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/Transformer.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

// forward declaration
class DirectTransformer;
class OnnxRuntime;
class ResourceLoader;
class OnnxGraph;
class OnnxOutputCache;
class MidLevelTransformer;

//-------------------------------------------------------------------------------------

class OnnxTransformer : public Transformer {
  std::shared_ptr<ResourceLoader> resourceLoader;
  std::shared_ptr<MidLevelTransformer> midLevelTransformer;

  std::unique_ptr<OnnxOutputCache> outputCache;

  std::unordered_map<Md5Hash, std::unique_ptr<OnnxGraph>, Md5Hasher> loadedGraphs;
  std::unordered_map<Md5Hash, std::unique_ptr<std::filesystem::path>, Md5Hasher> loadedTensors;

  public:
    OnnxTransformer(
        std::shared_ptr<ResourceLoader> _resourceLoader,
        std::shared_ptr<MidLevelTransformer> _midLevelTransformer
    );

    ~OnnxTransformer() override;

    void test();

    void loadGraph(const Md5Hash& _hash, const std::filesystem::path& _path);
    void loadTensor(const Md5Hash& _hash, const std::filesystem::path& _path);

    void run(
        const ResourceIdentifier& _graph,
        const std::unordered_map<std::string, ResourceIdentifier>& _inputs,
        std::unordered_map<std::string, ResourceIdentifier>& _outputs
    );
};
//-------------------------------------------------------------------------------------
