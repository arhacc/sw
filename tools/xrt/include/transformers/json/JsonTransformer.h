//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------

#include <string>
#include "transformers/direct/DirectTransformer.h"

class JsonTransformer : public Transformer {
    DirectTransformer *directTransformer;
public:
    JsonTransformer(DirectTransformer *_directTransformer);

    ~JsonTransformer() override = default;

    void load(const std::string &_path);

    void run(const std::string &_name);
};
//-------------------------------------------------------------------------------------