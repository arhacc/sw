//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include "sources/common/Source.h"
#include "sources/mux/MuxSource.h"

//-------------------------------------------------------------------------------------
class FileSource : public Source {

public:
	FileSource(MuxSource* _muxSource, std::vector<std::string> _files);
 	~FileSource();

private:
};
//-------------------------------------------------------------------------------------


