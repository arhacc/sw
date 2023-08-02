//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/cache/Cache.h>
#include <sources/batch/BatchSource.h>
#include <sources/cmd/CmdSource.h>
#include <sources/common/Source.h>
#include <sources/file/FileSource.h>
#include <sources/mux/MuxSource.h>
#include <sources/net/NetSource.h>
#include <transformers/Transformers.h>

#include <iostream>

//-------------------------------------------------------------------------------------
class Sources {
    MuxSource* muxSource;
    NetSource* netSource;
    BatchSource* batchSource;
    FileSource* fileSource;
    CmdSource* cmdSource;

  public:
    Sources(
        Transformers* _transformers,
        const Arch& _arch,
        const std::string& _serverPort,
        const std::vector<std::string>& _batchFiles,
        const std::vector<std::string>& _files,
        bool _enableCmd);

    ~Sources();
};
//-------------------------------------------------------------------------------------
