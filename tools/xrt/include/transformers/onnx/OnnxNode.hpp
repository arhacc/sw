//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class OnnxExecutionContext;
class OnnxTensor;

class OnnxNode {
  std::string name;
  std::string opName;
  std::vector<std::shared_ptr<OnnxTensor>> inputs;
  std::vector<std::shared_ptr<OnnxTensor>> outputs;

public:
  OnnxNode(std::string_view _name, std::string_view _opName);

  void addInput(std::shared_ptr<OnnxTensor> _input);
  void addOutput(std::shared_ptr<OnnxTensor> _output);

  const std::vector<std::shared_ptr<OnnxTensor>>& getInputs() const;
  const std::vector<std::shared_ptr<OnnxTensor>>& getOutputs() const;
  const std::string &getName() const;
  const std::string &getOpName() const;

  void setOutputTensorsTypeDim();
  void run(OnnxExecutionContext& _executionContext);
};
