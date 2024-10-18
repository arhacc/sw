//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/allocator/SAllocator.hpp>

#include <array>
#include <vector>

constexpr size_t UDmaSuperblockSizePow2 = 20; // 1MiB
constexpr size_t UDmaSuperblockSize = 1 << UDmaSuperblockSizePow2;

constexpr size_t UDmaMinSmallObjectSizePow2 = 8; // 64
constexpr size_t UDmaMinSmallObjectSize = 1 << UDmaMinSmallObjectSizePow2;

constexpr size_t UDmaMaxSmallObjectSizePow2 = UDmaSuperblockSizePow2 - 1; // half of superblock max size
constexpr size_t UDmaMaxSmallObjectSize = UDmaSuperblockSize / 2;

constexpr size_t UDmaNumberOfSmallObjectSizes = UDmaMaxSmallObjectSizePow2 - UDmaMinSmallObjectSizePow2 + 1;

constexpr size_t UDmaMaxLargeObjectSize = 4 * 1024 * 1024; // 4MiB


class UDmaRawBuffer {
    volatile void *data;
    int fd;
    uintptr_t physaddr;
    size_t size;
    unsigned i;
public:
    UDmaRawBuffer(unsigned i, size_t size);
    virtual ~UDmaRawBuffer();

    volatile void *getData();
    size_t         getSize();
    uintptr_t      getPhysicalAddress(volatile void *);
};


class UDmaSuperblock : public UDmaRawBuffer {
    size_t objectSize;
    std::vector<bool> allocated;

public:
    UDmaSuperblock(unsigned _i, size_t objectSize);

    volatile void *allocate();
    bool deallocate(volatile void *);
};


class UDmaSAllocator : public SAllocator {
  std::array<std::vector<std::unique_ptr<UDmaSuperblock>>, UDmaNumberOfSmallObjectSizes> superblocks;

  std::vector<std::unique_ptr<UDmaRawBuffer>> largeObjectBuffers;

  volatile void *allocateSmallObject(size_t _nBytes);
  volatile void *allocateLargeObject(size_t _nBytes);

  unsigned i = 0;

  public:
    UDmaSAllocator() = default;
~UDmaSAllocator() override = default;

volatile void *allocate(size_t _nBytes) override;
    void deallocate(volatile void *) override;
    uintptr_t getPhysicalAddress(volatile void *) override;
};
