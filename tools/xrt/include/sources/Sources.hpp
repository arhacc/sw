//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/cache/Cache.hpp>
#include <sources/batch/BatchSource.hpp>
#include <sources/cmd/CmdSource.hpp>
#include <sources/common/Source.hpp>
#include <sources/file/FileSource.hpp>
#include <sources/mux/MuxSource.hpp>
#include <sources/net/NetSource.hpp>
#include <transformers/Transformers.hpp>

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
