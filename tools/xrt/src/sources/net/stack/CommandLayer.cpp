//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/XrtException.hpp>
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.hpp>
#include <common/log/Logger.hpp>
#include <sources/mux/MuxSource.hpp>
#include <sources/net/stack/CommandLayer.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <vector>

#include <asio.hpp>
#include <asio/basic_stream_file.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <openssl/md5.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
CommandLayer::CommandLayer(MuxSource& _muxSource, Cache& _cache, const Arch& _arch, sockpp::tcp_socket&& _socket)
    : NetworkLayer(std::move(_socket)), arch(_arch), cache(_cache), muxSource(_muxSource) {}

//-------------------------------------------------------------------------------------
int CommandLayer::processCommand(int _command) {
    logWork.print(fmt::format("Received command number: {}\n", _command));

    try {
        switch (_command) {
            case COMMAND_RESERVED: {
                break;
            }

            case COMMAND_HALT: {
                break;
            }

            case COMMAND_RESET: {
                break;
            }
            case COMMAND_CLOSE_CONNECTION: {
                //      closeConnection();
                break;
            }

            case COMMAND_DONE: {
                break;
            }

            case COMMAND_ERROR: {
                break;
            }

            case COMMAND_PUT_FILE: {
                std::filesystem::path _fullPath = receiveFile();
                muxSource.load(_fullPath);

                break;
            }

            case COMMAND_GET_FILE: {
                send<int>(COMMAND_DONE);

                break;
            }

            case COMMAND_DEBUG_ADD_BREAKPOINT: {
                std::string _functionName = receiveString();
                uint32_t _lineNumber      = receive<int>();

                uint32_t _breakpointId = muxSource.debugSetBreakpoint(_functionName, _lineNumber);

                send<int>(COMMAND_DONE);
                send(_breakpointId);

                break;
            }

            case COMMAND_DEBUG_READ_ARRAY_REGISTRY: {
                uint32_t _firstCell{receive<uint32_t>()};
                uint32_t _lastCell{receive<uint32_t>()};

                std::vector<uint32_t> _ret = muxSource.debugGetArrayRegs(_firstCell, _lastCell);

                // cast to signed
                sendArray(std::move(_ret));

                break;
            }

            case COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA: {
                uint32_t _firstCell = receive<uint32_t>();
                uint32_t _lastCell  = receive<uint32_t>();
                uint32_t _firstRow  = receive<uint32_t>();
                uint32_t _lastRow   = receive<uint32_t>();

                std::vector<uint32_t> _ret = muxSource.debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);

                // cast to signed
                sendArray(reinterpret_cast<std::vector<int>&&>(std::move(_ret)));

                break;
            }

            case COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA: {
                uint32_t _firstCell = receive<uint32_t>();
                uint32_t _lastCell  = receive<uint32_t>();
                uint32_t _firstRow  = receive<uint32_t>();
                uint32_t _lastRow   = receive<uint32_t>();

                std::vector<uint32_t> _data;
                receiveArray(_data);

                muxSource.debugPutArrayData(
                    _firstCell, _lastCell, _firstRow, _lastRow, {(uint32_t*) _data.data(), _data.size()});

                break;
            }

            case COMMAND_GET_ARCHITECTURE_ID: {
                // TODO: Implement NetworkLayer::sendCharArray

                for (uint8_t c : arch.ID) {
                    send(c);
                }

                break;
            }

            case COMMAND_PING: {
                send<int>(COMMAND_ACK);
                break;
            }

            case COMMAND_ACK: {
                break;
            }

            default: {
                throw XrtException(fmt::format("Unknown net command {}", _command), XrtErrorNumber::UNKNOWN_COMMAND);
            }
        }
    } catch (XrtException& _exception) {
        fmt::println(
            "Error processing net command {}: {} ({})", _command, _exception.what(), _exception.errorNumberInt());

        send<int>(COMMAND_ERROR);
        send<int>(_exception.errorNumberInt());
    } catch (std::exception& _exception) {
        fmt::println("Error processing net command {}: {}", _command, _exception.what());

        send<int>(COMMAND_ERROR);
        send(static_cast<int>(XrtErrorNumber::GENERIC_ERROR));
    } catch (...) {
        fmt::println("Unknown error processing net command {}", _command);

        send<int>(COMMAND_ERROR);
        send<int>(static_cast<int>(XrtErrorNumber::GENERIC_ERROR));
    }

    return -1;
}

//-------------------------------------------------------------------------------------
std::filesystem::path CommandLayer::receiveFile() {
    std::array<uint8_t, Cache::cMD5HashSize> _md5;

    std::string _filename = receiveString();
    receiveArray(std::span<uint8_t>(_md5));

    if (!cache.needPutResource(_filename, _md5)) {
        send<int>(COMMAND_DONE);

        return cache.getResource(_filename);
    } else {
        send<int>(COMMAND_RETRY);
        uint64_t _length = receive<uint64_t>();

        fmt::println("Receiving file {} ({} bytes)", _filename, _length);

        auto _charStream = recieveCharStream(_length);

        return cache.putResource(_filename, _md5, *_charStream);
    }
}

//-------------------------------------------------------------------------------------
void CommandLayer::sendFile(std::string_view _filename) {
    std::array<uint8_t, Cache::cMD5HashSize> _md5 = cache.getResourceHash(_filename);

    sendArray(std::span<const uint8_t>(_md5));

    int _nextCommand = receive<int>();

    if (_nextCommand == COMMAND_DONE) {
        return;
    } else {
        std::filesystem::path _path = cache.getResource(_filename);
        std::ifstream _data(_path, std::ios_base::in | std::ios_base::binary);

        _data.seekg(std::ios_base::end);
        long long _filesize = _data.tellg();
        _data.seekg(std::ios_base::beg);

        send<long long>(_filesize);

        _data.close();

        FileReader _file(_path);

        sendCharStream(_file);
    }
}

//-------------------------------------------------------------------------------------
std::string CommandLayer::receiveString() {
    int _length = receive<int>();
    std::vector<char> _data(_length);

    receiveArray(_data);

    return {_data.begin(), _data.end()};
}
//-------------------------------------------------------------------------------------
