#include <transformers/onnx/OnnxNode.hpp>
#include <transformers/onnx/OnnxTensor.hpp>
#include <transformers/onnx/OnnxExecutionContext.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>

#include <cassert>

OnnxNode::OnnxNode(std::string_view _name, std::string_view _opName)
  : name(_name), opName(_opName)
{}


void OnnxNode::addInput(std::shared_ptr<OnnxTensor> _input) {
  inputs.push_back(std::move(_input));
}

void OnnxNode::addOutput(std::shared_ptr<OnnxTensor> _output) {
  outputs.push_back(std::move(_output));
}

const std::vector<std::shared_ptr<OnnxTensor>>& OnnxNode::getInputs() const {
  return inputs;
}

const std::vector<std::shared_ptr<OnnxTensor>>& OnnxNode::getOutputs() const {
  return outputs;
}

const std::string& OnnxNode::getName() const {
  return name;
}

const std::string& OnnxNode::getOpName() const {
  return opName;
}

void OnnxNode::setOutputTensorsTypeDim() {
  if (opName == "MatMul") {
    // TODO: replace assert
    assert(inputs.size() == 2);
    assert(outputs.size() == 1);

    assert(inputs[0]->getType() == inputs[1]->getType());

    // TODO: generalize
    assert(inputs[0]->getDimensions().size() == 2);
    assert(inputs[1]->getDimensions().size() == 2);
    assert(inputs[0]->getDimensions()[1] == inputs[1]->getDimensions()[0]);

    outputs[0]->setTypeDim(inputs[0]->getType(), {inputs[0]->getDimensions()[0], inputs[0]->getDimensions()[0]});
  } else {
    throw std::runtime_error("!!");
  }
}

void OnnxNode::run(OnnxExecutionContext& _executionContext) {
  auto &_midLevelFunction = _executionContext.getMidLevelTransformer()->getFunction(opName);

  std::vector<void *> _midLevelData;

  for (auto &output : outputs) {
    _midLevelData.push_back(output->getDataForMidLevel(_executionContext));
  }

  for (auto &input : inputs) {
    _midLevelData.push_back(input->getDataForMidLevel(_executionContext));
  }

  _midLevelFunction->call(_midLevelData);
}

