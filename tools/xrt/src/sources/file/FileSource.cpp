//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.h>
#include <sources/file/FileSource.h>
#include <sources/mux/MuxSource.h>

//-------------------------------------------------------------------------------------
FileSource::FileSource(MuxSource* _muxSource, const std::vector<std::string>& _files) {
    //    std::cout << "Starting FileSource..." << std::endl;
    for (const auto& _file : _files) {
        //    	std::cout << "Starting FileSource.1..." << _files.at(i) << std::endl;
        std::string _cmd = "source " + _file;
        _muxSource->runCommand(_cmd);

        std::string _functionName = getFileStemFromGeneralPath(_file);

        _cmd = "run " + _functionName;
        _muxSource->runCommand(_cmd);
    }
}

//-------------------------------------------------------------------------------------
