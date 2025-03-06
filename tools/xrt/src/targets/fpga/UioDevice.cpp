#include <targets/fpga/UioDevice.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>

#include <common/log/Logger.hpp>

#include <fmt/format.h>

#include <sys/mman.h>

UioDevice::UioDevice(std::string_view name, const char *path, std::size_t registerSpaceSize)
  : name_(name), registerSpaceSize_(registerSpaceSize) {

    registerSpaceFd_ = open(path, O_RDWR);
    if (registerSpaceFd_ < 0) {
        throw std::runtime_error(fmt::format("Failed to open {} for DMA: {}", path, strerror(errno)));
    }

    registerSpace_ = (volatile uint32_t *) mmap(
        nullptr,
        registerSpaceSize_,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        registerSpaceFd_,
        0
    );
    if (registerSpace_ == NULL) {
        auto errorMsg = fmt::format("Failed to mmap {} for DMA: {}", path, strerror(errno));
        close(registerSpaceFd_);
        throw std::runtime_error(errorMsg);
    }
}

UioDevice::~UioDevice() {
    munmap((void *) registerSpace_, registerSpaceSize_);
    close(registerSpaceFd_);
}

std::uint32_t UioDevice::readRegister(std::size_t address) {
    auto value = registerSpace_[address / 4];

    logWork.println<Debug>("Read from {} at address 0x{:X} -> {} (0x{:X})", name_, address, value, value);

    return value;
}

void UioDevice::writeRegister(std::size_t address, std::uint32_t value) {
    logWork.println<Debug>("Write to {} at address 0x{:X} -> {} (0x{:X})", name_, address, value, value);

    registerSpace_[address / 4] = value;
}
