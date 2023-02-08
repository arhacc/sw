//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <targets/Targets.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/onnx/OnnxTransformer.h>
//-------------------------------------------------------------------------------------
class Transformers {

public:
  Transformers(Targets* _targets);
  ~Transformers();
  void runFile(std::string _path);
  void dump(std::string _address);

  private:
    DirectTransformer* directTransformer;
    OnnxTransformer* onnxTransformer;
};
//-------------------------------------------------------------------------------------
