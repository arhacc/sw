#ifndef XRT_SIMULATOR_H
#define XRT_SIMULATOR_H

//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <vector>
#pragma once

#include <filesystem>
#include <iostream>
#include <span>

#include "targets/sim/ClockProducerThread.hpp"
#include "targets/sim/DrainThread.hpp"
#include "targets/sim/InitThread.hpp"
#include "targets/sim/Sync.hpp"
#include "targets/sim/Timer.hpp"
#include "targets/sim/WriteDataThread.hpp"
#include "targets/sim/WriteProgramThread.hpp"

//-------------------------------------------------------------------------------------
class Simulator {
  private:
    std::string pathToDesign;
    std::string clock;
    std::string reset;

    DUT* xpu_top;
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

    // std::span<uint32_t> programFile;
    // std::span<uint32_t> programData;

  public:
    Simulator(
        std::string designPath,
        std::string clockName,
        std::string resetName,
        std::span<uint32_t> programFile,
        std::span<uint32_t> dataFile);

    ~Simulator();

    void run();

    std::vector<unsigned int> getMatrix();

    uint32_t readRegister(uint32_t _address);

    //-------------------------------------------------------------------------------------
};
#endif // XRT_SIMULATOR_H
