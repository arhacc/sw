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
#include <common/resources/ResourceIdentifier.hpp>
#include <sources/mux/MuxSource.hpp>
#include <sources/net/stack/CommandLayer.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <exception>
#include <memory>
#include <vector>

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
std::string CommandLayer::commandString(int _command) {
    switch (_command) {
        case COMMAND_RESERVED: {
            return "COMMAND_RESERVED";
        }

        case COMMAND_HALT: {
            return "COMMAND_HALT";
        }

        case COMMAND_RESET: {
            return "COMMAND_RESET";
        }
        case COMMAND_CLOSE_CONNECTION: {
            return "COMMAND_CLOSE_CONNECTION";
        }

        case COMMAND_DONE: {
            return "COMMAND_DONE";
        }

        case COMMAND_ERROR: {
            return "COMMAND_ERROR";
        }

        case COMMAND_RETRY: {
            return "COMMAND_RETRY";
        }

        case COMMAND_RUN_GRAPH: {
            return "COMMAND_RUN_GRAPH";
        }

        case COMMAND_GET_RESOURCE: {
            return "COMMAND_GET_RESOURCE";
        }

        case COMMAND_DEBUG_ADD_BREAKPOINT: {
            return "COMMAND_DEBUG_ADD_BREAKPOINT";
        }

        case COMMAND_DEBUG_READ_ARRAY_REGISTRY: {
            return "COMMAND_DEBUG_READ_ARRAY_REGISTRY";
        }

        case COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA: {
            return "COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA";
        }

        case COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA: {
            return "COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA";
        }

        case COMMAND_DEBUG_READ_CONTROLLER_REGISTRY: {
            return "COMMAND_DEBUG_READ_CONTROLLER_REGISTRY";
        }

        case COMMAND_DEBUG_READ_CONTROLLER_MEMORY_DATA: {
            return "COMMAND_DEBUG_READ_CONTROLLER_MEMORY_DATA";
        };

        case COMMAND_GET_ARCHITECTURE_ID: {
            return "COMMAND_GET_ARCHITECTURE_ID";
        }

        case COMMAND_PING: {
            return "COMMAND_PING";
        }

        case COMMAND_ACK: {
            return "COMMAND_ACK";
        }

        default: {
            return "NOT A KNOWN COMMAND";
        }
    }
}

