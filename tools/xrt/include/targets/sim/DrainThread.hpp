#pragma once
#include "ClockConsumerThread.hpp"

class DrainThread : public ClockConsumerThread {

private:
    virtual void run() override ;

public:
    DrainThread(Sync& sync, DUT* pdut);
};
