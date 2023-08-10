//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/Manager.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <targets/Targets.h>

#include <cstdint>
#include <memory>
#include <vector>

struct XrtSimpleContext {
    Manager* manager;

    inline XrtSimpleContext(Manager* manager) : manager(manager) {}
};

struct XrtOwningContext {
    const std::unique_ptr<const Arch> arch;
    const std::unique_ptr<Manager> manager;
    const std::unique_ptr<Targets> targets;

    inline XrtOwningContext(
        std::unique_ptr<const Arch> arch,
        std::unique_ptr<Manager> manager,
        std::unique_ptr<Targets> targets)
        : arch(std::move(arch)),
          manager(std::move(manager)),
          targets(std::move(targets)) {}
};

struct XrtContext {
    bool isOwning;
    union {
        XrtSimpleContext simpleContext;
        XrtOwningContext owningContext;
    };

    inline XrtContext(Manager* manager) : isOwning(false), simpleContext(manager) {}

    inline ~XrtContext() {
        if (isOwning) {
            owningContext.~XrtOwningContext();
        } else {
            simpleContext.~XrtSimpleContext();
        }
    }

    inline XrtContext(
        std::unique_ptr<const Arch> arch,
        std::unique_ptr<Manager> manager,
        std::unique_ptr<Targets> targets)
        : isOwning(true),
          owningContext(std::move(arch), std::move(manager), std::move(targets)) {}

    inline Manager* getManager() const {
        return isOwning ? owningContext.manager.get() : simpleContext.manager;
    }
};

extern "C" void xpu_load(XrtContext* _ctx, const char* _path);

extern "C" void xpu_runRuntime(
    XrtContext* _ctx,
    LowLevelFunctionInfo* _functionPtr,
    uint32_t _argc,
    uint32_t* _argv);

extern "C" uint32_t xpu_readRegister(XrtContext* _ctx, uint32_t _address);

extern "C" void xpu_writeRegister(XrtContext* _ctx, uint32_t _address, uint32_t _value);

extern "C" LowLevelFunctionInfo* xpu_lowLevel(XrtContext* _ctx, const char* _path);

extern "C" void xpu_writeRawInstruction(XrtContext* _ctx, uint32_t _instruction);

extern "C" void xpu_writeRawInstructions(
    XrtContext* _ctx, const uint32_t* _instructions, uint32_t _length);

extern "C" void xpu_writeMatrixArray(
    XrtContext* _ctx,
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns);

extern "C" void xpu_readMatrixArray(
    XrtContext* _ctx,
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady);

extern const std::vector<std::pair<const char*, void*>> xpu_allFunctions;
