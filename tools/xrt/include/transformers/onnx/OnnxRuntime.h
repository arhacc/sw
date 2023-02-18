//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <transformers/direct/DirectTransformer.h>

//-------------------------------------------------------------------------------------
class OnnxRuntime {

public:
  OnnxRuntime(DirectTransformer* _directTransformer);

  ~OnnxRuntime();

  void run(std::string _name);

  private:
  DirectTransformer* directTransformer;
};
//-------------------------------------------------------------------------------------
