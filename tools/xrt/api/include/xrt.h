//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#ifndef XRT_API_HEADER
#define XRT_API_HEADER

#include <stdint.h>

#ifdef __cplusplus
#define XRT_EXTERN extern "C"
#else
#define XRT_EXTERN
#endif

extern void  (*load)(const char *);
extern void *(*lowLevel)(const char *);
extern void  (*runRuntime)(void *);
extern void  (*writeMatrixArray)(uint32_t *,
                                 uint32_t, uint32_t,
                                 uint32_t, uint32_t,
                                 uint32_t, uint32_t,
                                 uint32_t);
extern void  (*readMatrixArray)(uint32_t,
                                uint32_t, uint32_t,
                                int,
                                uint32_t*,
                                uint32_t, uint32_t,
                                uint32_t, uint32_t);

#endif
