//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
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

public:
	NetSource(MuxSource* _muxSource, int _port);
 	~NetSource();
	void startListening();
	int acceptClient();

private:
	int xpuSockfd;
	int serverStatus;
	int port;
	sockaddr_in xpuSockaddr;
	MuxSource* muxSource;
	std::vector<ApplicationLayer*> clients;
};
//-------------------------------------------------------------------------------------


