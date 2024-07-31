//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/onnx/OnnxGraph.hpp>
#include <transformers/onnx/OnnxNode.hpp>
#include <transformers/onnx/OnnxTensor.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <common/Utils.hpp>
#include <fmt/format.h>
#include <reproc++/run.hpp>

using json = nlohmann::json;

std::unique_ptr<OnnxGraph> OnnxGraph::parse(const std::filesystem::path &_onnxPath) {
  std::filesystem::path _jsonPath = getPath(ResourceDirectory::Onnx2JsonTmpDir) / _onnxPath.filename();
  _jsonPath.replace_extension(".json");

  std::array<std::string, 5> _arguments{"onnx2json", "--input_onnx_file_path", _onnxPath.string(), "--output_json_path", _jsonPath};

  int _status = -1;
  std::error_code _ec;

  std::tie(_status, _ec) = reproc::run(_arguments);

  if (_ec) {
    throw std::runtime_error(fmt::format("Error running onnx2json: {}", _ec.message()));
  }

  if (_status != 0) {
    throw std::runtime_error(fmt::format("Error running onnx2json: status code {}", _status));
  }

  return parseJson(_jsonPath);
}

std::unique_ptr<OnnxGraph> OnnxGraph::parseJson(const std::filesystem::path &_path) {
  std::ifstream _file(_path);
  auto _json = json::parse(_file);

  auto _graph = std::make_unique<OnnxGraph>();

  // collect all nodes and tensors
  size_t _counter = 0;
  for (const auto& _jsonNode : _json["graph"]["node"]) {
    std::string _nodeName = _jsonNode.count("name") ? std::string(_jsonNode["name"]) : fmt::format("UnnamedNode{}", _counter++);
    _graph->nodes.push_back(std::make_unique<OnnxNode>(_nodeName, std::string(_jsonNode["opType"])));

    for (const auto& _inputName : _jsonNode["input"]) {
      auto [it, _] =  _graph->tensors.try_emplace(std::string(_inputName), std::make_shared<OnnxTensor>(std::string(_inputName)));
      _graph->nodes.back()->addInput(it->second);
    }

    for (const auto& _outputName : _jsonNode["output"]) {
      auto [it, _] = _graph->tensors.try_emplace(std::string(_outputName), std::make_shared<OnnxTensor>(std::string(_outputName)));
      _graph->nodes.back()->addOutput(it->second);
    }
  }

  // sort nodes in execution order
  // TODO PERF: as it stands, this is the world's most inefficient algorithm
  std::sort(_graph->nodes.begin(), _graph->nodes.end(),
    [&](const std::unique_ptr<OnnxNode> &_node1, const std::unique_ptr<OnnxNode> &_node2) -> bool {
      auto _sameTensor = [&](const std::shared_ptr<OnnxTensor> &_tensor1, const std::shared_ptr<OnnxTensor> &_tensor2) -> bool {
        return _tensor1->getName() == _tensor2->getName();
      };

      auto _sameTensorAs = [&](const std::shared_ptr<OnnxTensor> &_tensor) -> auto {
        return [&](const std::shared_ptr<OnnxTensor> &_t) -> bool {
          return _sameTensor(_tensor, _t);
        };
      };

      std::function<bool(const std::unique_ptr<OnnxNode> &, const std::unique_ptr<OnnxNode>&)> _traverseUp = [&](const std::unique_ptr<OnnxNode> &_currentNode, const std::unique_ptr<OnnxNode> &_needle) -> bool {
        for (const auto& _input : _currentNode->getInputs()) {
          if (std::find_if(_needle->getOutputs().begin(), _needle->getOutputs().end(), _sameTensorAs(_input)) != _needle->getOutputs().end()) {
            return true;
          }

          for (const auto &_node : _graph->nodes) {
            if (std::find_if(_node->getOutputs().begin(), _node->getOutputs().end(), _sameTensorAs(_input)) != _node->getOutputs().end()) {
              if (_traverseUp(_node, _needle)) {
                return true;
              }
            }
          }
        }

        return false;
      };

      return _traverseUp(_node1, _node2);
    }
  );

  // obtain tensor type and dimension data for inputs
  for (const auto &_input : _json["graph"]["input"]) {
    auto _type = static_cast<OnnxTensor::Type>(_input["type"]["tensorType"]["elemType"]);

    OnnxTensor::Dimensions _dimensions;
    for (const auto &_dim : _input["type"]["tensorType"]["shape"]["dim"]) {
      _dimensions.push_back(std::stoll(std::string(_dim["dimValue"])));
    }

    auto &_tensor = _graph->tensors[_input["name"]];
    _tensor->setTypeDim(_type, _dimensions);
  }

  // obtain tensor type and dimensions data for the other tensors
  for (const auto &_node : _graph->nodes) {
    _node->setOutputTensorsTypeDim();
  }

  return _graph;
}


void OnnxGraph::run(
  OnnxExecutionContext &_executionContext,
  const std::unordered_map<std::string, std::filesystem::path> &_inputs,
  const std::unordered_map<std::string, std::filesystem::path> &_outputs
) {
  // TODO: Check position
  for (const auto& [_inputName, _inputPath] : _inputs) {
    tensors[_inputName]->parseData(_executionContext, _inputPath);
  }

  for (auto &_node : nodes) {
    _node->run(_executionContext);
  }

  for (const auto& [_outputName, _outputPath] : _outputs) {
    tensors[_outputName]->writeData(_executionContext, _outputPath);
  }
}
