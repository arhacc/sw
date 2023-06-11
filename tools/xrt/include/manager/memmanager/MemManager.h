//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>
#include <manager/driver/Driver.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/memmanager/FreeSpace.hpp>
#include <manager/memmanager/SymbolInfo.hpp>

//-------------------------------------------------------------------------------------
class MemManager {
    Driver *driver;
    const Arch& arch;

    std::unordered_map<std::string, SymbolInfo*> ctrlMemoryLoadedSymbols;
    std::vector<FreeSpace*> ctrlMemorySpace;

    void freeSpace();
    void freeAdjacentSpace(SymbolInfo *symbol);
    void addFunctionAsSymbol(FunctionInfo &_function, uint32_t _address, bool sticky);
    void addFunctionInBestSpace(FunctionInfo &_function);

    static uint64_t timeNow();

public:
    MemManager(Driver *_driver, const Arch& _arch);

    ~MemManager() = default;

    void loadFunction(FunctionInfo &_function, bool sticky = false);

    SymbolInfo *resolve(std::string _name);

    void dump();
};
//-------------------------------------------------------------------------------------
