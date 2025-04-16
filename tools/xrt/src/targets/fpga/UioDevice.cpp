#include <common/log/Logger.hpp>
#include <targets/fpga/UioDevice.hpp>

#include <cstring>
#include <stdexcept>
#include <fstream>

#include <fcntl.h>
#include <fmt/format.h>
#include <sys/mman.h>
#include <unistd.h>


UioDevice::UioDevice(const std::string_view name, const char *path, const std::size_t registerSpaceSize)
  : name_{name}, registerSpaceSize_{registerSpaceSize} {

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
    if (registerSpace_ == nullptr) {
        const auto errorMsg = fmt::format("Failed to mmap {} for DMA: {}", path, strerror(errno));
        close(registerSpaceFd_);
        throw std::runtime_error(errorMsg);
    }
}

UioDevice::~UioDevice() {
    munmap((void*) registerSpace_, registerSpaceSize_);
    close(registerSpaceFd_);
}

auto UioDevice::fromName(const std::string_view name, const std::size_t registerSpaceSize) -> UioDevice {
    return {name, findDeviceByName(name).c_str(), registerSpaceSize};
}

auto UioDevice::findDeviceByName(const std::string_view name) -> std::filesystem::path {
    for (const auto& entry : std::filesystem::directory_iterator(cUioPath)) {
        if (is_directory(entry)) {
            std::filesystem::path filePath = entry.path() / "name";
            if (std::ifstream file(filePath); file.is_open()) {
                std::string line;
                std::getline(file, line);
                if (line == name) {
                    auto path = std::filesystem::path("/dev") / filePath.filename();
                    logWork.println<InfoHigh>("Found uio device {} at {}", name, path.string());
                    return path;
                }
            }
        }
    }
    throw std::runtime_error(fmt::format("could not find uio device: {}"))

}

auto UioDevice::readRegister(std::size_t address) -> std::uint32_t {
    auto value = registerSpace_[address / 4];

    logWork.println<Debug>("Read from {} at address 0x{:X} -> {} (0x{:X})", name_, address, value, value);

    return value;
}

void UioDevice::writeRegister(std::size_t address, std::uint32_t value) {
    logWork.println<Debug>("Write to {} at address 0x{:X} -> {} (0x{:X})", name_, address, value, value);

    registerSpace_[address / 4] = value;
}
