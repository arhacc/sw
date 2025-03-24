//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <filesystem>
#include <vector>
#include <unordered_map>

class OnnxTensor;
class OnnxNode;
class OnnxExecutionContext;

class OnnxGraph {
  std::vector<std::shared_ptr<OnnxTensor>> inputs;
  std::vector<std::unique_ptr<OnnxNode>> nodes; // in execution order
  std::unordered_map<std::string, std::shared_ptr<OnnxTensor>> tensors;

public:
  static std::unique_ptr<OnnxGraph> parse(const std::filesystem::path&);
  static std::unique_ptr<OnnxGraph> parseJson(const std::filesystem::path&);

  void run(
      OnnxExecutionContext &_onnxExecutionContext,
      const std::unordered_map<std::string, std::filesystem::path> &_inputs,
      const std::unordered_map<std::string, std::filesystem::path> &_outputs
  );
};

