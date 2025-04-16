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
    static constexpr char cUioPath[] = "/sys/class/uio";

    std::string name_;
    std::size_t registerSpaceSize_;
    volatile std::uint32_t* registerSpace_;
    int registerSpaceFd_;

    static auto findDeviceByName(std::string_view name) -> std::filesystem::path;

  public:
    UioDevice(std::string_view name, const char* path, std::size_t registerSpaceSize);
    ~UioDevice();

    static auto fromName(std::string_view name, std::size_t registerSpaceSize) -> UioDevice;

    auto readRegister(std::size_t address) -> std::uint32_t;
    void writeRegister(std::size_t address, std::uint32_t value);
};
