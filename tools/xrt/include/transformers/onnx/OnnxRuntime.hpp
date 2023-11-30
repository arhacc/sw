//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/direct/DirectTransformer.hpp>

//-------------------------------------------------------------------------------------
class OnnxRuntime {
    DirectTransformer* directTransformer;

  public:
    OnnxRuntime(DirectTransformer* _directTransformer);

    ~OnnxRuntime() = default;

    void run(const std::string& _name);
};
//-------------------------------------------------------------------------------------
