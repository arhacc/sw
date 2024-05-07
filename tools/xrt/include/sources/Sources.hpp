//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

// forward declaration
class BatchSource;
class CmdSource;
class FileSource;
class MuxSource;
class NetSource;
class Transformers;
struct Arch;

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
