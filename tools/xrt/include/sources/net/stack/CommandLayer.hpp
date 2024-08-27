//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/resources/ResourceIdentifier.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <sources/net/stack/NetworkLayer.hpp>

#include <sockpp/tcp_socket.h>
#include <sources/mux/MuxSource.hpp>
#include <string>

#define COMMAND_RESERVED 0
#define COMMAND_HALT     1
#define COMMAND_RESET    2
#define COMMAND_PAUSE    3
#define COMMAND_CONTINUE 4

#define COMMAND_GET_ARCHITECTURE_ID 40000

#define COMMAND_DONE           100
#define COMMAND_ERROR          101
#define COMMAND_RETRY          102
#define COMMAND_BREAKPOINT_HIT 103

#define COMMAND_GET_RESOURCE 200

#define COMMAND_RUN_GRAPH 400

#define COMMAND_DEBUG_ADD_BREAKPOINT    401
#define COMMAND_DEBUG_REMOVE_BREAKPOINT 402

#define COMMAND_DEBUG_READ_CONTROLLER_REGISTRY              500
#define COMMAND_DEBUG_READ_CONTROLLER_MEMORY_INSTRUCTIONS   501
#define COMMAND_DEBUG_READ_CONTROLLER_MEMORY_DATA           502
#define COMMAND_DEBUG_READ_ARRAY_REGISTRY                   503
#define COMMAND_DEBUG_READ_ARRAY_MEMORY_INSTRUCTIONS        504
#define COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA                505

#define COMMAND_DEBUG_WRITE_CONTROLLER_REGISTRY             600
#define COMMAND_DEBUG_WRITE_CONTROLLER_MEMORY_INSTRUCTIONS  601
#define COMMAND_DEBUG_WRITE_CONTROLLER_MEMORY_DATA          602
#define COMMAND_DEBUG_WRITE_ARRAY_REGISTRY                  603
#define COMMAND_DEBUG_WRITE_ARRAY_MEMORY_INSTRUCTIONS       604
#define COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA               605

#define COMMAND_PING 1000
#define COMMAND_ACK  1001

#define COMMAND_OPEN_CONNECTION  10000
#define COMMAND_CLOSE_CONNECTION 10001

//-------------------------------------------------------------------------------------
class CommandLayer : public NetworkLayer {
    const Arch& arch;
    std::shared_ptr<ResourceLoader> resourceLoader;

    MuxSource& muxSource;

    bool firstCommand = true;

    static bool checkFileExtension(const std::string& _filename, int _command);

    static std::string commandString(int _command);

  public:
    CommandLayer(MuxSource& _muxSource, const Arch& _arch, sockpp::tcp_socket&& _socket);

    ~CommandLayer() override = default;

    int processCommand(int _command);

};
//-------------------------------------------------------------------------------------
