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
BatchSource::BatchSource(
    MuxSource* _muxSource, const std::vector<std::string>& _batchFiles) {
    for (const std::string& _fileName : _batchFiles) {
        std::ifstream _file(_fileName);
        if (_file.is_open()) {
            std::string _line;
            while (std::getline(_file, _line)) {
                _muxSource->runCommand(_line);
            }
            _file.close();
        }
    }
}

//-------------------------------------------------------------------------------------
