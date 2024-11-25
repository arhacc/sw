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
