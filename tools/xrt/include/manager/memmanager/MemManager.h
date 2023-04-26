//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <functional>
#include <manager/driver/Driver.h>
#include <manager/libmanager/FunctionInfo.hpp>

//-------------------------------------------------------------------------------------
class MemManager {
    Driver *driver;

public:
    MemManager(Driver *_driver);

    ~MemManager() = default;

    void runFile(const std::string &_path);

    void dump(const std::string &_address);
};
//-------------------------------------------------------------------------------------
