//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>

class AXILiteWriter {
  constexpr static char cDeviceName[] = "/dev/uio1";
  constexpr static size_t cMapSize = 1024;
    int fd;
    volatile uint32_t *regmap;

  public:
    AXILiteWriter();

    void writeRegister(uint32_t address, uint32_t value);
    uint32_t readRegister(uint32_t address);
};
