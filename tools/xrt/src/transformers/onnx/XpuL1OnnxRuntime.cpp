//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/onnx/XpuL1OnnxRuntime.h>

//-------------------------------------------------------------------------------------
XpuL1OnnxRuntime::XpuL1OnnxRuntime(DirectTransformer* _directTransformer) {
  xpuL2Resolver = new XpuL2Resolver(_directTransformer);
}

//-------------------------------------------------------------------------------------
XpuL1OnnxRuntime::~XpuL1OnnxRuntime() {
}

//-------------------------------------------------------------------------------------
void XpuL1OnnxRuntime::run(std::string _name) {
  xpuL2Resolver -> resolve(_name);
}

//-------------------------------------------------------------------------------------
