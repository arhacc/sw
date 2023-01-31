//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/batch/BatchSource.h>
#include <fstream>

//-------------------------------------------------------------------------------------
BatchSource::BatchSource(MuxSource* _muxSource, std::string _batch){
  std::ifstream _file(_batch);
  if (_file.is_open()) {
      std::string _line;
      while (std::getline(_file, _line)) {
          _muxSource -> runCommand(_line.c_str());
      }
      _file.close();
  }
}

//-------------------------------------------------------------------------------------
BatchSource::~BatchSource(){
}

//-------------------------------------------------------------------------------------
