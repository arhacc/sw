
//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/ResourceFetcher.hpp>

class OnnxOutputCacheResourceFetcher final : public ResourceFetcher {

  public:
    void update();

    std::string_view name() const override;
};

