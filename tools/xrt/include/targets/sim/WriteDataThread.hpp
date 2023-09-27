#pragma once
#include "targets/sim/ClockConsumerThread.hpp"
#include "nlohmann/json.hpp"

class WriteDataThread : public ClockConsumerThread {
    virtual void run() override;

    void AxiStreamSendData(const std::string_view& data);

    void AxiStreamSendStop();

public:
    WriteDataThread(Sync& sync, DUT* pdut, std::vector<std::string>& dataIn);

    std::vector<std::string>& _dataIn;
};
