//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <stddef.h>
#include <xrt.h>

#ifdef XRT_DYNAMIC_MID_LEVEL

void (*xpu_load)(XRT_CONTEX_HANDLE _ctx, const char *_path) = NULL;

XRT_FUNCTION_HANDLE (*xpu_lowLevel)(XRT_CONTEX_HANDLE _ctx, const char *_path) = NULL;

void (*xpu_runRuntime)(XRT_CONTEX_HANDLE _ctx, XRT_FUNCTION_HANDLE _function, uint32_t _argc, uint32_t *_argv) = NULL;

void (*xpu_writeMatrixArray)(XRT_CONTEX_HANDLE _ctx,
                              uint32_t _accMemStart,
                              uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLines, uint32_t _numColumns) = NULL;

void (*xpu_readMatrixArray)(XRT_CONTEX_HANDLE _ctx,
                            uint32_t _accMemStart,
                            uint32_t *_ramMatrix,
                            uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                            uint32_t _ramStartLine, uint32_t _ramStartColumn,
                            uint32_t _numLines, uint32_t _numColumns,
                            bool     _accRequireResultReady) = NULL;


uint32_t (*xpu_readRegister)(XRT_CONTEX_HANDLE _ctx, uint32_t _address) = NULL;

void (*xpu_writeRegister)(XRT_CONTEX_HANDLE _ctx, uint32_t _address, uint32_t _value) = NULL;

#endif
