//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <sources/mux/MuxSource.hpp>
#include <sources/net/stack/ApplicationLayer.hpp>
#include <transformers/common/resourcefetcher/SdkResourceFetcher.hpp>
#include <memory>

//-------------------------------------------------------------------------------------
ApplicationLayer::ApplicationLayer(
    MuxSource& _muxSource, const Arch& _arch, sockpp::tcp_socket&& _clientSock)
    : CommandLayer(_muxSource, _arch, std::move(_clientSock)) {
    runningThread = std::make_unique<std::thread>([this]() {
        processClient();
    });
    
    _muxSource.registerFetcher(std::make_unique<SdkResourceFetcher>(*this));

    runningThread->join();
}

//-------------------------------------------------------------------------------------
ApplicationLayer::~ApplicationLayer() {
    // TODO: find out how to kill thread
}

//-------------------------------------------------------------------------------------
void ApplicationLayer::processClient() {
    for (;;) {
        int _command = receive<int>();
        printf("Command: %d\n", _command);
        processCommand(_command);
    }
}

//-------------------------------------------------------------------------------------
