#ifndef XRT_SIMULATOR_H
#define XRT_SIMULATOR_H

//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <filesystem>

#include "targets/sim/Timer.hpp"
#include "targets/sim/Sync.hpp"
#include "targets/sim/ClockProducerThread.hpp"
#include "targets/sim/InitThread.hpp"
#include "targets/sim/WriteDataThread.hpp"
#include "targets/sim/WriteProgramThread.hpp"
#include "targets/sim/DrainThread.hpp"

//-------------------------------------------------------------------------------------
class Simulator {
private:
    std::string pathToDesign;
    std::string clock;
    std::string reset;

    DUT *xpu_top;
    Sync syncWP;
    Sync syncWD;
    Sync syncRD;
    Sync syncOther;

    Semaphore ExitTick;

    std::vector<std::string> dataIn;

    ClockProducerThread clkThread;
    InitThread initThread;
    WriteProgramThread wpThread;
    WriteDataThread wdThread;
    DrainThread drainThread;

public:
    Simulator(std::string designPath, std::string clockName, std::string resetName);

    ~Simulator();

    void run();

    void getMatrix();

    uint32_t readRegister(uint32_t _address);

//-------------------------------------------------------------------------------------

};
#endif //XRT_SIMULATOR_H
