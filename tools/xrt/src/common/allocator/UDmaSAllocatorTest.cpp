#include <common/allocator/UDmaSAllocator.hpp>

#include <gtest/gtest.h>

class UDmaSAllocatorTest : public testing::Test {
 protected:
  UDmaSAllocator allocator_;
  bool skip_ = false;

  UDmaSAllocatorTest() {
    if (!UDmaSAllocator::haveUDma()) {
      std::cerr << "             No u-dma-bug-mgr. Skipping test." << std::endl;
      skip_ = true;
    }
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
