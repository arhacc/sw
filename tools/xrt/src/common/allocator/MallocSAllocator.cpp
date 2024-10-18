//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/allocator/MallocSAllocator.hpp>

#include <cstdlib>

volatile void *MallocSAllocator::allocate(size_t _nBytes) {
  return std::malloc(_nBytes);
}

void MallocSAllocator::deallocate(volatile void *_object) {
  std::free(const_cast<void *>(_object));
}
