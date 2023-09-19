#pragma once
#include "Thread.hpp"
#include "DUT.hpp"
#include "Sync.hpp"

class ClockConsumerThread : public Thread {
    Sync& mSync;

protected:
    DUT* pDUT;

public:
    ClockConsumerThread(Sync& sync, DUT* dut, std::string name);

    void wait_clock_hcycle();

    void wait_clock_cycle();

    void wait_clock_cycles(int cycles);
};