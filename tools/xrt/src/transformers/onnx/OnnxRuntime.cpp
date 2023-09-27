//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/direct/DirectTransformer.h>
#include <transformers/onnx/OnnxRuntime.h>

//-------------------------------------------------------------------------------------
OnnxRuntime::OnnxRuntime(DirectTransformer* _directTransformer) {
    directTransformer = _directTransformer;
    //  xpuL2Resolver = new XpuL2Resolver(_directTransformer);
}

//-------------------------------------------------------------------------------------
void OnnxRuntime::run(const std::string& _name) {
    //  FunctionInfo* _functionInfo = directTransformer -> resolve(_name);
    directTransformer->run(_name);
}

//-------------------------------------------------------------------------------------
