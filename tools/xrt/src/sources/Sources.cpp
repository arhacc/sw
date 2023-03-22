//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/Transformers.h>
#include <sources/Sources.h>

//-------------------------------------------------------------------------------------
Sources::Sources(Transformers *_transformers, const std::string &_serverPort, const std::string &_batch,
        const std::vector<std::string> &_files, bool _enableCmd) {
    muxSource = new MuxSource(_transformers);

    if (!_batch.empty()) {
        batchSource = new BatchSource(muxSource, _batch);
    }
    if (!_files.empty()) {
        fileSource = new FileSource(muxSource, _files);
    }
    if (!_serverPort.empty()) {
        int _serverPortInt = stoi(_serverPort);
        netSource = new NetSource(muxSource, _serverPortInt);
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
