//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>

void  (*load)(const char *) = NULL;
void *(*lowLevel)(const char *) = NULL;
void  (*runRuntime)(void *, uint32_t, uint32_t*) = NULL;

void (*writeMatrixArray)(uint32_t *,
                         uint32_t, uint32_t,
                         uint32_t, uint32_t,
                         uint32_t, uint32_t,
                         uint32_t) = NULL;

void (*readMatrixArray)(uint32_t,
                        uint32_t, uint32_t,
                        int,
                        uint32_t*,
                        uint32_t, uint32_t,
                        uint32_t, uint32_t) = NULL;
