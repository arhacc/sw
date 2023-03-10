//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "sources/net/stack/CommandLayer.cpp"

//-------------------------------------------------------------------------------------
ApplicationLayer::ApplicationLayer(MuxSource* _muxSource, int _clientConnection) : CommandLayer(_muxSource, _clientConnection){
  muxSource = _muxSource;
  clientConnection = _clientConnection;
  clientStatus = CLIENT_STATUS_RUNNING;
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
    while(clientStatus == CLIENT_STATUS_RUNNING){
      int _command = receiveInt();
      printf("Command: %d\n", _command);
      processCommand(_command);
    }
  return 0;
}

//-------------------------------------------------------------------------------------
