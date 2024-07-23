//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <transformers/onnx/OnnxOutputCache.hpp>
#include <common/Utils.hpp>
#include <common/cache/Cache.hpp>


std::unordered_map<std::string, std::filesystem::path>
OnnxOutputCache::prepareOutputPaths(
  const std::unordered_map<std::string, ResourceIdentifier>& _outputs
) {
  std::unordered_map<std::string, std::filesystem::path> _outputPaths;

  std::filesystem::path _basePath = getPath(ResourceDirectory::OnnxOutputCacheTmpDir);

  for (const auto& [_name, _ri] : _outputs) {
    _outputPaths[_name] = _basePath / (_ri.toString() + ".undef");
  }

  return _outputPaths;
}

void
OnnxOutputCache::finalizeOutputs(
  std::unordered_map<std::string, ResourceIdentifier>& _outputs,
  const std::unordered_map<std::string, std::filesystem::path>& _outputPaths
) {
  std::unordered_map<std::string, ResourceIdentifier> _finalizedOutputs;

  std::filesystem::path _basePath = getPath(ResourceDirectory::OnnxOutputCacheTmpDir);

  for (const auto& [_name, _undefPath] : _outputPaths) {
    Md5Hash _hash = Cache::md5Hash(_undefPath);

    _outputs[_name].hash = _hash;

    std::filesystem::rename(_undefPath, _basePath / _outputs[_name].toString());
  }
}


