//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/net/stack/CommandLayer.h>

//-------------------------------------------------------------------------------------
#define CLIENT_STATUS_STOPPED 0
#define CLIENT_STATUS_RUNNING 1

//-------------------------------------------------------------------------------------
class ApplicationLayer : public CommandLayer {
  public:
    ApplicationLayer(
        MuxSource* _muxSource, Cache& _cache, const Arch& _arch, int _clientConnection);

    ~ApplicationLayer() override;

    int processClient();
};
//-------------------------------------------------------------------------------------
