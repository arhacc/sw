//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.hpp>
#include <common/log/Logger.hpp>
#include <sources/net/NetSource.hpp>

#include <memory>
#include <stdexcept>

#include "sockpp/inet_address.h"
#include "sockpp/tcp_socket.h"
#include "sources/net/stack/ApplicationLayer.hpp"

//-------------------------------------------------------------------------------------
NetSource::NetSource(MuxSource& _muxSource, const Arch& _arch, in_port_t _port)
    : muxSource(_muxSource), arch(_arch), cache(std::make_unique<Cache>()) {
    sockpp::initialize();

    std::string _host{"0.0.0.0"};
    sockpp::inet_address _address{_host, _port};

    logInit.print(fmt::format("Listening on {}\n", _address.to_string()));

    tcpServer = std::make_unique<sockpp::tcp_acceptor>(_address);

    if (!*tcpServer) {
        throw std::runtime_error("Failed to create tcp server");
    }

    listenerThread = std::make_unique<std::thread>([this]() -> void {
        listen();
    });
    listenerThread->detach();
}

//-------------------------------------------------------------------------------------
NetSource::~NetSource() {
    tcpServer->close();
}

//-------------------------------------------------------------------------------------
void NetSource::listen() {
    for (;;) {
        try {
            sockpp::inet_address _clientAddr;
            sockpp::tcp_socket _clientSocket{tcpServer->accept(&_clientAddr)};

            if (!_clientSocket) {
                throw std::runtime_error(
                    fmt::format("Error processing incoming request: {}", tcpServer->last_error_str()));
            }

            logWork.print(fmt::format("Got connection from: {}\n", _clientAddr.to_string()));

            clients.push_back(std::make_unique<ApplicationLayer>(muxSource, *cache, arch, std::move(_clientSocket)));
        } catch (const std::exception& _e) {
            logWork.print(fmt::format("Error processing net clients: {}\n", _e.what()));
        } catch (...) {
            logWork.print(fmt::format("Unkown error processing net clients\n"));
        }
    }
}

//-------------------------------------------------------------------------------------
