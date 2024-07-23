//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/onnx/OnnxTransformer.hpp>
#include <transformers/onnx/OnnxGraph.hpp>
#include <transformers/onnx/OnnxNode.hpp>
#include <transformers/onnx/OnnxExecutionContext.hpp>
#include <transformers/onnx/OnnxOutputCache.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <common/log/Logger.hpp>


//-------------------------------------------------------------------------------------
OnnxTransformer::OnnxTransformer(
    std::shared_ptr<ResourceLoader> _resourceLoader,
    std::shared_ptr<MidLevelTransformer> _midLevelTransformer
)
  : resourceLoader(std::move(_resourceLoader)),
    midLevelTransformer(std::move(_midLevelTransformer)),
    outputCache(std::make_unique<OnnxOutputCache>())
{
}

//-------------------------------------------------------------------------------------
OnnxTransformer::~OnnxTransformer() {
}


//-------------------------------------------------------------------------------------
void OnnxTransformer::loadGraph(const Md5Hash& _hash, const std::filesystem::path& _path) {
  loadedGraphs[_hash] = OnnxGraph::parse(_path);
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::loadTensor(const Md5Hash& _hash, const std::filesystem::path& _path) {
  loadedTensors[_hash] = std::make_unique<std::filesystem::path>(_path);
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::run(
  const ResourceIdentifier& _graph,
  const std::unordered_map<std::string, ResourceIdentifier>& _inputs,
  std::unordered_map<std::string, ResourceIdentifier>& _outputs
) {
  resourceLoader->load(_graph);
  for (const auto& [_, _input] : _inputs) {
    resourceLoader->load(_input);
  }

  std::unordered_map<std::string, std::filesystem::path> _inputPaths;

  for (auto &[_inputName, _inputRi] : _inputs) {
    _inputPaths[_inputName] = *loadedTensors[_inputRi.hash];
  }

  auto _outputPaths = outputCache->prepareOutputPaths(_outputs);

  OnnxExecutionContext _executionContext(midLevelTransformer);
  loadedGraphs[_graph.hash]->run(_executionContext, _inputPaths, _outputPaths);

  outputCache->finalizeOutputs(_outputs, _outputPaths);

  logWork.print("Final ONNX outputs:\n");
  for (const auto& [_outputName, _outputRi] : _outputs) {
    logWork.print(fmt::format("|-| {} -> {}", _outputName, _outputRi.toString()));
  }
}

//-------------------------------------------------------------------------------------
