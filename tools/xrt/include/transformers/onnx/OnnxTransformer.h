//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <onnx/defs/schema.h>
#include <onnx/defs/shape_inference.h>
#include <onnx/onnx_pb.h>
#include <onnx/defs/parser.h>
#include <onnx/shape_inference/implementation.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/onnx/OnnxRuntime.h>

//-------------------------------------------------------------------------------------

class OnnxTransformer : public Transformer {

public:
  OnnxTransformer(DirectTransformer* _directTransformer);

  ~OnnxTransformer();

  void load(std::string _path);
  void run(std::string _name);

private:
  onnx::GraphProto graph;
  OnnxRuntime *onnxRuntime;
};
//-------------------------------------------------------------------------------------
