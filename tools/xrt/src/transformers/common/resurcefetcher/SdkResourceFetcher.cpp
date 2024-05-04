#include <transformers/common/resourcefetcher/SdkResourceFetcher.hpp>
#include <sources/net/stack/CommandLayer.hpp>

#include <cstdint>

#include <common/XrtException.hpp>

//-------------------------------------------------------------------------------------
SdkResourceFetcher::SdkResourceFetcher(CommandLayer& _commandLayer) : commandLayer(_commandLayer) {}

//-------------------------------------------------------------------------------------
void SdkResourceFetcher::fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _path) {
    commandLayer.send<uint32_t>(COMMAND_GET_FILE);
    commandLayer.sendString(_resourceId.toString());

    uint32_t _response = commandLayer.receive<uint32_t>();
    if (_response == COMMAND_ERROR) {
        XrtErrorNumber _errorCode = static_cast<XrtErrorNumber>(commandLayer.receive<int>());
        throw XrtException("error from sdk on get file", _errorCode);
    } else if (_response != COMMAND_DONE) {
        throw XrtException("unrecognized response from sdk on COMMAND_GET_FILE", XrtErrorNumber::BAD_NET_SEQUENCE);
    }

    commandLayer.receiveFile(_path);
}

//-------------------------------------------------------------------------------------
