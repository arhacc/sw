//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/allocator/SAllocator.hpp>

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
    enum class Allocated : std::uint8_t {
        E,
        B,
        C,
    };
    
    static std::string gName;
    static int gFd;
    static volatile void *gData;
    static uintptr_t gPhysAddr;
    static size_t gSize;
    static std::vector<Allocated> gAllocatedBuffers; 

    volatile void *data;
    std::uintptr_t physaddr;
    std::size_t size;
    std::size_t idx;
    std::string name;

    static void gInitIfNeeded();
    static void gInit();

    static int gInitGetFd(std::string_view name);
    static void gInitSetSyncMode(std::string_view name);
    static uintptr_t gInitGetPhysAddr(std::string_view name);
    static size_t gInitGetSize(std::string_view name);
    static volatile void *gInitGetData(std::string_view name, int fd, std::size_t size);
    static std::vector<Allocated> gInitCreateAllocatedBuffers(std::size_t size);

    static bool gAllocateRawBuffer(std::size_t size, volatile void **data, uintptr_t *physaddr, std::size_t *idx);
    static void gDeallocateRawBuffer(std::size_t idx);
public:

    
    UDmaRawBuffer(size_t _objectSize, size_t size);
    virtual ~UDmaRawBuffer();

    UDmaRawBuffer(const UDmaRawBuffer&) = delete;
    UDmaRawBuffer(UDmaRawBuffer&&) = default;
    UDmaRawBuffer& operator=(const UDmaRawBuffer&) = delete;
    UDmaRawBuffer& operator=(UDmaRawBuffer&&) = default;

    volatile void *getData();
    size_t         getSize();
    uintptr_t      getPhysicalAddress(volatile void *);
};


class UDmaSuperblock : public UDmaRawBuffer {
    size_t objectSize;
    std::vector<bool> allocated;

    size_t objectsAllocated = 0;

public:
    UDmaSuperblock(size_t _objectSize);

    UDmaSuperblock(const UDmaSuperblock&) = delete;
    UDmaSuperblock(UDmaSuperblock&&) = default;
    UDmaSuperblock& operator=(const UDmaSuperblock&) = delete;
    UDmaSuperblock& operator=(UDmaSuperblock&&) = default;

    volatile void *allocate();
    bool deallocate(volatile void *);
    size_t getNumObjectsAllocated();
};

class UDmaSuperblockBucket {
    std::vector<std::unique_ptr<UDmaSuperblock>> superblocks;

    size_t objectSize;
public:
    UDmaSuperblockBucket(size_t objectSize);
    ~UDmaSuperblockBucket();

    UDmaSuperblockBucket(const UDmaSuperblockBucket&) = delete;
    UDmaSuperblockBucket(UDmaSuperblockBucket&&) = default;
    UDmaSuperblockBucket& operator=(const UDmaSuperblockBucket&) = delete;
    UDmaSuperblockBucket& operator=(UDmaSuperblockBucket&&) = default;

    volatile void *allocate();
    bool deallocate(volatile void *);
    size_t getNumObjectsAllocated();
    size_t getNumSuperblocks();
    uintptr_t getPhysicalAddress(volatile void *);
};


class UDmaSAllocator : public SAllocator {
  // std::array<UDmaSuperblockBucket, UDmaNumberOfSmallObjectSizes> buckets;
  std::vector<UDmaSuperblockBucket> buckets;

  std::vector<std::unique_ptr<UDmaRawBuffer>> largeObjectBuffers;

  volatile void *allocateSmallObject(size_t _nBytes);
  volatile void *allocateLargeObject(size_t _nBytes);

  unsigned i = 0;

  public:
    UDmaSAllocator();
    ~UDmaSAllocator() override;

    UDmaSAllocator(const UDmaSAllocator&) = delete;
    UDmaSAllocator(UDmaSAllocator&&) = default;
    UDmaSAllocator& operator=(const UDmaSAllocator&) = delete;
    UDmaSAllocator& operator=(UDmaSAllocator&&) = default;

    volatile void *allocate(size_t _nBytes) override;
    void deallocate(volatile void *) override;
    uintptr_t getPhysicalAddress(volatile void *) override;

    static bool haveUDma();

    // use only in tests
    size_t getNumObjectsAllocated();
    size_t getNumSuperblocks();
    size_t getNumBuffers();
};
