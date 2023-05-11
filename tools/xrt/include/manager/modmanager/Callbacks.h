//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/Manager.h>

extern Manager *callbackManager;

extern "C"
void callbackLoad(const char *_path);

extern "C"
void callbackRunRuntime(void *_functionPtr);

extern "C"
void *callbackLowLevel(const char *_path);
