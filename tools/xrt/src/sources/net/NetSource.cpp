//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <sources/net/NetSource.h>

//-------------------------------------------------------------------------------------
NetSource::NetSource(MuxSource* _muxSource, const Arch& _arch, int _port) : arch(_arch) {
    port         = _port;
    serverStatus = SERVER_STATUS_INIT;
    muxSource    = _muxSource;
    //  std::cout << "Loading NetSource[" << _port << "]..." << std::endl;
    //  applicationLayer = new ApplicationLayer(_cmdSource, _port);
    std::cout << "Listening @ localhost:" << port << " ..." << std::endl;
    std::thread t(&NetSource::startListening, this);
    t.detach();
    //  t.join();
}

//-------------------------------------------------------------------------------------
NetSource::~NetSource() {
    serverStatus = SERVER_STATUS_STOPPED;
    close(xpuSockfd);
    clients.clear();
}

//-------------------------------------------------------------------------------------
void NetSource::startListening() {
    // Create a socket (IPv4, TCP)
    xpuSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (xpuSockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Fixes a problem where the socket remains unusable 1-2 mins after a crash.
    // Can cause network inconsistencies in rare cases. (see issue #17)
    int enable = 1;
    int result = setsockopt(xpuSockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (result < 0) {
        std::cout << "Failed to set SO_REUSEADDR. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    xpuSockaddr.sin_family      = AF_INET;
    xpuSockaddr.sin_addr.s_addr = INADDR_ANY;
    xpuSockaddr.sin_port = htons(port); // htons is necessary to convert a number to
    // network byte order
    if (bind(xpuSockfd, (struct sockaddr*) &xpuSockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port " << port << ". errno: " << errno
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(xpuSockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    serverStatus = SERVER_STATUS_RUNNING;

    while (serverStatus == SERVER_STATUS_RUNNING) {
        int _clientConnection = acceptClient();
        //--- one connection at the time! (for now)
        //    clientStatus = CLIENT_STATUS_RUNNING;
        auto* _client = new ApplicationLayer(muxSource, cache, arch, _clientConnection);
        clients.push_back(_client);
    }
}

//-------------------------------------------------------------------------------------
int NetSource::acceptClient() {
    // Grab a connection from the queue
    int addrlen = sizeof(xpuSockaddr);
    int _connection =
        accept(xpuSockfd, (struct sockaddr*) &xpuSockaddr, (socklen_t*) &addrlen);
    if (_connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    return _connection;
}

//-------------------------------------------------------------------------------------
