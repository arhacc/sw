//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <transformers/onnx/XpuL2Resolver.h>

//-------------------------------------------------------------------------------------

class XpuL1OnnxRuntime {

public:
  XpuL1OnnxRuntime(DirectTransformer* _directTransformer);

  ~XpuL1OnnxRuntime();

  void run(std::string _name);

  private:
    XpuL2Resolver *xpuL2Resolver;
};
//-------------------------------------------------------------------------------------
