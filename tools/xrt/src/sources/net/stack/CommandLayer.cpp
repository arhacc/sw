//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "sources/net/stack/CommandLayer.h"
#include "common/Utils.h"
#include "sources/mux/MuxSource.h"
#include <openssl/md5.h>
#include <stdexcept>

//-------------------------------------------------------------------------------------
CommandLayer::CommandLayer(MuxSource *_muxSource, Cache *_cache, const Arch& _arch, int _clientConnection)
    : NetworkLayer(_muxSource, _clientConnection),
      arch(_arch), cache(_cache) {

}

//-------------------------------------------------------------------------------------
bool CommandLayer::checkFileExtension(const std::string& _filename, int _command) {

    int _fileType = getFileTypeFromGeneralPath(_filename);

    switch (_command) {
    case COMMAND_LOAD_FILE_HEX:
        return _fileType == XPU_FILE_HEX;
    case COMMAND_LOAD_FILE_JSON:
        return _fileType == XPU_FILE_JSON;
    case COMMAND_LOAD_FILE_OBJ:
        return _fileType == XPU_FILE_OBJ;
    case COMMAND_LOAD_FILE_CPP:
        return _fileType == XPU_FILE_SO;
    case COMMAND_LOAD_FILE_ONNX:
        return _fileType == XPU_FILE_ONNX;
    default:
        throw std::runtime_error("CommandLayer::checkFileExtension internal error");
    }
}

//-------------------------------------------------------------------------------------
int CommandLayer::processCommand(int _command) {
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

        case COMMAND_IDLE: {
            break;
        }

        case COMMAND_RUN: {
            break;
        }

        case COMMAND_DEBUG_MODE: {
            break;
        }


        case COMMAND_LOAD_CODE_MEMORY: {
            //      loadCodeMemory();
            break;
        }
        case COMMAND_LOAD_DATA_MEMORY: {
            //      loadDataMemory();
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

        case COMMAND_LOAD_FILE_HEX:
        case COMMAND_LOAD_FILE_JSON:
        case COMMAND_LOAD_FILE_OBJ:
        case COMMAND_LOAD_FILE_ONNX:
        case COMMAND_LOAD_FILE_CPP: {
            unsigned char _level = receiveChar();

            // currently unused; TODO
            (void) _level;

            std::string _fullPath = receiveFile();
            muxSource->runCommand("source " + _fullPath);

            /*if (!checkFileExtension(_fullPath, _command)) {
                throw std::runtime_error("bad file exenstion");
            }*/


            break;
        }

        case COMMAND_RUN_FUNCTION: {
            std::string _functionName = receiveString();

            muxSource->runCommand("run " + _functionName);

            break;
        }

        case COMMAND_GET_ARCHITECTURE_ID: {
            // TODO: Implement NetworkLayer::sendCharArray

            for (uint8_t c : arch.ID) {
                sendChar(c);
            }

            break;
        }

        case COMMAND_PING: {
            sendInt(COMMAND_ACK);
            break;
        }

        case COMMAND_ACK: {

            break;
        }

        default: {
            throw std::runtime_error("Error: Unknown command: " + std::to_string(_command));
        }
    }
    return -1;
}

//-------------------------------------------------------------------------------------
std::string CommandLayer::receiveFile() {
    unsigned char _md5[MD5_DIGEST_LENGTH];
    std::string _filename = receiveString();
    receiveCharArray(_md5, MD5_DIGEST_LENGTH);
    std::string _md5Hex = toHexString(_md5);
    std::cout << "Receive file MD5: " << _md5Hex << std::endl;
    if (!cache->needInstallResource(_filename, _md5Hex)) {
        sendInt(COMMAND_DONE);

        return cache->getResourceFromFilename(_filename);
    } else {
        std::cout << "Send RETRY..." << std::endl;
        sendInt(COMMAND_RETRY);
        long _length = receiveLong();
        return cache->installResource(_filename, _md5Hex, recieveCharStream(_length));
    }
}

//-------------------------------------------------------------------------------------
std::string CommandLayer::receiveString() {
    int _length = receiveInt();
    auto *_string = new unsigned char[_length];

    receiveCharArray(_string, _length);
    std::stringstream _stdString;
    for (int i = 0; i < _length; i++) {
        //      printf("%02x", (0xff & (unsigned int)_bytes[i]));
        _stdString << _string[i];
        //      std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)_md[i]);
    }
    delete[] _string;
    return _stdString.str();
}

//-------------------------------------------------------------------------------------
std::string CommandLayer::toHexString(unsigned char *_bytes) {
    std::stringstream _hexString;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        //      printf("%02x", (0xff & (unsigned int)_bytes[i]));
        _hexString << std::hex << std::setw(2) << std::setfill('0') << (int) _bytes[i];
        //      std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)_md[i]);
    }
    return _hexString.str();
}
//-------------------------------------------------------------------------------------
