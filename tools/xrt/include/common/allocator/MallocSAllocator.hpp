//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/allocator/SAllocator.hpp>

class MallocSAllocator : public SAllocator {
  public:
    MallocSAllocator() = default;
    ~MallocSAllocator() override = default;

    volatile void *allocate(size_t _nBytes) override;
    void deallocate(volatile void *) override;
};
