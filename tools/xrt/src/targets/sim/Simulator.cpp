#include "targets/sim/Simulator.h"

#include <cstdint>
#include <vector>

Simulator::Simulator(
    std::string designPath,
    std::string clockName,
    std::string resetName,
    std::span<uint32_t> programFile,
    std::span<uint32_t> dataFile)
    : pathToDesign(std::ref(designPath)),
      clock(std::ref(clockName)),
      reset(std::ref(resetName)),
      xpu_top(new DUT(pathToDesign, "librdi_simulator_kernel.so", 1, clock, reset)),
      clkThread(xpu_top, syncWP, syncWD, syncRD, syncOther, ExitTick),
      initThread(syncOther, xpu_top, dataIn, dataFile),
      wpThread(syncWP, xpu_top, programFile),
      wdThread(syncWD, xpu_top, dataIn),
      drainThread(syncOther, xpu_top) {}

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

std::vector<unsigned int> Simulator::getMatrix() {
    std::vector<unsigned int> result = xpu_top->keepAndRemove(clkThread.masterValues);

    std::cout << "Matrix size: " << clkThread.masterValues.size() << '\n';

    return result;
}

void Simulator::run() {
    Timer::getInstance();

    std::cout << "[MAIN] Current root: " << std::filesystem::current_path() << std::endl;
    xpu_top->generateClock(xpu_top->getHalfClockPeriod());

    std::cout << "Start time in ns: " << xpu_top->getTime() << '\n';
    std::cout << "Clock with half-period of " << std::dec << xpu_top->getHalfClockPeriod()
              << " simulation steps\n";

    clkThread.start();

    initThread.start();
    initThread.join();

    wpThread.start();
    wdThread.start();

    wpThread.join();
    wdThread.join();

    drainThread.start();
    drainThread.join();

    // request clock thread to end
    ExitTick.release();
    clkThread.join();

    std::cout << "\nEnd time: " << std::dec << xpu_top->getTime() << " ns" << std::endl;
}