//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <sources/mux/MuxSource.h>
#include <sources/net/stack/ApplicationLayer.h>

//-------------------------------------------------------------------------------------
ApplicationLayer::ApplicationLayer(
    MuxSource* _muxSource, Cache& _cache, const Arch& _arch, int _clientConnection)
    : CommandLayer(_muxSource, _cache, _arch, _clientConnection) {
    muxSource        = _muxSource;
    clientConnection = _clientConnection;
    clientStatus     = CLIENT_STATUS_RUNNING;
    printf("New client: %d\n", clientConnection);
    //  commandLayer = new CommandLayer(_cmdSource, _clientConnection);
    //  processClient();
    std::thread t(&ApplicationLayer::processClient, this);
    t.detach();
    //  t.join();
}

//-------------------------------------------------------------------------------------
ApplicationLayer::~ApplicationLayer() {
    clientStatus = CLIENT_STATUS_STOPPED;
    //  delete(commandLayer);
    closeConnection();
    close(clientConnection);
}

//-------------------------------------------------------------------------------------
int ApplicationLayer::processClient() {
    printf("processClient...\n");
    while (clientStatus == CLIENT_STATUS_RUNNING) {
        int _command = receiveInt();
        printf("Command: %d\n", _command);
        processCommand(_command);
    }
    return 0;
}

//-------------------------------------------------------------------------------------
