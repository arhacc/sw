//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include <vector>
#include <filesystem>
#include <iostream>
#include <span>
#include "Dut.h"

//-------------------------------------------------------------------------------------
class Simulator {
  private:
    std::string pathToDesign;
    std::string clockName;
    std::string resetName;

    Dut* dut;

    std::vector<std::string> dataIn;


  public:
    Simulator(std::string pathToDesign, std::string clockName, std::string resetName);

    ~Simulator();

    void run();

    uint32_t readRegister(uint32_t _address);

};
//-------------------------------------------------------------------------------------
