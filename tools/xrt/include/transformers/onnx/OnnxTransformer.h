//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <onnx/defs/shape_inference.h>
#include <onnx/onnx_pb.h>
#include <onnx/shape_inference/implementation.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/onnx/OnnxRuntime.h>
#include <string>
//-------------------------------------------------------------------------------------

class OnnxTransformer : public Transformer {
    onnx::GraphProto graph;
    OnnxRuntime *onnxRuntime;
public:
    OnnxTransformer(DirectTransformer *_directTransformer);

    ~OnnxTransformer() override;

    void load(const std::string& _path);

    void run(const std::string& _name);
};
//-------------------------------------------------------------------------------------
