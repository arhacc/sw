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
    resetName = _resetName;
    dut = new Dut(_pathToDesign, "librdi_simulator_kernel.so", 1, _clockName, _resetName);
}

//-------------------------------------------------------------------------------------
Simulator::~Simulator() {
    delete dut;
}

//-------------------------------------------------------------------------------------
uint32_t Simulator::readRegister(uint32_t _address) {
    if (_address % 4 != 0) {
        throw std::runtime_error("Register address must be a multiple of 4");
    }
    return 0;
}

//-------------------------------------------------------------------------------------
void Simulator::run() {

    std::cout << "[MAIN] Current root: " << std::filesystem::current_path() << std::endl;
    dut->generateClock(dut->getHalfClockPeriod());

    std::cout << "Start time in ns: " << dut->getTime() << '\n';
    std::cout << "Clock with half-period of " << std::dec << dut->getHalfClockPeriod()
              << " simulation steps\n";


    std::cout << "\nEnd time: " << std::dec << dut->getTime() << " ns" << std::endl;
}
//-------------------------------------------------------------------------------------
