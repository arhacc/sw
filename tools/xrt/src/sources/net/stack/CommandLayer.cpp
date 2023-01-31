//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "sources/net/stack/NetworkLayer.cpp"

//-------------------------------------------------------------------------------------
CommandLayer::CommandLayer(MuxSource* _muxSource, int _clientConnection) : NetworkLayer(_muxSource, _clientConnection){
//  cmdSource = _cmdSource;
//  clientConnection = _clientConnection;
}

//-------------------------------------------------------------------------------------
CommandLayer::~CommandLayer() {
}

//-------------------------------------------------------------------------------------
int CommandLayer::processCommand(int _command) {
  switch(_command){
    case COMMAND_RESERVED:{
      break;
    }

    case COMMAND_HALT:{
      break;
    }

    case COMMAND_RESET:{
      break;
    }

    case COMMAND_IDLE:{
      break;
    }

    case COMMAND_RUN:{
      break;
    }

    case COMMAND_DEBUG_MODE:{
      break;
    }



    case COMMAND_LOAD_CODE_MEMORY:{
//      loadCodeMemory();
      break;
    }
    case COMMAND_LOAD_DATA_MEMORY:{
//      loadDataMemory();
      break;
    }
    case COMMAND_CLOSE_CONNECTION:{
//      closeConnection();
      break;
    }

    case COMMAND_DONE:{
      break;
    }

    case COMMAND_ERROR:{
      break;
    }

    case COMMAND_RUN_FILE_ONNX:{
      unsigned char _md5[MD5_DIGEST_LENGTH];
      receiveCharArray(_md5, MD5_DIGEST_LENGTH);
      std::string _md5Hex = toHexString(_md5);
      std::cout << "Receive ONNX file MD5: " << _md5Hex << std::endl;
      std::string _fullPath = std::getenv("HOME");
      _fullPath = _fullPath + "/.xpu/tmp/" + _md5Hex;
      if(checkMD5File(_md5Hex)){
        sendInt(COMMAND_DONE);
      } else {
        std::cout << "Send RETRY..." << std::endl;
        sendInt(COMMAND_RETRY);
        long _length = receiveLong();
        std::cout << "File length = " << _length << std::endl;
        std::ofstream _file;
        _file.open(_fullPath, std::ios::binary);
        unsigned char _buffer[1024];
        long _index = 0;
        while(_index < _length) {
          int _bufferSize = (_length - _index);
          _bufferSize = (_bufferSize > 1024) ? 1024 : _bufferSize;
          receiveCharArray(_buffer, _bufferSize);
          _file.write((const char *)_buffer, _bufferSize);
          _index += _bufferSize;
        }
        _file.close();        
      }
      muxSource->runCommand("run " + _fullPath);
      break;
    }

    case COMMAND_PING:{
      sendInt(COMMAND_ACK);
      break;
    }

    case COMMAND_ACK:{
      
      break;
    }
    

    default: {
      std::cout << "Error: Unknown command: " << _command << std::endl;
      exit(EXIT_FAILURE);      
      break;
    }
  }
  return -1;
}

//-------------------------------------------------------------------------------------
bool CommandLayer::checkMD5File(std::string _path) {
  std::string _fullPath = std::getenv("HOME");
  _fullPath = _fullPath + "/.xpu/tmp/" + _path;
  std::cout << "Check file:  " << _fullPath << std::endl;  
  std::ifstream _file(_fullPath.c_str());
  return _file.good();
}

//-------------------------------------------------------------------------------------
std::string CommandLayer::toHexString(unsigned char* _bytes){
    std::stringstream _hexString;
    for (int i=0; i < MD5_DIGEST_LENGTH; i++) {
//      printf("%02x", (0xff & (unsigned int)_bytes[i]));
      _hexString << std::hex << std::setw(2) << std::setfill('0') << (int) _bytes[i] ;
//      std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)_md[i]);
    }
    return _hexString.str();
}
//-------------------------------------------------------------------------------------
