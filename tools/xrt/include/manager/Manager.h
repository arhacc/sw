//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include "manager/libmanager/FunctionInfo.hpp"
#include "manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp"
#include <cstdint>
#include <targets/Targets.h>
#include <manager/libmanager/LibManager.h>
#include <manager/memmanager/MemManager.h>
#include <manager/driver/Driver.h>
#include <manager/libmanager/LibraryResolver.h>

//-------------------------------------------------------------------------------------
class Manager {
    LibManager *libManager;
    MemManager *memManager;
    Driver *driver;

    LibraryResolver *libraryResolver;

    const Arch& arch;
public:
    Manager(Targets *_targets, const Arch& _arch);

    ~Manager();

    void run(const std::string &_name);

    void run(FunctionInfo _function);

    void load(const std::string &_path, LibLevel _level = LibLevel::ANY_LEVEL);

    // driver encapsulation

    void runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args);

    uint32_t readRegister(uint32_t _address);

    void writeRegister(uint32_t _address, uint32_t _value);

    // used in callbacks

    LowLevelFunctionInfo *lowLevel(const std::string& _name);

    void runRuntime(LowLevelFunctionInfo *_function, uint32_t _argc, uint32_t *_argv);


    void readMatrixArray(uint32_t _accMemStart,
                         uint32_t *_ramMatrix,
                         uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                         uint32_t _ramStartLine, uint32_t _ramStartColumn,
                         uint32_t _numLines, uint32_t _numColumns,
                         bool     _accRequireResultReady);

    void writeMatrixArray(uint32_t _accMemStart,
                          uint32_t *_ramMatrix,
                          uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                          uint32_t _ramStartLine, uint32_t _ramStartColumn,
                          uint32_t _numLines, uint32_t _numColumns);
};
//-------------------------------------------------------------------------------------
