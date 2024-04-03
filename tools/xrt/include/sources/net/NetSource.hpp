//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/common/Source.hpp>
#include <sources/net/stack/ApplicationLayer.hpp>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include <netinet/in.h>
#include <sockpp/tcp_acceptor.h>
#include <sys/socket.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
class NetSource : public Source {
    MuxSource& muxSource;

    const Arch& arch;
    std::unique_ptr<Cache> cache;
    std::unique_ptr<std::thread> listenerThread;
    std::unique_ptr<sockpp::tcp_acceptor> tcpServer;

    std::vector<std::unique_ptr<ApplicationLayer>> clients;

  public:
    NetSource(MuxSource& _muxSource, const Arch& _arch, in_port_t _port);

    ~NetSource() override;

    void listen();
};
//-------------------------------------------------------------------------------------
