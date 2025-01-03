#include <transformers/common/resourcefetcher/SdkResourceFetcher.hpp>
#include <sources/net/stack/CommandLayer.hpp>
#include <common/log/Logger.hpp>

#include <cstdint>

#include <common/XrtException.hpp>

//-------------------------------------------------------------------------------------
SdkResourceFetcher::SdkResourceFetcher(NetworkLayer& _commandLayer) : commandLayer(_commandLayer), myName(fmt::format("sdk[{}]", commandLayer.clinetAddress())) {}

//-------------------------------------------------------------------------------------
void SdkResourceFetcher::fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) {
    commandLayer.send<uint32_t>(COMMAND_GET_RESOURCE);
    std::string s = _resourceId.toString();
    commandLayer.sendString(s);

    logWork.print<InfoMedium>("Fetching resource {} from SDK", s);

    uint32_t _response = commandLayer.receive<uint32_t>();
    if (_response == COMMAND_ERROR) {
        XrtErrorNumber _errorCode = static_cast<XrtErrorNumber>(commandLayer.receive<int>());
        throw XrtException("error from sdk on get file", _errorCode);
    } else if (_response != COMMAND_DONE) {
        throw XrtException(fmt::format("unrecognized response from sdk on COMMAND_GET_FILE {}", _response), XrtErrorNumber::BAD_NET_SEQUENCE);
    }

    commandLayer.receiveFile(_path);
}

//-------------------------------------------------------------------------------------
std::string_view SdkResourceFetcher::name() const {
    return myName;
}

//-------------------------------------------------------------------------------------
