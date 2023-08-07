#pragma once
#include "targets/sim/ClockConsumerThread.hpp"
#include "nlohmann/json.hpp"

class InitThread : public ClockConsumerThread {

private:
    virtual void run() override;

public:
    InitThread(Sync& sync, DUT* pdut, std::vector<std::string>& dataIn);

    std::vector<std::string>& _dataIn;
};
