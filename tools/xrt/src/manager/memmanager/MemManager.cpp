//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/driver/Driver.h>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/memmanager/FreeSpace.hpp>
#include <manager/memmanager/MemManager.h>
#include <manager/memmanager/SymbolInfo.hpp>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <fmt/format.h>

namespace chrono = std::chrono;

// TODO: Fix SimTarget macros
#undef CONTROLLER_INSTR_MEM_SIZE

//-------------------------------------------------------------------------------------
MemManager::MemManager(Driver* _driver, const Arch& _arch)
    : driver(_driver), arch(_arch) {
    assert(_driver != nullptr);

    FreeSpace* _totalSpace = new FreeSpace;

    _totalSpace->address = 0;
    _totalSpace->length  = _arch.CONTROLLER_INSTR_MEM_SIZE;

    ctrlMemorySpace.push_back(_totalSpace);
}

//-------------------------------------------------------------------------------------
uint64_t MemManager::timeNow() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

//-------------------------------------------------------------------------------------
void MemManager::addFunctionInBestSpace(LowLevelFunctionInfo& _function) {
    FreeSpace& _space = **ctrlMemorySpace.begin();

    assert(_space.length >= _function.length);

    // To consider alignment
    _space.address += _function.length;
    _space.length -= _function.length;

    if (_space.length == 0) {
        std::pop_heap(ctrlMemorySpace.begin(), ctrlMemorySpace.end());
        ctrlMemorySpace.pop_back();
    }
}

//-------------------------------------------------------------------------------------
void MemManager::addFunctionAsSymbol(
    LowLevelFunctionInfo& _function, uint32_t _address, bool sticky) {
    SymbolInfo* symbol = new SymbolInfo;

    symbol->address        = _address;
    symbol->length         = _function.length;
    symbol->name           = _function.name;
    symbol->timeLastUsedMs = timeNow();
    symbol->sticky         = sticky;

    ctrlMemoryLoadedSymbols.insert({_function.name, symbol});
}

//-------------------------------------------------------------------------------------
void MemManager::loadFunction(LowLevelFunctionInfo& _function, bool sticky) {
    FreeSpace& _space = **ctrlMemorySpace.begin();

    while (_space.length < _function.length) {
        freeSpace();

        _space = **ctrlMemorySpace.begin();
    }

    driver->writeCode(_space.address, _function.code, _function.length);

    addFunctionAsSymbol(_function, _space.address, sticky);

    // modifies _space.address, so should happen last
    addFunctionInBestSpace(_function);
}

//-------------------------------------------------------------------------------------
void MemManager::freeSpace() {
    // TODO: this is inefficient, we should index by timeLastUsedMs
    auto _oldestSymbolIt = std::min_element(
        ctrlMemoryLoadedSymbols.begin(),
        ctrlMemoryLoadedSymbols.end(),
        [](const auto& l, const auto& r) {
            if (l.second->sticky)
                return false;

            if (r.second->sticky)
                return true;

            return l.second->timeLastUsedMs < r.second->timeLastUsedMs;
        });

    if (_oldestSymbolIt == ctrlMemoryLoadedSymbols.end()
        || _oldestSymbolIt->second->sticky)
        throw std::runtime_error("Out Of Memory");

    SymbolInfo* _oldestSymbol = _oldestSymbolIt->second;

    freeAdjacentSpace(_oldestSymbol);

    ctrlMemoryLoadedSymbols.erase(_oldestSymbolIt->first);

    delete _oldestSymbol;
}

//-------------------------------------------------------------------------------------
void MemManager::freeAdjacentSpace(SymbolInfo* _symbol) {
    // Free space before the symbol
    auto _freeSpaceBeforeIt = std::find_if(
        ctrlMemorySpace.begin(), ctrlMemorySpace.end(), [=](FreeSpace* _space) {
            return _space->address + _space->length == _symbol->address;
        });

    FreeSpace* _freeSpaceBefore = nullptr;
    if (_freeSpaceBeforeIt != ctrlMemorySpace.end()) {
        _freeSpaceBefore = *_freeSpaceBeforeIt;

        ctrlMemorySpace.erase(_freeSpaceBeforeIt);
    }

    // Free space after the symbol
    auto _freeSpaceAfterIt = std::find_if(
        ctrlMemorySpace.begin(), ctrlMemorySpace.end(), [=](FreeSpace* _space) {
            return _symbol->address + _symbol->length == _space->address;
        });

    FreeSpace* _freeSpaceAfter = nullptr;
    if (_freeSpaceAfterIt != ctrlMemorySpace.end()) {
        _freeSpaceAfter = *_freeSpaceAfterIt;

        ctrlMemorySpace.erase(_freeSpaceAfterIt);
    }

    // If there is free space both before and after the symbol
    if (_freeSpaceBefore != nullptr && _freeSpaceAfter != nullptr) {
        _freeSpaceBefore->length += _symbol->length + _freeSpaceAfter->length;

        delete _freeSpaceAfter;

        ctrlMemorySpace.push_back(_freeSpaceBefore);

        // If there is free space only before the symbol
    } else if (_freeSpaceBefore != nullptr) {
        _freeSpaceBefore->length += _symbol->length;

        ctrlMemorySpace.push_back(_freeSpaceBefore);

        // If there is free space only after the symbol
    } else if (_freeSpaceAfter != nullptr) {
        _freeSpaceAfter->address = _symbol->address;
        _freeSpaceAfter->length  = _symbol->length + _freeSpaceAfter->length;

        ctrlMemorySpace.push_back(_freeSpaceAfter);

        // If there is no free space on either side of the symbol
    } else {
        FreeSpace* _freeSpaceNew = new FreeSpace;

        _freeSpaceNew->address = _symbol->address;
        _freeSpaceNew->length  = _symbol->length;

        ctrlMemorySpace.push_back(_freeSpaceNew);
    }

    std::make_heap(ctrlMemorySpace.begin(), ctrlMemorySpace.end());
}

//-------------------------------------------------------------------------------------
SymbolInfo* MemManager::resolve(std::string _name) {
    try {
        SymbolInfo* _symbol = ctrlMemoryLoadedSymbols.at(_name);

        _symbol->timeLastUsedMs = timeNow();

        return _symbol;
    } catch (std::out_of_range&) {
        return nullptr;
    }
}

//-------------------------------------------------------------------------------------
void MemManager::dump() {
    fmt::println("memory map dump");

    fmt::println("SYMBOLS");
    for (auto [_, _symbol] : ctrlMemoryLoadedSymbols) {
        fmt::println(
            "symbol at 0x{:08X} len 0x{:08X} -- {}",
            _symbol->address,
            _symbol->length,
            _symbol->name);
    }

    std::cout << "FREE SPACES\n";
    for (FreeSpace* _freeSpace : ctrlMemorySpace) {
        fmt::println(
            "free space {:08X} len {:08X}", _freeSpace->address, _freeSpace->length);
    }
}

//-------------------------------------------------------------------------------------
