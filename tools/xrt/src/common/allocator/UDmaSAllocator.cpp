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
#include <numeric>
#include <stdexcept>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <fmt/format.h>

// -------- UDmaRawBuffer static

std::string UDmaRawBuffer::gName;
int UDmaRawBuffer::gFd;
volatile void *UDmaRawBuffer::gData = NULL;
uintptr_t UDmaRawBuffer::gPhysAddr;
size_t UDmaRawBuffer::gSize;
std::vector<UDmaRawBuffer::Allocated> UDmaRawBuffer::gAllocatedBuffers;

void UDmaRawBuffer::gInitIfNeeded() {
    if (gData == NULL) {
        gInit();
    }
}

void UDmaRawBuffer::gInit() {
    gName = "udmabuf0";
    
    gFd = gInitGetFd(gName);
    gInitSetSyncMode(gName);
    gPhysAddr = gInitGetPhysAddr(gName);
    gSize = gInitGetSize(gName);
    gData = gInitGetData(gName, gFd, gSize);
    gAllocatedBuffers = gInitCreateAllocatedBuffers(gSize);
}

int UDmaRawBuffer::gInitGetFd(std::string_view name) {
    if (!std::filesystem::exists(fmt::format("/dev/{}", name))) {
        throw std::runtime_error(fmt::format("/dev/{} not found; please load the udmabuf module", name));
    }

    int fd = open(fmt::format("/dev/{}", name).c_str(), O_RDWR | O_SYNC);
    if (fd < 0) {
        throw std::runtime_error(fmt::format("failed to open /dev/{}: {}", name, strerror(errno)));
    }

    return fd;
}

void UDmaRawBuffer::gInitSetSyncMode(std::string_view name) {
    std::ofstream _uDmaBufSyncModeFile(fmt::format("/sys/class/u-dma-buf/{}/sync_mode", name));

    _uDmaBufSyncModeFile << "1";
    if (!_uDmaBufSyncModeFile.good()) {
        throw std::runtime_error(fmt::format("failed to set sync_mode for {}", name));
    }

    _uDmaBufSyncModeFile.close();
}

volatile void *UDmaRawBuffer::gInitGetData(std::string_view name, int fd, std::size_t size) {
    volatile void *data;

    data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        throw std::runtime_error(fmt::format("failed to mmap {}: {}", name, strerror(errno)));
    }

    return data;
}

uintptr_t UDmaRawBuffer::gInitGetPhysAddr(std::string_view name) {
    uintptr_t physaddr;

    std::ifstream _uDmaBufPhysAddrFile(fmt::format("/sys/class/u-dma-buf/{}/phys_addr", name));

    _uDmaBufPhysAddrFile >> std::hex >> physaddr;
    if (!_uDmaBufPhysAddrFile.good()) {
        throw std::runtime_error(fmt::format("failed to read phys_addr for {}", name));
    }

    _uDmaBufPhysAddrFile.close();

    return physaddr;
}

size_t UDmaRawBuffer::gInitGetSize(std::string_view name) {
    size_t size;

    std::ifstream _uDmaBufSizeFile(fmt::format("/sys/class/u-dma-buf/{}/size", name));

    _uDmaBufSizeFile >> size;
    if (!_uDmaBufSizeFile.good()) {
        throw std::runtime_error(fmt::format("failed to read size for {}", name));
    }

    _uDmaBufSizeFile.close();

    return size;
}

auto UDmaRawBuffer::gInitCreateAllocatedBuffers(size_t size) -> std::vector<Allocated> {
    std::vector<Allocated> allocatedBuffers;
    
    allocatedBuffers.resize(size / UDmaSuperblockSize);
    if (allocatedBuffers.size() == 0) {
        throw std::runtime_error("dma buffer too small");
    }

    std::fill(allocatedBuffers.begin(), allocatedBuffers.end(), Allocated::E);

    return allocatedBuffers;
}

bool UDmaRawBuffer::gAllocateRawBuffer(std::size_t size, volatile void **data, uintptr_t *physaddr, size_t *idx) {
    std::size_t numbuffers = (size - 1) / UDmaSuperblockSize + 1;

    for (std::size_t i = 0; i <= gAllocatedBuffers.size() - numbuffers; i++) {
        if (gAllocatedBuffers[i] == Allocated::E) {
            bool ok = true;
            for (std::size_t j = i + 1; j < i + numbuffers; j++) {
                if (gAllocatedBuffers[j] != Allocated::E) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                gAllocatedBuffers[i] = Allocated::B;
                for (std::size_t j = i + 1; j < i + numbuffers; j++) {
                    gAllocatedBuffers[j] = Allocated::C;
                }

                *data = ((volatile std::uint8_t *) gData) + i * UDmaSuperblockSize;
                *physaddr = gPhysAddr + i * UDmaSuperblockSize;
                *idx = i;

                return true;
            }
        }
    }

    return false;
}

void UDmaRawBuffer::gDeallocateRawBuffer(std::size_t idx) {
    if (gAllocatedBuffers.at(idx) != Allocated::B) {
        throw std::runtime_error("bad dealloc");
    }

    gAllocatedBuffers.at(idx) = Allocated::E;

    idx++;
    while (idx < gAllocatedBuffers.size() && gAllocatedBuffers.at(idx) == Allocated::C) {
        gAllocatedBuffers.at(idx) = Allocated::E;
        idx++;
    }
}

// -------- UDmaRawBuffer

UDmaRawBuffer::UDmaRawBuffer(std::size_t _objectSize, std::size_t size) : size(size) {
    if (!gAllocateRawBuffer(size, &data, &physaddr, &idx)) {
        throw std::runtime_error("could not allocate: buffer full");
    }
}

