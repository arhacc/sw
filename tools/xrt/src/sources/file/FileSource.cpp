//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/file/FileSource.h>

//-------------------------------------------------------------------------------------
FileSource::FileSource(MuxSource *_muxSource, const std::vector<std::string>& _files) {
    //    std::cout << "Starting FileSource..." << std::endl;
    for (const auto &_file: _files) {
        //    	std::cout << "Starting FileSource.1..." << _files.at(i) << std::endl;
        std::string _cmd = "run " + _file;
        _muxSource->runCommand(_cmd);
    }
}

//-------------------------------------------------------------------------------------
