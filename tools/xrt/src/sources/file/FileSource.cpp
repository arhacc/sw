//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <sources/file/FileSource.hpp>
#include <sources/mux/MuxSource.hpp>

//-------------------------------------------------------------------------------------
FileSource::FileSource(MuxSource* _muxSource, const std::vector<std::string>& _files) {
    //    std::cout << "Starting FileSource..." << std::endl;
    for (const auto& _file : _files) {
        //    	std::cout << "Starting FileSource.1..." << _files.at(i) << std::endl;
        _muxSource->load(_file);

        std::string _functionName = getFileNameFromPath(_file);

        _muxSource->run(_file);
    }
}

//-------------------------------------------------------------------------------------
