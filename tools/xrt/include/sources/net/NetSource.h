//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/cache/Cache.h>
#include <sources/common/Source.h>
#include <sources/mux/MuxSource.h>
#include <sources/net/stack/ApplicationLayer.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
#define SERVER_DEFAULT_LISTENING_PORT 49000

//-------------------------------------------------------------------------------------
class NetSource : public Source {
    int xpuSockfd;
    int serverStatus;
    int port;
    sockaddr_in xpuSockaddr;
    MuxSource* muxSource;
    std::vector<ApplicationLayer*> clients;

    const Arch& arch;
    Cache cache;

  public:
    NetSource(MuxSource* _muxSource, const Arch& _arch, int _port);

    ~NetSource() override;

    void startListening();

    int acceptClient();
};
//-------------------------------------------------------------------------------------
