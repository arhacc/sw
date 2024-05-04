//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/net/stack/CommandLayer.hpp>

#include <sockpp/inet_address.h>
#include <sockpp/tcp_socket.h>

#include <thread>

//-------------------------------------------------------------------------------------
#define CLIENT_STATUS_STOPPED 0
#define CLIENT_STATUS_RUNNING 1

//-------------------------------------------------------------------------------------
class ApplicationLayer : public CommandLayer {
    std::unique_ptr<std::thread> runningThread;

  public:
    ApplicationLayer(MuxSource& _muxSource, const Arch& _arch, sockpp::tcp_socket&& _clientSock);

    ~ApplicationLayer() override;

    void processClient();
};
//-------------------------------------------------------------------------------------
