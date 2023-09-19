//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/driver/Driver.h>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/memmanager/FreeSpace.hpp>
#include <manager/memmanager/SymbolInfo.hpp>

#include <cstdint>
#include <functional>
#include <map>
#include <queue>
#include <unordered_map>

// TODO: pload/prun addresses are incremented by 1 for every pair of instructions, not for
// every instruction

//-------------------------------------------------------------------------------------
class MemManager {
    Driver* driver;
    [[maybe_unused]] const Arch& arch;

    std::unordered_map<std::string, SymbolInfo*> ctrlMemoryLoadedSymbols;
    std::vector<FreeSpace*> ctrlMemorySpace;

    void freeSpace();
    void freeAdjacentSpace(SymbolInfo* symbol);
    void
    addFunctionAsSymbol(LowLevelFunctionInfo& _function, uint32_t _address, bool sticky);
    void addFunctionInBestSpace(LowLevelFunctionInfo& _function);

    static uint64_t timeNow();

  public:
    MemManager(Driver* _driver, const Arch& _arch);

    ~MemManager() = default;

    void loadFunction(LowLevelFunctionInfo& _function, bool sticky = false);

    SymbolInfo* resolve(std::string _name);

    void dump();
};
//-------------------------------------------------------------------------------------
