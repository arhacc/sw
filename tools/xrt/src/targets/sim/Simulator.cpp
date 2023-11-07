//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include "targets/sim/Simulator.h"

#include <cstdint>
#include <vector>

//-------------------------------------------------------------------------------------
Simulator::Simulator(std::string _pathToDesign, std::string _clockName, std::string _resetName) {
    pathToDesign = _pathToDesign;
    clockName = _clockName;
    resetName = _resetName
    dut = new Dut(_pathToDesign, "librdi_simulator_kernel.so", 1, _clockName, _resetName);
}

//-------------------------------------------------------------------------------------
Simulator::~Simulator() {
    delete xpu_top;
}

//-------------------------------------------------------------------------------------
uint32_t Simulator::readRegister(uint32_t _address) {
    if (_address % 4 != 0) {
        throw std::runtime_error("Register address must be a multiple of 4");
    }
    return 0;
}

//-------------------------------------------------------------------------------------
std::vector<unsigned int> Simulator::getMatrix() {
    std::vector<unsigned int> result = xpu_top->keepAndRemove(clkThread.masterValues);

    std::cout << "Matrix size: " << clkThread.masterValues.size() << '\n';

    return result;
}

//-------------------------------------------------------------------------------------
void Simulator::run() {

    std::cout << "[MAIN] Current root: " << std::filesystem::current_path() << std::endl;
    xpu_top->generateClock(xpu_top->getHalfClockPeriod());

    std::cout << "Start time in ns: " << xpu_top->getTime() << '\n';
    std::cout << "Clock with half-period of " << std::dec << xpu_top->getHalfClockPeriod()
              << " simulation steps\n";


    std::cout << "\nEnd time: " << std::dec << xpu_top->getTime() << " ns" << std::endl;
}
//-------------------------------------------------------------------------------------
