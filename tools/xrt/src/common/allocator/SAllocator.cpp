//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/allocator/SAllocator.hpp>
#include <common/allocator/MallocSAllocator.hpp>
#include <common/allocator/UDmaSAllocator.hpp>
#include <memory>



uintptr_t SAllocator::getPhysicalAddress(volatile void *) {
    return 0;
}

std::unique_ptr<SAllocator> gsAllocator;

void initGSAllocator(SAllocatorType _type) {
  switch (_type) {
    case SAllocatorType::Malloc:
      gsAllocator = std::make_unique<MallocSAllocator>();
    case SAllocatorType::UDma:
      gsAllocator = std::make_unique<UDmaSAllocator>();
  }
}
