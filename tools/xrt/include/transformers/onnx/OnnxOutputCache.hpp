//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/resources/ResourceIdentifier.hpp>

#include <unordered_map>
#include <filesystem>

class OnnxOutputCache {
  std::unordered_map<Md5Hash, std::filesystem::path, Md5Hasher> _cacheEntries;

public:
  std::unordered_map<std::string, std::filesystem::path>
  prepareOutputPaths(const std::unordered_map<std::string, ResourceIdentifier>& _outputs);

  void finalizeOutputs(std::unordered_map<std::string, ResourceIdentifier>& _outputs, const std::unordered_map<std::string, std::filesystem::path>& _outputPaths);
};
