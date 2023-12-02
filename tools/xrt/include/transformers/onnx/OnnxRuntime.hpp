//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/Transformer.hpp>

#include <string>

// forward declaration
class DirectTransformer;

//-------------------------------------------------------------------------------------
class OnnxRuntime {
    DirectTransformer* directTransformer;

  public:
    OnnxRuntime(DirectTransformer* _directTransformer);

    ~OnnxRuntime() = default;

    void run(const std::string& _name);
};
//-------------------------------------------------------------------------------------
