//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <openssl/md5.h>
#include "sources/net/stack/NetworkLayer.h"
#define COMMAND_RESERVED                    0
#define COMMAND_HALT                        1
#define COMMAND_RESET                       2
#define COMMAND_IDLE                        3
#define COMMAND_RUN                         4
#define COMMAND_DEBUG_MODE                  5
#define COMMAND_LOAD_CODE_MEMORY            6
#define COMMAND_LOAD_DATA_MEMORY            7
#define COMMAND_UNLOAD_DATA_MEMORY          8
#define COMMAND_LOAD_FEATURE_MEMORY         9
#define COMMAND_UNLOAD_FEATURE_MEMORY       10

#define COMMAND_DONE                        100
#define COMMAND_ERROR                       101
#define COMMAND_RETRY                       102

#define COMMAND_RUN_FILE_HEX                200
#define COMMAND_RUN_FILE_JSON               201
#define COMMAND_RUN_FILE_OBJ                202
#define COMMAND_RUN_FILE_ONNX               203


#define COMMAND_PING                        1000
#define COMMAND_ACK                         1001

#define COMMAND_OPEN_CONNECTION             10000
#define COMMAND_CLOSE_CONNECTION            10001

//-------------------------------------------------------------------------------------
class CommandLayer : public NetworkLayer {

public:
	CommandLayer(MuxSource* _muxSource, int _clientConnection);
	~CommandLayer();
	int processCommand(int _command);
	std::string receiveFile();
	std::string receiveString();
	bool checkMD5File(std::string _filename, std::string _md5Hex);
	std::string toHexString(unsigned char* _bytes);

protected:
//	int clientConnection;
//	CmdSource* cmdSource;
};
//-------------------------------------------------------------------------------------
