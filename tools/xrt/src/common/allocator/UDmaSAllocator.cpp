//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/allocator/UDmaSAllocator.hpp>
#include <common/log/Logger.hpp>

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <fmt/format.h>

UDmaRawBuffer::UDmaRawBuffer(unsigned _i, size_t _size) : size(_size), i(_i) {
    if (!std::filesystem::exists("/dev/u-dma-buf-mgr")) {
        throw std::runtime_error("/dev/u-dma-buf-mgr not found; please load the u-dma-buf-mgr module");
    }

    std::ofstream _uDmaBufManager("/dev/u-dma-buf-mgr");

    logWork.println<InfoHigh>("create udmabuf{} 0x{:x}", _i, _size);
    _uDmaBufManager << fmt::format("create udmabuf{} 0x{:x}", _i, _size) << std::endl;

    if (!_uDmaBufManager.good()) {
       throw std::runtime_error(fmt::format("failed to create udmabuf{} of size {}", _i, _size)); 
    }

    _uDmaBufManager.close();

    fd = open(fmt::format("/dev/udmabuf{}", _i).c_str(), O_RDWR | O_SYNC);
    if (fd < 0) {
        throw std::runtime_error(fmt::format("failed to open /dev/udmabuf{}: {}", _i, strerror(errno)));
    }

    std::ofstream _uDmaBufSyncModeFile(fmt::format("/sys/class/u-dma-buf/udmabuf{}/sync_mode", _i));
    _uDmaBufSyncModeFile << "1";
    if (!_uDmaBufSyncModeFile.good()) {
        throw std::runtime_error(fmt::format("failed to set sync_mode for udmabuf{}", _i));
    }
    _uDmaBufSyncModeFile.close();

    data = mmap(nullptr, _size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        throw std::runtime_error(fmt::format("failed to mmap udmabuf{}: {}", _i, strerror(errno)));
    }

    std::ifstream _uDmaBufPhysAddrFile(fmt::format("/sys/class/u-dma-buf/udmabuf{}/phys_addr", _i));
    _uDmaBufPhysAddrFile >> std::hex >> physaddr;
    if (!_uDmaBufPhysAddrFile.good()) {
        throw std::runtime_error(fmt::format("failed to read phys_addr for udmabuf{}", _i));
    }
    _uDmaBufPhysAddrFile.close();
}

UDmaRawBuffer::~UDmaRawBuffer() {
    munmap(const_cast<void *>(data), size);
    close(fd);

    std::ofstream _uDmaBufManager("/dev/u-dma-buf-mgr");

    // logWork.println<InfoHigh>("delete udmabuf{}", i);
    _uDmaBufManager << fmt::format("delete udmabuf{}", i) << std::endl;
    _uDmaBufManager.close();
}

volatile void *UDmaRawBuffer::getData() {
    return data;
}

size_t UDmaRawBuffer::getSize() {
    return size;
}

uintptr_t UDmaRawBuffer::getPhysicalAddress(volatile void *_a) {
    auto _pos = (size_t)((unsigned char *) _a - (unsigned char *) data);

    if (_pos > size) {
        return 0;
    }

    return physaddr + _pos;
}

UDmaSuperblock::UDmaSuperblock(unsigned _i, size_t _objectSize)
  : UDmaRawBuffer(_i, UDmaSuperblockSize),
    objectSize(_objectSize),
    allocated(UDmaSuperblockSize / _objectSize, false)
{}

volatile void *UDmaSuperblock::allocate() {
    auto free = std::find(allocated.begin(), allocated.end(), false);

    if (free == allocated.end()) {
        return nullptr;
    }

    auto pos = std::distance(allocated.begin(), free);

    *free = true;

    return (volatile void *) ((unsigned char *) getData() + pos * objectSize);
}

bool UDmaSuperblock::deallocate(volatile void *_object) {
    auto _pos = (size_t)((unsigned char *) _object - (unsigned char *) getData()) / objectSize;

    if (_pos < allocated.size()) {
        allocated[_pos] = false;
        return true;
    } else {
        return false;
    }
}

volatile void *UDmaSAllocator::allocate(size_t _numBytes) {
    size_t _effectiveNumBytes = std::bit_ceil(_numBytes);

    if (_effectiveNumBytes <= UDmaMaxSmallObjectSize) {
        return allocateSmallObject(_effectiveNumBytes);
    } else if (_effectiveNumBytes <= UDmaMaxLargeObjectSize) {
        return allocateLargeObject(_effectiveNumBytes);
    } else {
        throw std::runtime_error(fmt::format("Allocation of object of {} bytes more than maximum {} bytes", _effectiveNumBytes, UDmaMaxLargeObjectSize));
    }
}

volatile void *UDmaSAllocator::allocateSmallObject(size_t _nBytes) {
    if (_nBytes < UDmaMinSmallObjectSize) {
       _nBytes = UDmaMinSmallObjectSize;
    }

    assert(std::popcount(_nBytes) == 1);

    size_t _sizePow2 = std::countr_zero(_nBytes);
    size_t _superblockIndex = _sizePow2 - UDmaMinSmallObjectSizePow2;

    assert(_superblockIndex < superblocks.size());

    // try to allocate in an existing superblock for that size
    auto &_superblockBucket = superblocks[_superblockIndex];
    for (auto &_superblock : _superblockBucket) {
        volatile void *_a;
        if ((_a = _superblock->allocate()) != nullptr) {
            return _a;
        }
    }

    // if not, create a new superblock
    _superblockBucket.push_back(std::make_unique<UDmaSuperblock>(i++, _nBytes));
    volatile void *_a = _superblockBucket.back()->allocate();
    assert(_a != nullptr); // superblock should be empty when new
    return _a;
}

volatile void *UDmaSAllocator::allocateLargeObject(size_t _nBytes) {
    largeObjectBuffers.push_back(std::make_unique<UDmaRawBuffer>(i++, _nBytes));

    return largeObjectBuffers.back()->getData();
}

void UDmaSAllocator::deallocate(volatile void *_a) {
    // try large objects
    for (auto _largeObjectBuffer = largeObjectBuffers.begin(); _largeObjectBuffer < largeObjectBuffers.end(); _largeObjectBuffer++) {
        if ((*_largeObjectBuffer)->getData() == _a) {
            largeObjectBuffers.erase(_largeObjectBuffer);
            return;
        }
    }

    // try small objects
    for (auto &_superblockBucket : superblocks) {
        for (auto &_superblock : _superblockBucket) {
            if (_superblock->deallocate(_a)) {
                return;
            }
        }
    }

    throw std::runtime_error("corrupt free");
}

uintptr_t UDmaSAllocator::getPhysicalAddress(volatile void *_a) {
    uintptr_t _p;      

    // try large objects
    for (auto &_largeObjectBuffer : largeObjectBuffers) {
        if ((_p = _largeObjectBuffer->getPhysicalAddress(_a)) != 0) {
            return _p;
        }
    }

    // try small objects
    for (auto &_superblockBucket : superblocks) {
        for (auto &_superblock : _superblockBucket) {
            if ((_p = _superblock->getPhysicalAddress(_a)) != 0) {
                return _p;
            }
        }
    }

    throw std::runtime_error("corrupt getPhysicalAddress");
}

bool UDmaSAllocator::haveUDma() {
    return std::filesystem::exists("/dev/u-dma-buf-mgr");
}
