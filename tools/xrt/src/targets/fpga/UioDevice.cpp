#include <targets/fpga/UioDevice.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>

#include <fmt/format.h>

#include <sys/mman.h>

UioDevice::UioDevice(const char *path, std::size_t registerSpaceSize)
  : registerSpaceSize_(registerSpaceSize) {

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
    return registerSpace_[address / 4];
}

void UioDevice::writeRegister(std::size_t address, std::uint32_t value) {
    registerSpace_[address / 4] = value;
}
