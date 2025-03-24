//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
// FpgaTarget -- XPU AXI Driver
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>

#include <string>
#include <string_view>

class UioDevice {
  private:
    const    std::string    name_;
    const    std::size_t    registerSpaceSize_;
    volatile std::uint32_t *registerSpace_;
             int            registerSpaceFd_;

  public:
    UioDevice(std::string_view name, const char *path, std::size_t registerSpaceSize);
    ~UioDevice();

    std::uint32_t readRegister(std::size_t address);
    void writeRegister(std::size_t address, std::uint32_t value);
};
