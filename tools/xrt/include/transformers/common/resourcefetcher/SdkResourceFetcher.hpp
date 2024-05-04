//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/ResourceFetcher.hpp>
#include <sources/net/stack/CommandLayer.hpp>

class SdkResourceFetcher final : public ResourceFetcher {
    CommandLayer& commandLayer;

  public:
    SdkResourceFetcher(CommandLayer& _commandLayer);
    ~SdkResourceFetcher() override = default;

    void fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) override;
};
