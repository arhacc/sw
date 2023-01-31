//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <transformers/Transformers.h>
#include <sources/common/Source.h>
#include "sources/mux/MuxSource.h"
#include "sources/net/NetSource.h"
#include "sources/batch/BatchSource.h"
#include "sources/file/FileSource.h"
#include "sources/cmd/CmdSource.h"

//-------------------------------------------------------------------------------------
class Sources {

public:
	Sources(Transformers* _transformers, std::string _serverPort, std::string _batch, std::vector<std::string> _files, bool _enableCmd);
 	~Sources();

private:
	MuxSource* muxSource;
	NetSource* netSource;
	BatchSource* batchSource;
	FileSource* fileSource;
	CmdSource* cmdSource;
};
//-------------------------------------------------------------------------------------


