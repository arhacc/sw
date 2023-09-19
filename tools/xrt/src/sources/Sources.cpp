//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/Sources.h>
#include <transformers/Transformers.h>

//-------------------------------------------------------------------------------------
Sources::Sources(
    Transformers* _transformers,
    const Arch& _arch,
    const std::string& _serverPort,
    const std::vector<std::string>& _batchFiles,
    const std::vector<std::string>& _files,
    bool _enableCmd)

    : muxSource(nullptr),
      netSource(nullptr),
      batchSource(nullptr),
      fileSource(nullptr),
      cmdSource(nullptr) {
    muxSource = new MuxSource(_transformers);

    if (!_batchFiles.empty()) {
        batchSource = new BatchSource(muxSource, _batchFiles);
    }
    if (!_files.empty()) {
        fileSource = new FileSource(muxSource, _files);
    }
    if (!_serverPort.empty()) {
        int _serverPortInt = stoi(_serverPort);
        netSource          = new NetSource(muxSource, _arch, _serverPortInt);
    }
    if (_enableCmd == 1) {
        cmdSource = new CmdSource(muxSource);
    }
    if (netSource || cmdSource) {
        pause();
    }
}

//-------------------------------------------------------------------------------------
Sources::~Sources() {
    delete muxSource;
    delete batchSource;
    delete fileSource;
    delete netSource;
    delete cmdSource;
}

//-------------------------------------------------------------------------------------
