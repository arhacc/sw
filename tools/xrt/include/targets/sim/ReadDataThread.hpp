#pragma once
#include "AxiLite.hpp"

class ReadDataThread : public AxiLite {
    std::fstream axi_stream_out_file;

    virtual void run() override;

    void AxiStreamReceiveData();

    void AxiStreamStopReceiveData();

public:
    ReadDataThread(Sync& sync, DUT* pdut);
};
