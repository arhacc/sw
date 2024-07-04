//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <span>

#include <common/log/Logger.hpp>
#include <endian/network.hpp>
#include <sockpp/tcp_socket.h>

// forward declaration
class Cache;
class MuxSource;
class ByteReader;
struct Arch;

#define SERVER_STATUS_INIT    0
#define SERVER_STATUS_RUNNING 1
#define SERVER_STATUS_STOPPED 2

//-------------------------------------------------------------------------------------
class NetworkLayer {
  protected:
    sockpp::tcp_socket clientSocket;

  public:
    NetworkLayer(sockpp::tcp_socket&& _clientSocket);
    virtual ~NetworkLayer() = default;

    void closeConnection();

    inline std::string clinetAddress() {
        return clientSocket.address().to_string();
    }

    template<class Integer>
    Integer receive();

    template<class Integer>
    void send(Integer _i);

    template<class Integer>
    void receiveArray(std::span<Integer> _values);

    template<class Integer>
    void receiveArray(std::vector<Integer>& _values);

    template<class Integer>
    void sendArray(std::span<const Integer> _values);

    // template<>
    // void sendArray(std::span<const uint8_t> _values);

    // template<>
    // void sendArray(std::span<const int8_t> _values);

    template<class Integer>
    void sendArray(std::vector<Integer>&& _values);

    // std::unique_ptr<ByteReader> recieveCharStream(size_t _length);
    // void sendCharStream(ByteReader& _b);

    void sendFile(const std::filesystem::path& _path);
    void receiveFile(const std::filesystem::path& _path);

    std::string receiveString();
    void sendString(std::string_view _string);
};

//-------------------------------------------------------------------------------------
template<class Integer>
Integer NetworkLayer::receive() {
    static_assert(std::is_integral<Integer>::value, "Non-integer type wanted from NetworkLayer::receive().");

    std::array<unsigned char, sizeof(Integer)> _buffer;

    ssize_t _bytesRead = clientSocket.read_n(_buffer.data(), _buffer.size());
    if (_bytesRead != static_cast<ssize_t>(_buffer.size())) {
        throw std::runtime_error("Failed to read value from socket");
    }

    Integer _val;
    endian::network::get(_val, _buffer.data());

    logNet.print(fmt::format("Got {}int{}: 0x{:x}\n", std::is_signed_v<Integer> ? "" : "u", 8 * sizeof(Integer), _val));

    return _val;
}

//-------------------------------------------------------------------------------------
template<class Integer>
void NetworkLayer::receiveArray(std::span<Integer> _values) {
    static_assert(std::is_integral<Integer>::value, "Non-integer type wanted from NetworkLayer::receiveArray().");

    ssize_t _bytesRead = clientSocket.read_n(_values.data(), _values.size_bytes());
    if (static_cast<size_t>(_bytesRead) != _values.size_bytes()) {
        throw std::runtime_error("Failed to read value from socket");
    }

    if constexpr (sizeof(Integer) > 1) {
        for (Integer& _val : _values) {
            Integer _tmp;
            endian::network::get(_tmp, (unsigned char*) &_val);
            _val = _tmp;
        }
    }

    logNet.print(fmt::format("Got {}int{} array of size {}: [ ", std::is_signed_v<Integer> ? "" : "u", 8 * sizeof(Integer), _values.size()));

    for (const Integer &_val : _values) {
	logNet.print(fmt::format("0x{:x} ", _val));
    }

    logNet.print("]\n");
}

//-------------------------------------------------------------------------------------
template<class Integer>
void NetworkLayer::receiveArray(std::vector<Integer>& _values) {
    std::span<Integer> _valuesSpan{_values};

    receiveArray(_valuesSpan);
}

//-------------------------------------------------------------------------------------
template<class Integer>
void NetworkLayer::send(Integer _i) {
    static_assert(std::is_integral<Integer>::value, "Non-integer type sent to NetworkLayer::send().");

    std::array<unsigned char, sizeof(Integer)> _buffer;

    endian::network::put(_i, _buffer.data());

    ssize_t _bytesWritten = clientSocket.write_n(_buffer.data(), _buffer.size());
    if (_bytesWritten != static_cast<ssize_t>(_buffer.size())) {
        throw std::runtime_error("Failed to write value to socket");
    }

    logNet.print(fmt::format("Sent {}int{}: 0x{:x}\n", std::is_signed_v<Integer> ? "" : "u", 8 * sizeof(Integer), _i));
}

//-------------------------------------------------------------------------------------
template<class Integer>
void NetworkLayer::sendArray(std::span<const Integer> _values) {
    std::vector<Integer> _valuesCopy{_values.begin(), _values.end()};

    sendArray(std::move(_valuesCopy));
}

//-------------------------------------------------------------------------------------
template<class Integer>
void NetworkLayer::sendArray(std::vector<Integer>&& _values) {
    if constexpr (sizeof(Integer) > 1) {
        for (Integer& _val : _values) {
            Integer _tmp;
            endian::network::put(_val, reinterpret_cast<uint8_t*>(&_tmp));
            _val = _tmp;
        }
    }

    sendArray(
        std::span<const uint8_t>{reinterpret_cast<const uint8_t*>(_values.data()), _values.size() * sizeof(Integer)});
}

//-------------------------------------------------------------------------------------
