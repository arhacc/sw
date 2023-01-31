//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/file/FileSource.h>
//-------------------------------------------------------------------------------------
FileSource::FileSource(MuxSource* _muxSource, std::vector<std::string> _files){
//    std::cout << "Starting FileSource..." << std::endl;
	for (int i = 0; i < _files.size(); i++) {
//    	std::cout << "Starting FileSource.1..." << _files.at(i) << std::endl;
    	std::string _cmd = "run " + _files.at(i);
	    _muxSource -> runCommand(_cmd);
	}
}

//-------------------------------------------------------------------------------------
FileSource::~FileSource(){

}

//-------------------------------------------------------------------------------------