//-------------------------------------------------------------------------------------
int CommandLayer::processCommand(int _command) {
    logWork.print(fmt::format("Received command: {} ({})\n", _command, commandString(_command)));

    if (firstCommand) {
      if (_command != COMMAND_GET_ARCHITECTURE_ID) {
        throw std::runtime_error("client connected and sent some other command instead of COMMAND_GET_ARCHITECTURE_ID as first command");
      } else {
        firstCommand = false;
      }
    }

    try {
        try {
            switch (_command) {
                case COMMAND_RESERVED: {
                    break;
                }

                case COMMAND_HALT: {
                    exit(EXIT_SUCCESS);
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

                case COMMAND_RETRY: {
                    unsigned _bp;
                    
                    if ((_bp = muxSource.debugContinue()) > 0) {
                        send<uint32_t>(COMMAND_BREAKPOINT_HIT);
                        send<uint32_t>(_bp - 1);

                        logWork.print("\nHit breakpoint\n");
                    } else {
                        send<uint32_t>(COMMAND_DONE);
                        logWork.print("\nDone running function\n");
                    }

                    break;
                }

                case COMMAND_RUN_GRAPH: {
                    std::string _s = receiveString();

                    logWork.print(fmt::format("GRAPH: {}\n", _s));

                    std::size_t _numInputs{receive<uint32_t>()};
                    std::vector<std::string> _inputMapNames(_numInputs);
                    std::vector<std::string> _inputMapRiString(_numInputs);

                    for (std::size_t _i = 0; _i < _numInputs; _i++) {
                      _inputMapNames[_i] = receiveString();
                      _inputMapRiString[_i] = receiveString();

                      logWork.print(fmt::format("INPUT: {} : {}\n", _inputMapNames[_i], _inputMapRiString[_i]));
                    }

                    std::size_t _numOutputs{receive<uint32_t>()};
                    std::vector<std::string> _outputMapNames(_numOutputs);
                    std::vector<std::string> _outputMapRiString(_numOutputs);

                    for (std::size_t _i = 0; _i < _numOutputs; _i++) {
                      _outputMapNames[_i] = receiveString();
                      _outputMapRiString[_i] = receiveString() + "@0.0.0#00000000000000000000000000000000";

                      logWork.print(fmt::format("OUTPUT: {} : {}\n", _outputMapNames[_i], _outputMapRiString[_i]));
                    }

                    ResourceIdentifier _ri = ResourceIdentifier::fromString(_s);

                    std::unordered_map<std::string, ResourceIdentifier> _inputsMap;
                    for (std::size_t _i = 0; _i < _numInputs; _i++) {
                      _inputsMap[_inputMapNames[_i]] = ResourceIdentifier::fromString(_inputMapRiString[_i]);
                    }

                    std::unordered_map<std::string, ResourceIdentifier> _outputsMap;
                    for (std::size_t _i = 0; _i < _numOutputs; _i++) {
                      outputsMap[_outputMapNames[_i]] = ResourceIdentifier::fromString(_outputMapRiString[_i]);
                    }

                    unsigned _bp;
                    
                    if ((_bp = muxSource.run(_ri, _inputsMap, _outputsMap)) > 0) {
                        send<uint32_t>(COMMAND_BREAKPOINT_HIT);
                        send<uint32_t>(_bp - 1);

                        logWork.print("\nHit breakpoint\n");
                    } else {
                        send<uint32_t>(COMMAND_DONE);
                        logWork.print("\nDone running function\n");
                    }

                    break;
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
                    uint32_t _iterationCounter = receive<uint32_t>();

                    (void) _iterationCounter;
                    logWork.print(fmt::format("WARNING: iteration counter {} ignored for COMMAND_DEBUG_ADD_BREAKPOINT", _iterationCounter));

                    // TODO: NOT LIKE THIS
                    _functionName.resize(_functionName.find('.'));

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

                    std::vector<uint32_t> _ret =
                        muxSource.debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);

                    // cast to signed
                    sendArray(reinterpret_cast<std::vector<int>&&>(std::move(_ret)));

                    break;
                }

                case COMMAND_DEBUG_READ_CONTROLLER_REGISTRY: {
                    std::vector<uint32_t> _ret = muxSource.debugGetControllerRegs();

                    // cast to signed
                    sendArray(std::move(_ret));

                    break;
                }

                case COMMAND_DEBUG_READ_CONTROLLER_MEMORY_DATA: {
                    uint32_t _firstRow  = receive<uint32_t>();
                    uint32_t _lastRow   = receive<uint32_t>();

                    std::vector<uint32_t> _ret =
                        muxSource.debugGetControllerData(_firstRow, _lastRow);

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
                    throw XrtException(
                        fmt::format("Unknown net command {}", _command), XrtErrorNumber::UNKNOWN_COMMAND);
                }
            }
        } catch (const XrtException& _exception) {
            fmt::println(
                "Error processing net command {}: {} ({})", _command, _exception.what(), _exception.errorNumberInt());

            send<int>(COMMAND_ERROR);
            send<int>(_exception.errorNumberInt());
        } catch (const std::exception& _exception) {
            fmt::println("Error processing net command {}: {}", _command, _exception.what());

            send<int>(COMMAND_ERROR);
            send(static_cast<int>(XrtErrorNumber::GENERIC_ERROR));
        } catch (...) {
            fmt::println("Unknown error processing net command {}", _command);

            auto e = std::current_exception();

            send<int>(COMMAND_ERROR);
            send<int>(static_cast<int>(XrtErrorNumber::GENERIC_ERROR));
        }
    } catch (...) {
        fmt::println("Exception while handling exception in CommandLayer");
    }

    return -1;
}

//-------------------------------------------------------------------------------------
