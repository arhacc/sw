//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#ifndef XRT_API_HEADER
#define XRT_API_HEADER

#include <stdbool.h>
#include <stdint.h>

typedef void* XRT_CONTEX_HANDLE;
typedef void* XRT_FUNCTION_HANDLE;

#ifdef __cplusplus
#define XRT_EXTERN extern "C"
#else
#define XRT_EXTERN
#endif

#ifdef XRT_DYNAMIC_LOW_LEVEL

extern
void (*xpu_load)(XPU_CONTEX_HANDLE _ctx, const char *_path);

extern
XRT_FUNCTION_HANDLE (*xpu_lowLevel)(XPU_CONTEX_HANDLE _ctx, const char *_path);

extern
void (*xpu_runRuntime)(XPU_CONTEX_HANDLE _ctx, XRT_FUNCTION_HANDLE _function, uint32_t _argc, uint32_t *_argv);

extern
void (*xpu_writeMatrixArray)(XPU_CONTEX_HANDLE _ctx,
                              uint32_t _accMemStart,
                              uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLines, uint32_t _numColumns);

extern
void (*xpu_readMatrixArray)(XPU_CONTEX_HANDLE _ctx,
                            uint32_t _accMemStart,
                            uint32_t *_ramMatrix,
                            uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                            uint32_t _ramStartLine, uint32_t _ramStartColumn,
                            uint32_t _numLines, uint32_t _numColumns,
                            bool     _accRequireResultReady);

extern
uint32_t (*xpu_readRegister)(XPU_CONTEX_HANDLE _ctx, uint32_t _address);

extern
void (*xpu_writeRegister)(XPU_CONTEX_HANDLE _ctx, uint32_t _address, uint32_t _value);

#else

XRT_EXTERN
XRT_CONTEX_HANDLE xpu_init(bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget);

XRT_EXTERN
void xpu_load(XRT_CONTEX_HANDLE _ctx, const char *_path);

XRT_EXTERN
void xpu_runRuntime(XRT_CONTEX_HANDLE _ctx, XRT_FUNCTION_HANDLE _function, uint32_t _argc, uint32_t *_argv);

XRT_EXTERN
uint32_t xpu_readRegister(XRT_CONTEX_HANDLE _ctx, uint32_t _address);

XRT_EXTERN
void xpu_writeRegister(XRT_CONTEX_HANDLE _ctx, uint32_t _address, uint32_t _value);

XRT_EXTERN
XRT_FUNCTION_HANDLE xpu_lowLevel(XRT_CONTEX_HANDLE _ctx, const char *_path);

XRT_EXTERN
void xpu_writeMatrixArray(XRT_CONTEX_HANDLE _ctx,
                          uint32_t _accMemStart,
                          uint32_t *_ramMatrix,
                          uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                          uint32_t _ramStartLine, uint32_t _ramStartColumn,
                          uint32_t _numLines, uint32_t _numColumns);

XRT_EXTERN
void xpu_readMatrixArray(XRT_CONTEX_HANDLE _ctx,
                         uint32_t _accMemStart,
                         uint32_t *_ramMatrix,
                         uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                         uint32_t _ramStartLine, uint32_t _ramStartColumn,
                         uint32_t _numLines, uint32_t _numColumns,
                         bool     _accRequireResultReady);

#endif // XRT_DYNAMIC_LOW_LEVEL

#endif
