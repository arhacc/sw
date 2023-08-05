#include "targets/sim/Simulator.h"

Simulator::Simulator(std::string designPath, std::string clockName, std::string resetName):
        pathToDesign(std::ref(designPath)),
        clock(std::ref(clockName)),
        reset(std::ref(resetName)),
        xpu_top(new DUT(pathToDesign, "librdi_simulator_kernel.so", 1, clock, reset)),
        clkThread(xpu_top, syncWP, syncWD, syncRD, syncOther, ExitTick),
        initThread(syncOther, xpu_top, dataIn),
        wpThread(syncWP, xpu_top),
        wdThread(syncWD, xpu_top, dataIn),
        drainThread(syncOther, xpu_top){

}

Simulator::~Simulator() {
    delete xpu_top;
}

uint32_t Simulator::readRegister(uint32_t _address) {

#ifndef NDEBUG
    if (_address % 4 != 0) {
        throw std::runtime_error("Register address must be a multiple of 4");
    }
#endif

    return 0;
}

void Simulator::getMatrix() {
    std::vector<unsigned int> result = xpu_top->keepAndRemove(clkThread.masterValues);

    std::cout << "Matrix size: " << clkThread.masterValues.size() << '\n';

    for (std::size_t i = 0; auto &it: result) {
        if (i % 16 == 0)
            std::cout << '\n';
        std::cout << it << ' ';
        i++;
    }
}

void Simulator::run() {
    Timer::getInstance();

    Simulator simulator("xsim.dir/simulator_axi/xsimk.so", "clock", "resetn");

    std::cout << "[MAIN] Current root: " << std::filesystem::current_path() << std::endl;
    xpu_top->generateClock(xpu_top->getHalfClockPeriod());

    std::cout << "Start time in ns: " << xpu_top->getTime() << '\n';
    std::cout << "Clock with half-period of " << std::dec << xpu_top->getHalfClockPeriod() << " simulation steps\n";

    simulator.clkThread.start();

    simulator.initThread.start();
    simulator.initThread.join();

    simulator.wpThread.start();
    simulator.wdThread.start();

    simulator.wpThread.join();
    simulator.wdThread.join();

    simulator.drainThread.start();
    simulator.drainThread.join();

    // request clock thread to end
    simulator.ExitTick.release();
    simulator.clkThread.join();

    std::cout << "\nEnd time: " << std::dec << xpu_top->getTime() << " ns" << std::endl;
}