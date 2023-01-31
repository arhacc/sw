//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <thread>
//#include "sources/net/stack/ApplicationLayer.h"
#define SERVER_STATUS_INIT                      0
#define SERVER_STATUS_RUNNING                   1
#define SERVER_STATUS_STOPPED                   2

//#define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
//#define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

//-------------------------------------------------------------------------------------
class NetworkLayer {

public:
	NetworkLayer(MuxSource* _muxSource, int _clientConnection);
	~NetworkLayer();
	void closeConnection();

	unsigned char receiveChar();
	int receiveInt();
	long receiveLong();

	void receiveCharArray(unsigned char* _array, int _length);
	void receiveIntArray(int* _array, int _length);
	void receiveLongArray(long* _array, int _length);

	void sendChar(unsigned char _c);
	void sendInt(int _i);

	int charArrayToInt(unsigned char* _c);
	int charArrayToLong(unsigned char* _c);

protected:
	MuxSource* muxSource;
	int clientConnection;
	int clientStatus;
};
//-------------------------------------------------------------------------------------
