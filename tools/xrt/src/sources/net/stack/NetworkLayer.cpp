//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#include <sources/net/stack/ApplicationLayer.h>
#include <sources/net/stack/NetworkLayer.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <netinet/in.h>
#include <sys/socket.h>

class NetworkReader final : public ByteReader {
    const int fdConnection;
    size_t leftToRead;

  public:
    NetworkReader(int _fdConnection, size_t _length)
        : fdConnection(_fdConnection), leftToRead(_length) {}

    ~NetworkReader() override = default;

    size_t read(std::span<uint8_t> _buf) override {
        if (leftToRead == 0) {
            return 0;
        }

        ssize_t _bytesRead =
            ::read(fdConnection, _buf.data(), std::min(_buf.size(), leftToRead));

        if (_bytesRead < 1) {
            if (_bytesRead == 0) {
                throw std::runtime_error("unexpected connection closed");
            } else {
                throw std::runtime_error("error reading data from client connection");
            }
        }

        leftToRead -= _bytesRead;

        return _bytesRead;
    }
};

static_assert(sizeof(short) == 2, "Unexpected short size");
static_assert(sizeof(int) == 4, "Unexpected int size");
static_assert(sizeof(long long) == 8, "Unexpected long long size");

NetworkLayer::NetworkLayer(MuxSource* _muxSource, int _clientConnection) {
    muxSource        = _muxSource;
    clientConnection = _clientConnection;
}

//-------------------------------------------------------------------------------------
void NetworkLayer::closeConnection() {
    printf("Close client...\n");
    clientStatus = CLIENT_STATUS_STOPPED;
    //  sendChar(SERVER_COMMAND_DONE);
}

//-------------------------------------------------------------------------------------
unsigned char NetworkLayer::receiveChar() {
    unsigned char _buffer;
    //  int _bytesRead =
    read(clientConnection, &_buffer, 1);
    //  printf("The message was: %02x", _buffer);
    return _buffer;
}

//-------------------------------------------------------------------------------------
int NetworkLayer::receiveInt() {
    unsigned char _buffer[4];
    int _bytesRead = read(clientConnection, _buffer, 4);
    std::cout << "_bytesRead: " << _bytesRead << std::endl;
    if (_bytesRead < 4) {
        exit(1);
    }
    //  std::cout << "The message was: " << buffer;
    //  int _data =
    return charArrayToInt(_buffer);
}

//-------------------------------------------------------------------------------------
long long NetworkLayer::receiveLong() {
    unsigned char _buffer[8];
    //  int _bytesRead =
    read(clientConnection, _buffer, 8);
    //  std::cout << "The message was: " << buffer;
    //  int _data =
    return charArrayToLong(_buffer);
}

//-------------------------------------------------------------------------------------
void NetworkLayer::receiveCharArray(unsigned char* _array, int _length) {
    //  int* _buffer = new int[_length];
    for (int i = 0; i < _length; i++) {
        _array[i] = receiveChar();
    }
    //  return _buffer;
}

//-------------------------------------------------------------------------------------
void NetworkLayer::receiveIntArray(int* _array, int _length) {
    //  int* _buffer = new int[_length];
    for (int i = 0; i < _length; i++) {
        _array[i] = receiveInt();
    }
    //  return _buffer;
}

//-------------------------------------------------------------------------------------
void NetworkLayer::receiveLongArray(long long* _array, int _length) {
    //  int* _buffer = new int[2 * _length];
    for (int i = 0; i < _length; i++) {
        _array[i] = receiveLong();
    }
    //  return _buffer;
}

//-------------------------------------------------------------------------------------
std::unique_ptr<ByteReader> NetworkLayer::recieveCharStream(size_t _length) {
    return std::make_unique<NetworkReader>(clientConnection, _length);
}

// TODO: check return values and throw exceptions

//-------------------------------------------------------------------------------------
void NetworkLayer::sendChar(unsigned char _c) {
    send(clientConnection, &_c, 1, 0);
    //  return 0;
}

//-------------------------------------------------------------------------------------
void NetworkLayer::sendInt(int _i) {
    uint32_t _tmp = htonl(_i);
    write(clientConnection, &_tmp, sizeof(_tmp));
}

//-------------------------------------------------------------------------------------
void NetworkLayer::sendIntArray(const int* _array, int _length) {
    for (int i = 0; i < _length; i++) {
        sendInt(_array[i]);
    }
}

//-------------------------------------------------------------------------------------
/** length should be less than 4 (for int) **/
int NetworkLayer::charArrayToInt(const unsigned char* _c) {
    int val = 0;
    for (int i = 0; i < 4; i++) {
        val = val << 8;
        val = val | (_c[i] & 0xFF);
    }
    return val;
}

//-------------------------------------------------------------------------------------
/** length should be less than 4 (for int) **/
long long NetworkLayer::charArrayToLong(const unsigned char* _c) {
    long long val = 0;
    for (int i = 0; i < 8; i++) {
        val = val << 8;
        val = val | (_c[i] & 0xFF);
    }
    return val;
}
//-------------------------------------------------------------------------------------
