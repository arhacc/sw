//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/ResourceFetcher.hpp>
#include <sources/net/stack/CommandLayer.hpp>
#include <string>
#include <string_view>

class SdkResourceFetcher final : public ResourceFetcher {
    NetworkLayer& commandLayer;
    std::string myName;

  public:
    SdkResourceFetcher(NetworkLayer& _commandLayer);
    ~SdkResourceFetcher() override = default;

    std::string_view name() const override;
    void fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) override;
};
