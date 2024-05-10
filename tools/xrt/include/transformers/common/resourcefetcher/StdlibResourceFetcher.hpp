//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/resourcefetcher/ResourceFetcher.hpp>

#include <filesystem>
#include <string_view>

struct ResourceIdentifier;
struct Arch;

// TODO LATER: The standard library currently reports version 0.0.0,
// and does not calculate hash
class StdlibResourceFetcher final : public ResourceFetcher {
    const std::string myName = "stdlib";

    std::vector<ResourceIdentifier> resources;
    std::vector<std::filesystem::path> resourcePaths;

    void initLoadStdlibLowLevel();
    void initLoadStdlib();

    const Arch& arch;

  public:
    StdlibResourceFetcher(const Arch& _arch);
    ~StdlibResourceFetcher() override = default;

    void fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) override;
    std::string_view name() const override;
    const std::vector<ResourceIdentifier>& getResources() const;
    const std::vector<std::filesystem::path>& getResourcePaths() const;
};