UDmaRawBuffer::~UDmaRawBuffer() {
    gDeallocateRawBuffer(idx);
}

volatile void *UDmaRawBuffer::getData() {
    return data;
}

size_t UDmaRawBuffer::getSize() {
    return size;
}

uintptr_t UDmaRawBuffer::getPhysicalAddress(volatile void *_a) {
    auto _pos = (size_t)((unsigned char *) _a - (unsigned char *) data);

    if (_pos >= size) {
        return 0;
    }

    return physaddr + _pos;
}

// -------- UDmaSuperblock

UDmaSuperblock::UDmaSuperblock(size_t _objectSize)
  : UDmaRawBuffer(_objectSize, UDmaSuperblockSize),
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

    objectsAllocated++;
    return (volatile void *) ((unsigned char *) getData() + pos * objectSize);
}

bool UDmaSuperblock::deallocate(volatile void *_object) {
    auto _pos = (size_t)((unsigned char *) _object - (unsigned char *) getData()) / objectSize;

    if (_pos < allocated.size()) {
        allocated[_pos] = false;

        objectsAllocated--;
        return true;
    } else {
        return false;
    }
}

size_t UDmaSuperblock::getNumObjectsAllocated() {
    return objectsAllocated;
}

// -------- UDmaSuperblockBucket

UDmaSuperblockBucket::UDmaSuperblockBucket(size_t objectSize)
    : objectSize(objectSize)
{}
UDmaSuperblockBucket::~UDmaSuperblockBucket() = default;

volatile void* UDmaSuperblockBucket::allocate() {
    volatile void *ptr;

    // try existing superblocks
    for (auto& superblock : superblocks) {
        if ((ptr = superblock->allocate()) != nullptr) {
            return ptr;
        }
    }

    // create a new superblock
    superblocks.push_back(std::make_unique<UDmaSuperblock>(objectSize));
    ptr = superblocks.back()->allocate();
    assert(ptr != nullptr);
    return ptr;
}

bool UDmaSuperblockBucket::deallocate(volatile void *ptr) {
    for (auto superblockIt = superblocks.begin(); superblockIt != superblocks.end(); superblockIt++) {
        if ((*superblockIt)->deallocate(ptr)) {
            if ((*superblockIt)->getNumObjectsAllocated() == 0) {
                superblocks.erase(superblockIt);
            }

            return true;
        }
    }

    return false;
}

uintptr_t UDmaSuperblockBucket::getPhysicalAddress(volatile void *ptr) {
    uintptr_t addr;
    for (auto& superblock : superblocks) {
        if ((addr = superblock->getPhysicalAddress(ptr)) != 0) {
            return addr;
        }
    }

    return 0;
}

size_t UDmaSuperblockBucket::getNumSuperblocks() {
    return superblocks.size();
}

size_t UDmaSuperblockBucket::getNumObjectsAllocated() {
    return std::accumulate(
        superblocks.begin(),
        superblocks.end(),
        0,
        [](size_t acc, std::unique_ptr<UDmaSuperblock>& superblock) -> size_t {
            return acc + superblock->getNumObjectsAllocated();
    });
}

// -------- UDmaSAllocator

UDmaSAllocator::UDmaSAllocator() {
    for (size_t objectSizePow2 = UDmaMinSmallObjectSizePow2; objectSizePow2 <= UDmaMaxSmallObjectSizePow2; objectSizePow2++) {
        buckets.emplace_back(1 << objectSizePow2);
    }

    assert(buckets.size() == UDmaNumberOfSmallObjectSizes);
    
    for (auto const& device : std::filesystem::directory_iterator{"/dev"}) {
        if (device.path().filename().string().starts_with("udmabuf-xpu-")) {
            std::ofstream _uDmaBufManager("/dev/u-dma-buf-mgr");

            logWork.println<InfoHigh>("delete {}", device.path().filename().string());
            _uDmaBufManager << fmt::format("delete {}", device.path().filename().string()) << std::endl;
            _uDmaBufManager.close();
        }
    }

}

UDmaSAllocator::~UDmaSAllocator() = default;

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

    assert(_superblockIndex < buckets.size());

    auto &_bucket = buckets[_superblockIndex];
    return _bucket.allocate();
}

volatile void *UDmaSAllocator::allocateLargeObject(size_t _nBytes) {
    largeObjectBuffers.push_back(std::make_unique<UDmaRawBuffer>(0, _nBytes));

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
    for (auto &_bucket : buckets) {
        if (_bucket.deallocate(_a)) {
            return;
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
    for (auto &_bucket : buckets) {
        if ((_p = _bucket.getPhysicalAddress(_a)) != 0) {
            return _p;
        }
    }

    throw std::runtime_error("corrupt getPhysicalAddress");
}

bool UDmaSAllocator::haveUDma() {
    return std::filesystem::exists("/dev/u-dma-buf-mgr");
}

size_t UDmaSAllocator::getNumObjectsAllocated() {
    return std::accumulate(
        buckets.begin(),
        buckets.end(),
        0,
        [](size_t acc, UDmaSuperblockBucket& bucket) -> size_t {
            return acc + bucket.getNumObjectsAllocated();
    }) + largeObjectBuffers.size();
}

size_t UDmaSAllocator::getNumSuperblocks() {
    return std::accumulate(
        buckets.begin(),
        buckets.end(),
        0,
        [](size_t acc, UDmaSuperblockBucket& bucket) -> size_t {
            return acc + bucket.getNumSuperblocks();
    });
}

size_t UDmaSAllocator::getNumBuffers() {
    return getNumSuperblocks() + largeObjectBuffers.size();
}
