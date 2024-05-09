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

#include <cassert>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

#include <common/resources/ResourceIdentifier.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <openssl/md5.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
CommandLayer::CommandLayer(MuxSource& _muxSource, const Arch& _arch, sockpp::tcp_socket&& _socket)
    : NetworkLayer(std::move(_socket)),
      arch(_arch),
      resourceLoader(std::make_shared<ResourceLoader>(_arch)),
      muxSource(_muxSource) {}

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

            case COMMAND_RUN_GRAPH: {
                std::string _s = receiveString();
                logWork.print(fmt::format("Net: run graph: {}\n", _s));
                ResourceIdentifier _ri = ResourceIdentifier::fromString(_s);
                muxSource.run(_ri);
            }

            case COMMAND_GET_RESOURCE: {
                std::string _s = receiveString();
                logWork.print(fmt::format("Net: get file: {}\n", _s));
                ResourceIdentifier _ri = ResourceIdentifier::fromString(_s);

                if (!Cache::haveResource(_ri)) {
                    send<uint32_t>(COMMAND_ERROR);
                    send(static_cast<uint32_t>(XrtErrorNumber::RESOURCE_NOT_FOUND));
                    break;
                }

                send<uint8_t>(COMMAND_DONE);
                sendFile(Cache::getPathOfResource(_ri));

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
