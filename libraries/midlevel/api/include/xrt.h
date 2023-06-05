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

#ifdef XRT_DYNAMIC_LOW_LEVEL

extern void  (*xpu_load)(void *, const char *);
extern void *(*xpu_lowLevel)(void *, const char *);
extern void  (*xpu_runRuntime)(void *, void *, uint32_t, uint32_t*);
extern void  (*xpu_writeMatrixArray)(void *, uint32_t *,
                                 uint32_t, uint32_t,
                                 uint32_t, uint32_t,
                                 uint32_t, uint32_t,
                                 uint32_t);
extern void  (*xpu_readMatrixArray)(void *, uint32_t,
                                uint32_t, uint32_t,
                                int,
                                uint32_t*,
                                uint32_t, uint32_t,
                                uint32_t, uint32_t);

#else

XRT_EXTERN
void *xpu_init(bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget);

XRT_EXTERN
void xpu_load(void *_ctx, const char *_path);

XRT_EXTERN
void xpu_runRuntime(void *_ctx, void *_functionPtr, uint32_t _argc, uint32_t *_argv);

XRT_EXTERN
void *xpu_lowLevel(void *_ctx, const char *_path);

XRT_EXTERN
void xpu_writeMatrixArray(void *_ctx, uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLine, uint32_t _numColumn,
                              uint32_t _accMemStart);

XRT_EXTERN
void xpu_readMatrixArray(void *_ctx, uint32_t _accMemStart,
                             uint32_t _numLine, uint32_t _numColumn,
                             int      _accRequireResultReady,
                             uint32_t *_ramMatrix,
                             uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn);

#endif // XRT_DYNAMIC_LOW_LEVEL

#endif
