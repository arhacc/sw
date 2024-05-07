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

#include <onnx/defs/shape_inference.h>
#include <onnx/onnx_pb.h>
#include <onnx/shape_inference/implementation.h>

// forward declaration
class DirectTransformer;
class OnnxRuntime;

//-------------------------------------------------------------------------------------

class OnnxTransformer : public Transformer {
    onnx::GraphProto graph;
    OnnxRuntime* onnxRuntime;

  public:
    OnnxTransformer(DirectTransformer* _directTransformer);

    ~OnnxTransformer() override;

    void loadGraph(const std::filesystem::path& _path);
    void loadTensor(const std::filesystem::path& _path);

    void run(const std::string& _name);
};
//-------------------------------------------------------------------------------------
