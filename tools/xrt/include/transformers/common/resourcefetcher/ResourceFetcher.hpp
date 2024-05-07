//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/XrtException.hpp>
#include <common/resources/ResourceIdentifier.hpp>

#include <filesystem>

class ResourceNotFoundException : XrtException {
  public:
    ResourceNotFoundException(const std::string& message) : XrtException(message, XrtErrorNumber::RESOURCE_NOT_FOUND) {}
    ResourceNotFoundException(std::string&& message)
        : XrtException(std::move(message), XrtErrorNumber::RESOURCE_NOT_FOUND) {}
};

class ResourceFetcher {
  protected:
    ResourceFetcher() = default;
  public:
    virtual ~ResourceFetcher() = default;

    virtual void fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) = 0;
};
