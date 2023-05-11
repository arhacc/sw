//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include "manager/libmanager/FunctionInfo.hpp"
#include "manager/modmanager/ModManager.h"
#include <targets/Targets.h>
#include <manager/libmanager/LibManager.h>
#include <manager/memmanager/MemManager.h>
#include <manager/driver/Driver.h>

//-------------------------------------------------------------------------------------
class Manager {
    LibManager *libManager;
    MemManager *memManager;
    ModManager *modManager;
    Driver *driver;
public:
    Manager(Targets *_targets);

    ~Manager();

    void reset();

    void run(const std::string &_name);

    void uploadFunction(const std::string &_name, int32_t _address);

    void runRuntime(uint32_t _address, uint32_t *_args);

    void runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress);

    void readRegister(uint32_t _address, uint32_t _register);

    void writeRegister(uint32_t _address, uint32_t _register);

    void writeCode(uint32_t _address, uint32_t *_code, uint32_t _length);

    void readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop);

    void writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop);

    void
    readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop);

    void
    writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop);

    void load(const std::string &_path);

    void dump(const std::string &_address);


    // usedInCallbacks

    FunctionInfo *lowLevel(const std::string& _name);

    void runRuntime(FunctionInfo *_function);
};
//-------------------------------------------------------------------------------------
