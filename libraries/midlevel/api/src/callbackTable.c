//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>

#ifdef XRT_DYNAMIC_MID_LEVEL

void  (*load)(void *, const char *) = NULL;
void *(*lowLevel)(void *, const char *) = NULL;
void  (*runRuntime)(void *, void *, uint32_t, uint32_t*) = NULL;

void (*writeMatrixArray)(void *, uint32_t *,
                         uint32_t, uint32_t,
                         uint32_t, uint32_t,
                         uint32_t, uint32_t,
                         uint32_t) = NULL;

void (*readMatrixArray)(void *, uint32_t,
                        uint32_t, uint32_t,
                        int,
                        uint32_t*,
                        uint32_t, uint32_t,
                        uint32_t, uint32_t) = NULL;

#endif
