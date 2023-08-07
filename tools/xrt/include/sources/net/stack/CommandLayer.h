//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/net/stack/NetworkLayer.h>

#define COMMAND_RESERVED              0
#define COMMAND_HALT                  1
#define COMMAND_RESET                 2
#define COMMAND_IDLE                  3
#define COMMAND_RUN                   4
#define COMMAND_DEBUG_MODE            5
#define COMMAND_LOAD_CODE_MEMORY      6
#define COMMAND_LOAD_DATA_MEMORY      7
#define COMMAND_UNLOAD_DATA_MEMORY    8
#define COMMAND_LOAD_FEATURE_MEMORY   9
#define COMMAND_UNLOAD_FEATURE_MEMORY 10

#define COMMAND_DONE           100
#define COMMAND_ERROR          101
#define COMMAND_RETRY          102
#define COMMAND_BREAKPOINT_HIT 103

#define COMMAND_LOAD_FILE_HEX  200
#define COMMAND_LOAD_FILE_JSON 201
#define COMMAND_LOAD_FILE_OBJ  202
#define COMMAND_LOAD_FILE_ONNX 203
#define COMMAND_LOAD_FILE_CPP  204

#define COMMAND_RUN_FUNCTION 400

#define COMMAND_DEBUG_ADD_BREAKPOINT    401
#define COMMAND_DEBUG_REMOVE_BREAKPOINT 402

#define COMMAND_DEBUG_READ_COMNTROLLER_REGISTRY            500
#define COMMAND_DEBUG_READ_COMNTROLLER_MEMORY_INSTRUCTIONS 501
#define COMMAND_DEBUG_READ_COMNTROLLER_MEMORY_DATA         502
#define COMMAND_DEBUG_READ_ARRAY_REGISTRY                  503
#define COMMAND_DEBUG_READ_ARRAY_MEMORY_INSTRUCTIONS       504
#define COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA               505

#define COMMAND_DEBUG_WRITE_COMNTROLLER_REGISTRY            600
#define COMMAND_DEBUG_WRITE_COMNTROLLER_MEMORY_INSTRUCTIONS 601
#define COMMAND_DEBUG_WRITE_COMNTROLLER_MEMORY_DATA         602
#define COMMAND_DEBUG_WRITE_ARRAY_REGISTRY                  603
#define COMMAND_DEBUG_WRITE_ARRAY_MEMORY_INSTRUCTIONS       604
#define COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA               605

#define COMMAND_PING 1000
#define COMMAND_ACK  1001

#define COMMAND_OPEN_CONNECTION  10000
#define COMMAND_CLOSE_CONNECTION 10001

#define COMMAND_GET_ARCHITECTURE_ID 40000

//-------------------------------------------------------------------------------------
class CommandLayer : public NetworkLayer {
    const Arch& arch;
    Cache& cache;

    static bool checkFileExtension(const std::string& _filename, int _command);

  public:
    static std::string toHexString(unsigned char* _bytes);

    CommandLayer(
        MuxSource* _muxSource, Cache& _cahce, const Arch& _arch, int _clientConnection);

    ~CommandLayer() override = default;

    int processCommand(int _command);

    std::string receiveFile();

    std::string receiveString();
};
//-------------------------------------------------------------------------------------
