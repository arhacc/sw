//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Reader.h>
#include <sources/mux/MuxSource.h>

#include <memory>
#include <thread>

#define SERVER_STATUS_INIT    0
#define SERVER_STATUS_RUNNING 1
#define SERVER_STATUS_STOPPED 2

// We use long long because in C, long is 4 bytes insted of 8 on 32-bit systems.

//-------------------------------------------------------------------------------------
class NetworkLayer {
  protected:
    MuxSource* muxSource;
    int clientConnection;
    int clientStatus;

  public:
    static int charArrayToInt(const unsigned char* _c);

    static long long charArrayToLong(const unsigned char* _c);

    NetworkLayer(MuxSource* _muxSource, int _clientConnection);

    virtual ~NetworkLayer() = default;

    void closeConnection();

    unsigned char receiveChar();

    int receiveInt();

    long long receiveLong();

    void receiveCharArray(unsigned char* _array, int _length);

    void receiveIntArray(int* _array, int _length);

    void receiveLongArray(long long* _array, int _length);

    std::unique_ptr<ByteReader> recieveCharStream(size_t _length);

    void sendChar(unsigned char _c);

    void sendInt(int _i);

    void sendIntArray(const int* _array, int _length);
};
//-------------------------------------------------------------------------------------
