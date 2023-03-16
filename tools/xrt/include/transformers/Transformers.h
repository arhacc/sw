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

public:
  Transformers(Manager* _manager);
  ~Transformers();
  void load(std::string _path);
  void run(std::string _path);
  void dump(std::string _address);

  private:
    DirectTransformer* directTransformer;
    JsonTransformer* jsonTransformer;
    OnnxTransformer* onnxTransformer;
};
//-------------------------------------------------------------------------------------
