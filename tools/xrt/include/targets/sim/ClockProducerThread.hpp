#pragma once
#include "Thread.hpp"
#include "DUT.hpp"
#include "Sync.hpp"

#include <set>

class ClockProducerThread : public Thread {
    DUT* pDUT;
    Sync& mWP;
    Sync& mWD;
    Sync& mRD;
    Sync& mOther;
    Semaphore& mExitTick;

public:
    ClockProducerThread(DUT* dut, Sync& wp, Sync& wd, Sync& rd, Sync& other, Semaphore& exitTick);
    void run() override;
    std::vector<unsigned int> masterValues;
};