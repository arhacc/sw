//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Reader.hpp>
#include <sources/mux/MuxSource.hpp>
#include <sources/net/stack/ApplicationLayer.hpp>
#include <sources/net/stack/NetworkLayer.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

#include "sockpp/tcp_socket.h"
#include <endian.h>
#include <endian/network.hpp>
#include <netinet/in.h>
#include <sys/socket.h>

class NetworkReader final : public ByteReader {
    sockpp::tcp_socket& sock;
    size_t leftToRead;

  public:
    NetworkReader(sockpp::tcp_socket& _sock, size_t _length) : sock(_sock), leftToRead(_length) {}

    ~NetworkReader() override = default;

    auto read(std::span<uint8_t> _buf) -> size_t override {
        if (leftToRead == 0) {
            return 0;
        }

        ssize_t _bytesRead{sock.read(_buf.data(), std::min(_buf.size(), leftToRead))};

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

//-------------------------------------------------------------------------------------
NetworkLayer::NetworkLayer(sockpp::tcp_socket&& _clientSocket) : clientSocket(std::move(_clientSocket)) {}

//-------------------------------------------------------------------------------------
template<>
void NetworkLayer::sendArray(std::span<const uint8_t> _values) {
    ssize_t _bytesWritten = clientSocket.write_n(_values.data(), _values.size_bytes());
    if (static_cast<size_t>(_bytesWritten) != _values.size_bytes()) {
        throw std::runtime_error("Failed to read array from socket");
    }
}

//-------------------------------------------------------------------------------------
template<>
void NetworkLayer::sendArray(std::span<const int8_t> _values) {
    sendArray(std::span<const uint8_t>{reinterpret_cast<const uint8_t*>(_values.data()), _values.size()});
}

//-------------------------------------------------------------------------------------
void NetworkLayer::closeConnection() {
    clientSocket.close();
}

std::unique_ptr<ByteReader> NetworkLayer::recieveCharStream(size_t _length) {
    return std::make_unique<NetworkReader>(clientSocket, _length);
}

//-------------------------------------------------------------------------------------

void NetworkLayer::sendCharStream(ByteReader& _reader) {
    std::array<unsigned char, BUFSIZ> _buf;

    size_t _bytesRead = _reader.read(_buf);
    while (_bytesRead > 0) {
        sendArray(std::span<const uint8_t>{_buf.data(), _bytesRead});
        _bytesRead = _reader.read(_buf);
    }
}

//-------------------------------------------------------------------------------------
