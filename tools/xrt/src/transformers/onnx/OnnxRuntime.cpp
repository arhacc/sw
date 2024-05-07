//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/onnx/OnnxRuntime.hpp>

//-------------------------------------------------------------------------------------
OnnxRuntime::OnnxRuntime(DirectTransformer* _directTransformer) {
    directTransformer = _directTransformer;
    //  xpuL2Resolver = new XpuL2Resolver(_directTransformer);
}

//-------------------------------------------------------------------------------------
void OnnxRuntime::run(const std::string& _name) {
    
}

//-------------------------------------------------------------------------------------
