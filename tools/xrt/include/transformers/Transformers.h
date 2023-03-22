//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/Manager.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/json/JsonTransformer.h>
#include <transformers/onnx/OnnxTransformer.h>

//-------------------------------------------------------------------------------------
class Transformers {
    DirectTransformer *directTransformer;
    JsonTransformer *jsonTransformer;
    OnnxTransformer *onnxTransformer;
public:
    Transformers(Manager *_manager);

    ~Transformers();

    void load(const std::string &_path);

    void run(const std::string &_path);

    void dump(const std::string &_address);
};
//-------------------------------------------------------------------------------------
