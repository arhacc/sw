#include "targets/sim/ClockConsumerThread.hpp"

ClockConsumerThread::ClockConsumerThread(Sync& sync, DUT* dut, std::string name) :
        Thread(name),
        mSync(sync),
        pDUT(dut) {
}

void ClockConsumerThread::wait_clock_hcycle() {
    mSync.getRequestedTicks().release();
    mSync.getGeneratedTicks().acquire();
}

void ClockConsumerThread::wait_clock_cycle() {
    wait_clock_hcycle();
    wait_clock_hcycle();
}

void ClockConsumerThread::wait_clock_cycles(int cycles) {
    while (cycles-- > 0)
        wait_clock_cycle();
}
