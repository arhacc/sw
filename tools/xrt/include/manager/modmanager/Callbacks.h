//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <manager/Manager.h>

extern Manager *callbackManager;

extern "C"
void callbackLoad(const char *_path);

extern "C"
void callbackRunRuntime(void *_functionPtr);

extern "C"
void *callbackLowLevel(const char *_path);

extern "C"
void *callbackWriteMatrixArray(uint32_t *_ramMatrix,
                               uint32_t _ramLineSize, uint32_t _ramColumnSize,
                               uint32_t _ramStartLine, uint32_t _ramStartColumn,
                               uint32_t _ramNumLine, uint32_t _ramNumColumn,
                               uint32_t _accMemStart,
                               uint32_t _accNumLine, uint32_t _accNumColumn);

extern "C"
void *callbackReadMatrixArray(uint32_t _accMemStart,
                              uint32_t _accNumLine, uint32_t _accNumColumn,
                              bool     _accRequireResultReady,
                              uint32_t *_ramMatrix,
                              uint32_t _ramLineSize, uint32_t _ramColumnSize,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _ramNumLine, uint32_t _ramNumColumn);
