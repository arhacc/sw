#include <targets/fpga/Dma.hpp>

#include "common/allocator/SAllocator.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

#include <common/allocator/UDmaSAllocator.hpp>
#include <cstdint>
#include <memory>
#include <random>

class DmaTest : public testing::Test {
 protected:
  Dma dma_;
  bool skip_ = false;
  std::mt19937 randomGenerator_;

  DmaTest() {
    if (!UDmaSAllocator::haveUDma()) {
      std::cerr << "             No u-dma-bug-mgr. Skipping test." << std::endl;
      skip_ = true;
    }

    // TODO: seed this by reading /dev/urandom
    srand(time(NULL));
    randomGenerator_.seed(rand());

    gsAllocator = std::make_unique<UDmaSAllocator>();
  }

  ~DmaTest() {
    gsAllocator = nullptr;
  }
};

class DmaBufTest : public testing::TestWithParam<std::size_t> {
 protected:
  Dma dma_;
  bool skip_ = false;
  std::mt19937 randomGenerator_;

  volatile uint64_t *data;
  std::uintptr_t physAddress;
  std::size_t n = GetParam();

  DmaBufTest() {
    if (!UDmaSAllocator::haveUDma()) {
      std::cerr << "             No u-dma-bug-mgr. Skipping test." << std::endl;
      skip_ = true;
    }

    // TODO: seed this by reading /dev/urandom
    srand(time(NULL));
    randomGenerator_.seed(rand());

    gsAllocator = std::make_unique<UDmaSAllocator>();

    data = reinterpret_cast<volatile uint64_t *>(gsAllocator->allocate(n * sizeof(uint64_t)));
    physAddress = gsAllocator->getPhysicalAddress(reinterpret_cast<volatile void *>(data));

    for (std::size_t i = 0; i < n; i++) {
        data[i] = i + 5;
    }
  }

  ~DmaBufTest() {
    gsAllocator->deallocate(data);

    gsAllocator = nullptr;
  }
};

static constexpr std::size_t basicWriteReadbackSizes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
INSTANTIATE_TEST_SUITE_P(DmaBufTestSuite, DmaBufTest, testing::ValuesIn(basicWriteReadbackSizes));

