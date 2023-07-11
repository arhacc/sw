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

struct XrtSimpleContext {
    Manager *manager;

    inline
    XrtSimpleContext(Manager *manager)
        : manager(manager) {
    }
};

struct XrtOwningContext {
    const std::unique_ptr<const Arch> arch;
    const std::unique_ptr<Cache> cache;
    const std::unique_ptr<Manager> manager;
    const std::unique_ptr<Targets> targets;

    inline
    XrtOwningContext(std::unique_ptr<const Arch> arch,
        std::unique_ptr<Cache> cache,
        std::unique_ptr<Manager> manager,
        std::unique_ptr<Targets> targets)
        : arch(std::move(arch)), cache(std::move(cache)), manager(std::move(manager)), targets(std::move(targets)) {
    }
};

struct XrtContext {
    bool isOwning;
    union {
        XrtSimpleContext simpleContext;
        XrtOwningContext owningContext;
    };

    inline
    XrtContext(Manager *manager)
        : isOwning(false), simpleContext(manager) {
    }

    inline
    ~XrtContext() {
        if (isOwning) {
            owningContext.~XrtOwningContext();
        } else {
            simpleContext.~XrtSimpleContext();
        }
    }

    inline
    XrtContext(std::unique_ptr<const Arch> arch,
        std::unique_ptr<Cache> cache,
        std::unique_ptr<Manager> manager,
        std::unique_ptr<Targets> targets)
        : isOwning(true), owningContext(std::move(arch), std::move(cache), std::move(manager), std::move(targets)) {
    }

    inline
    Manager* getManager() const {
        return isOwning ? owningContext.manager.get() : simpleContext.manager;
    }
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
