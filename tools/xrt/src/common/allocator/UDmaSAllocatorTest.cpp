#include <common/allocator/UDmaSAllocator.hpp>
#include <cstdint>
#include <random>

#include <gtest/gtest.h>

class UDmaSAllocatorTest : public testing::Test {
 protected:
  UDmaSAllocator allocator_;
  bool skip_ = false;
  std::mt19937 randomGenerator_;

  UDmaSAllocatorTest() {
    if (!UDmaSAllocator::haveUDma()) {
      std::cerr << "             No u-dma-bug-mgr. Skipping test." << std::endl;
      skip_ = true;
    }

    // TODO: seed this by reading /dev/urandom
    srand(time(NULL));
    randomGenerator_.seed(rand());
  }
};

TEST_F(UDmaSAllocatorTest, BasicAllocateDeallocate) {
  if (skip_) {
    SUCCEED();
    return;
  }

  allocator_.deallocate(allocator_.allocate(10));
  allocator_.deallocate(allocator_.allocate(20));
  allocator_.deallocate(allocator_.allocate(30));
  allocator_.deallocate(allocator_.allocate(1000));
  allocator_.deallocate(allocator_.allocate(10000));
  allocator_.deallocate(allocator_.allocate(100000));
  allocator_.deallocate(allocator_.allocate(UDmaMaxLargeObjectSize));
  allocator_.deallocate(allocator_.allocate(UDmaMaxSmallObjectSize));
  allocator_.deallocate(allocator_.allocate(UDmaMaxSmallObjectSize));
  allocator_.deallocate(allocator_.allocate(UDmaMaxSmallObjectSize));
  allocator_.deallocate(allocator_.allocate(UDmaMaxSmallObjectSize));
}

TEST_F(UDmaSAllocatorTest, ReadWriteBytes) {
  if(skip_) {
    SUCCEED();
    return;
  }

  auto byte1 = reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(1));
  auto byte2 = reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(1));
  auto byte3 = reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(1));
  auto byte4 = reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(1));
  auto byte5 = reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(1));

  *byte1 = 0x22;
  *byte2 = 0x32;
  *byte3 = 0x42;
  *byte4 = 0x52;
  *byte5 = 0x62;

  EXPECT_EQ(*byte1, 0x22);
  EXPECT_EQ(*byte2, 0x32);
  EXPECT_EQ(*byte3, 0x42);
  EXPECT_EQ(*byte4, 0x52);
  EXPECT_EQ(*byte5, 0x62);

  allocator_.deallocate(byte1);
  allocator_.deallocate(byte2);
  allocator_.deallocate(byte3);
  allocator_.deallocate(byte4);
  allocator_.deallocate(byte5);
}

TEST_F(UDmaSAllocatorTest, ReadWriteRandom) {
  if (skip_) {
      SUCCEED();
      return;
  }

  constexpr size_t numTransactions = 128;

  std::vector<std::pair<std::uint8_t *, size_t>> heapAllocated;
  std::vector<std::pair<volatile std::uint8_t *, size_t>> udmaAllocated;

  for (size_t i = 0; i < numTransactions; i++) {
    switch (randomGenerator_() % 10) {
      // 50% chance: add data
      case 0: case 1: case 2: case 3: case 4: {
          size_t objectSize;
          // 80% chance add small object
          if (randomGenerator_() % 10 < 8) {
              objectSize = randomGenerator_() % UDmaMaxSmallObjectSize + 1;
          
          // 20% chance add probably large object
          } else {
              objectSize = randomGenerator_() % UDmaMaxLargeObjectSize + 1;
          }

          heapAllocated.emplace_back(new std::uint8_t[objectSize], objectSize);
          for (size_t j = 0; j < objectSize; j++) {
              heapAllocated.back().first[j] = randomGenerator_();
          }

          udmaAllocated.emplace_back(reinterpret_cast<volatile std::uint8_t *>(allocator_.allocate(objectSize)), objectSize);
          for (size_t j = 0; j < objectSize; j++) {
              udmaAllocated.back().first[j] = heapAllocated.back().first[j];
          }
  
          for (size_t j = 0; j < heapAllocated.back().second; j++) {
              EXPECT_EQ(udmaAllocated.back().first[j], heapAllocated.back().first[j]) << "data mismatch after adding data";
          }

          EXPECT_EQ(udmaAllocated.size(), allocator_.getNumObjectsAllocated()) << "object count wrong after adding data";

          break;
      }
      

      // 30%: change data
      case 5: case 6: case 7: {
        if (heapAllocated.size() == 0) {
          break;
        }

        size_t objectIt = randomGenerator_() % heapAllocated.size();

        for (size_t j = 0; j < heapAllocated.at(objectIt).second; j++) {
            EXPECT_EQ(udmaAllocated.at(objectIt).first[j], heapAllocated.at(objectIt).first[j]) << "data mismatch before changing data";
        }

        for (size_t j = 0; j < heapAllocated.at(objectIt).second; j++) {
            // EXPECT_EQ(udmaAllocated.at(objectIt).first[j], heapAllocated.at(objectIt).first[j]) << "data mismatch while changing data";

            udmaAllocated.at(objectIt).first[j] = heapAllocated.at(objectIt).first[j] = randomGenerator_();
        }

        for (size_t j = 0; j < heapAllocated.at(objectIt).second; j++) {
            EXPECT_EQ(udmaAllocated.at(objectIt).first[j], heapAllocated.at(objectIt).first[j]) << "data mismatch after changing data";
        }

        break;
      }

      // 20%: remove data
      case 8: case 9: {
        if (heapAllocated.size() == 0) {
          break;
        }

        size_t objectIt = randomGenerator_() % heapAllocated.size();

        for (size_t j = 0; j < heapAllocated.at(objectIt).second; j++) {
            EXPECT_EQ(udmaAllocated.at(objectIt).first[j], heapAllocated.at(objectIt).first[j]) << "data mismatch before removing data";
        }

        delete [] heapAllocated.at(objectIt).first;
        heapAllocated.erase(heapAllocated.begin() + objectIt);

        allocator_.deallocate(udmaAllocated.at(objectIt).first);
        udmaAllocated.erase(udmaAllocated.begin() + objectIt);

        EXPECT_EQ(udmaAllocated.size(), allocator_.getNumObjectsAllocated()) << "object count wrong after removing data";
      }
    }
  }


  for (size_t i = 0; i < heapAllocated.size(); i++) {
    for (size_t j = 0; j < heapAllocated.at(i).second; j++) {
      EXPECT_EQ(heapAllocated.at(i).first[j], udmaAllocated.at(i).first[j]) << "data mismatch at final count";
    }
    allocator_.deallocate(udmaAllocated.at(i).first);
  }

  EXPECT_EQ(allocator_.getNumObjectsAllocated(), 0) << "number of objects allocated not 0 at the final count";
  EXPECT_EQ(allocator_.getNumSuperblocks(), 0) << "number of superblocks not 0 at the final count";
  EXPECT_EQ(allocator_.getNumBuffers(), 0) << "number of total buffers not 0 at the final count";
}
