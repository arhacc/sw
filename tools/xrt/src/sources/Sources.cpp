//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/Transformers.h>
#include <sources/Sources.h>
#include <sources/common/Source.cpp>
#include <sources/mux/MuxSource.cpp>
#include <sources/net/NetSource.cpp>
#include <sources/batch/BatchSource.cpp>
#include <sources/file/FileSource.cpp>
#include <sources/cmd/CmdSource.cpp>
//-------------------------------------------------------------------------------------
Sources::Sources(Transformers* _transformers, std::string _serverPort, std::string _batch, std::vector<std::string> _files, bool _enableCmd){
    muxSource = new MuxSource(_transformers);

    if(!_batch.empty()){
        batchSource = new BatchSource(muxSource, _batch);
    }
    if(_files.size() > 0){
        fileSource = new FileSource(muxSource, _files);
    }
    if(!_serverPort.empty()){
        int _serverPortInt = stoi(_serverPort);
        netSource = new NetSource(muxSource, _serverPortInt);
    }
    if(_enableCmd == 1){
        cmdSource = new CmdSource(muxSource);
    }
    if(netSource || cmdSource){
        pause();
    }
}

//-------------------------------------------------------------------------------------
Sources::~Sources(){
    delete(muxSource);
    delete(batchSource);
    delete(fileSource);
    delete(netSource);
    delete(cmdSource);
}

//-------------------------------------------------------------------------------------
