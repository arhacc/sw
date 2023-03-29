//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <csignal>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "sources/common/Source.h"
#include "sources/mux/MuxSource.h"
#include "sources/net/stack/ApplicationLayer.h"

//-------------------------------------------------------------------------------------
#define SERVER_DEFAULT_LISTENING_PORT                   49000

//-------------------------------------------------------------------------------------
class NetSource : public Source {
    int xpuSockfd;
    int serverStatus;
    int port;
    sockaddr_in xpuSockaddr;
    MuxSource *muxSource;
    std::vector<ApplicationLayer *> clients;
public:
    NetSource(MuxSource *_muxSource, int _port);

    ~NetSource() override;

    void startListening();

    int acceptClient();
};
//-------------------------------------------------------------------------------------


