
//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/ResourceFetcher.hpp>

class OnnxOutputCacheResourceFetcher final : public ResourceFetcher {
  const std::string myName = "onnx";

  public:
    std::string_view name() const override;
    void fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) override;
};

