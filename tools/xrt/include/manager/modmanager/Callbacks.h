//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include "manager/libmanager/FunctionInfo.hpp"
#include "targets/Targets.h"
#include <cstdint>
#include <manager/Manager.h>
#include <memory>

struct XrtContext {
    const std::unique_ptr<const Arch> arch;
    const std::unique_ptr<Cache> cache;
    const std::unique_ptr<Manager> manager;
    const std::unique_ptr<Targets> targets;
};

extern "C"
void xpu_load(XrtContext *_ctx, const char *_path);

extern "C"
void xpu_runRuntime(XrtContext *_ctx, FunctionInfo *_functionPtr, uint32_t _argc, uint32_t *_argv);

extern "C"
uint32_t xpu_readRegister(XrtContext *_ctx, uint32_t _address);

extern "C"
void xpu_writeRegister(XrtContext *_ctx, uint32_t _address, uint32_t _value);

extern "C"
FunctionInfo *xpu_lowLevel(XrtContext *_ctx, const char *_path);

extern "C"
void xpu_writeMatrixArray(XrtContext *_ctx,
                          uint32_t _accMemStart,
                          uint32_t *_ramMatrix,
                          uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                          uint32_t _ramStartLine, uint32_t _ramStartColumn,
                          uint32_t _numLines, uint32_t _numColumns);

extern "C"
void xpu_readMatrixArray(XrtContext *_ctx,
                         uint32_t _accMemStart,
                         uint32_t *_ramMatrix,
                         uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                         uint32_t _ramStartLine, uint32_t _ramStartColumn,
                         uint32_t _numLines, uint32_t _numColumns,
                         bool     _accRequireResultReady);
