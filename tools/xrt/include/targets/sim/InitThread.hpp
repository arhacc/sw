#pragma once
#include <cstdint>
#include <span>

#include "nlohmann/json.hpp"
#include "targets/sim/ClockConsumerThread.hpp"

class InitThread : public ClockConsumerThread {
  private:
    virtual void run() override;

  public:
    InitThread(
        Sync& sync,
        DUT* pdut,
        std::vector<std::string>& dataIn,
        std::span<uint32_t> dataFile);

    std::vector<std::string>& _dataIn;
};
