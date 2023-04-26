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
    MuxSource *muxSource;
    NetSource *netSource;
    BatchSource *batchSource;
    FileSource *fileSource;
    CmdSource *cmdSource;
public:
    Sources(Transformers *_transformers, const std::string &_serverPort, const std::vector<std::string> &_batchFiles,
            const std::vector<std::string> &_files, bool _enableCmd);

    ~Sources();
};
//-------------------------------------------------------------------------------------


