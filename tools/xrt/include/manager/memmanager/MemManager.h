//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <manager/driver/Driver.h>
//#include <transformers/common/Transformer.h>
//#include <transformers/direct/DirectTransformer.h>
//#include <transformers/onnx/OnnxTransformer.h>
//-------------------------------------------------------------------------------------
class MemManager {

public:
  MemManager(Driver* _driver);
  ~MemManager();
  void runFile(std::string _path);
  void dump(std::string _address);

  private:
  Driver* driver;
};
//-------------------------------------------------------------------------------------
