#include "targets/sim/DrainThread.hpp"

void DrainThread::run()  {
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Started: " << getCurrentThreadName() << std::endl;

    wait_clock_cycles(SIMULATION_DRAIN_TIME_CLOCK_CYCLES);

    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Ended: " << getCurrentThreadName() << std::endl;
}

DrainThread::DrainThread(Sync& sync, DUT* pdut) :
        ClockConsumerThread(sync, pdut, "DrainThread") {

}

