//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

// forward declarations
class Driver;
struct Arch;
struct FreeSpace;
struct LowLevelFunctionInfo;
struct SymbolInfo;

//-------------------------------------------------------------------------------------
class MemManager {
    [[maybe_unused]] const Arch& arch;
    [[maybe_unused]] std::function<uint64_t()> getTime; // for log

    std::unordered_map<std::string, SymbolInfo*> ctrlMemoryLoadedSymbols;
    std::vector<FreeSpace*> ctrlMemorySpace;

    void freeSpace();
    void freeAdjacentSpace(SymbolInfo* symbol);
    void addFunctionAsSymbol(LowLevelFunctionInfo& _function, uint32_t _address, bool sticky);
    void addFunctionInBestSpace(LowLevelFunctionInfo& _function);

    static uint64_t timeNow();

  public:
    MemManager(const Arch& _arch, std::function<uint64_t()> _getTime);

    ~MemManager() = default;

    void loadFunction(LowLevelFunctionInfo& _function, bool sticky = false);

    SymbolInfo* resolve(std::string _name);
    inline SymbolInfo* resolve(std::string_view _name) {
        return resolve(std::string(_name));
    };

    void printMemMap() const;
};
//-------------------------------------------------------------------------------------
