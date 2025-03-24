//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <memory>

// Base class for allocator for objects that can be shared with the accelerator (matrices, tensors)
// Most objects, that do not need to be shared, should be allocated with standard new/malloc etc.
//
// Users should use the gsAllocator object and not create their own allocators.

class SAllocator {
  protected:
    SAllocator() = default;
  public:
    virtual ~SAllocator() = default;

  virtual volatile void *allocate(size_t _nBytes) = 0;
  virtual void deallocate(volatile void *) = 0;
  virtual uintptr_t getPhysicalAddress(volatile const void *); // returns 0 if the allocator is not for DMA
};

extern std::unique_ptr<SAllocator> gsAllocator;

enum class SAllocatorType {
  Malloc,
  UDma,
};

void initGSAllocator(SAllocatorType _type);
void destroyGSAllocator();

