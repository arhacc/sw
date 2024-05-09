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
#include <fstream>
#include <stdexcept>
#include <vector>

#include "sockpp/tcp_socket.h"
#include <endian.h>
#include <endian/network.hpp>
#include <netinet/in.h>
#include <sys/socket.h>


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


//-------------------------------------------------------------------------------------

// void NetworkLayer::sendCharStream(ByteReader& _reader) {
//     std::array<unsigned char, BUFSIZ> _buf;

//     size_t _bytesRead = _reader.read(_buf);
//     while (_bytesRead > 0) {
//         sendArray(std::span<const uint8_t>{_buf.data(), _bytesRead});
//         _bytesRead = _reader.read(_buf);
//     }
// }

//-------------------------------------------------------------------------------------
void NetworkLayer::sendFile(const std::filesystem::path& _path) {
    std::ifstream _file(_path, std::ios::binary | std::ios::in);
    _file.exceptions(std::ifstream::badbit);
    
    _file.seekg(0, std::ios::end);
    uint64_t _size = _file.tellg();
    _file.seekg(0, std::ios::beg);

    send(_size);

    std::array<uint8_t, BUFSIZ> _buffer;

    do {
        _file.read(reinterpret_cast<char*>(_buffer.data()), _buffer.size());
        sendArray(std::span<const uint8_t>{_buffer.data(), static_cast<size_t>(_file.gcount())});
    } while (!_file.eof());
}

//-------------------------------------------------------------------------------------
void NetworkLayer::receiveFile(const std::filesystem::path& _path) {
    std::ofstream _file(_path, std::ios::binary | std::ios::out | std::ios::trunc);
    _file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    auto _size          = receive<uint64_t>();
    uint64_t _bytesRead = 0;
    std::array<uint8_t, BUFSIZ> _buffer;

    while (_bytesRead < _size) {
        size_t _bytesToRead = static_cast<size_t>(std::min(_size - _bytesRead, static_cast<uint64_t>(BUFSIZ)));
        receiveArray(std::span<uint8_t>(_buffer.begin(), _bytesToRead));

        _file.write(reinterpret_cast<char*>(_buffer.data()), _bytesToRead);

        _bytesRead += _bytesToRead;
    }
}

//-------------------------------------------------------------------------------------
std::string NetworkLayer::receiveString() {
    int _length = receive<int>();
    std::vector<char> _data(_length);

    receiveArray(_data);

    return {_data.begin(), _data.end()};
}

//-------------------------------------------------------------------------------------
void NetworkLayer::sendString(std::string_view _string) {
    send<uint32_t>(_string.size());

    std::span<const char> _strspn{_string.begin(), _string.end()};
    sendArray(_strspn);
}

//-------------------------------------------------------------------------------------
