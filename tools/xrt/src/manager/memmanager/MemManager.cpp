//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/driver/Driver.hpp>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/memmanager/FreeSpace.hpp>
#include <manager/memmanager/MemManager.hpp>
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
MemManager::MemManager(const Arch& _arch) : arch(_arch) {
    FreeSpace* _totalSpace = new FreeSpace;

    _totalSpace->address = 0;
    _totalSpace->length  = _arch.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE);

    ctrlMemorySpace.push_back(_totalSpace);
}

//-------------------------------------------------------------------------------------
uint64_t MemManager::timeNow() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

//-------------------------------------------------------------------------------------
void MemManager::addFunctionInBestSpace(LowLevelFunctionInfo& _function) {
    FreeSpace& _space = **ctrlMemorySpace.begin();

    assert(_space.length >= _function.memLength());

    _space.address += _function.memLength();
    _space.length -= _function.memLength();

    if (_space.length == 0) {
        std::pop_heap(ctrlMemorySpace.begin(), ctrlMemorySpace.end());
        ctrlMemorySpace.pop_back();
    }
}

//-------------------------------------------------------------------------------------
void MemManager::addFunctionAsSymbol(LowLevelFunctionInfo& _function, uint32_t _address, bool sticky) {
    SymbolInfo* symbol = new SymbolInfo;

    symbol->address        = _address;
    symbol->length         = _function.memLength();
    symbol->name           = _function.name;
    symbol->timeLastUsedMs = timeNow();
    symbol->sticky         = sticky;

    ctrlMemoryLoadedSymbols.insert({_function.name, symbol});
}

//-------------------------------------------------------------------------------------
void MemManager::loadFunction(LowLevelFunctionInfo& _function, bool sticky) {
    FreeSpace& _space = **ctrlMemorySpace.begin();

    while (_space.length < _function.memLength()) {
        freeSpace();

        _space = **ctrlMemorySpace.begin();
    }

    _function.address = _space.address;

    addFunctionAsSymbol(_function, _space.address, sticky);

    // modifies _space.address, so should happen last
    addFunctionInBestSpace(_function);
}

//-------------------------------------------------------------------------------------
void MemManager::freeSpace() {
    // TODO: this is inefficient, we should index by timeLastUsedMs
    auto _oldestSymbolIt = std::min_element(
        ctrlMemoryLoadedSymbols.begin(), ctrlMemoryLoadedSymbols.end(), [](const auto& l, const auto& r) {
            if (l.second->sticky)
                return false;

            if (r.second->sticky)
                return true;

            return l.second->timeLastUsedMs < r.second->timeLastUsedMs;
        });

    if (_oldestSymbolIt == ctrlMemoryLoadedSymbols.end() || _oldestSymbolIt->second->sticky)
        throw std::runtime_error("Out Of Memory");

    SymbolInfo* _oldestSymbol = _oldestSymbolIt->second;

    freeAdjacentSpace(_oldestSymbol);

    ctrlMemoryLoadedSymbols.erase(_oldestSymbolIt->first);

    delete _oldestSymbol;
}

//-------------------------------------------------------------------------------------
void MemManager::freeAdjacentSpace(SymbolInfo* _symbol) {
    // Free space before the symbol
    auto _freeSpaceBeforeIt = std::find_if(ctrlMemorySpace.begin(), ctrlMemorySpace.end(), [=](FreeSpace* _space) {
        return _space->address + _space->length == _symbol->address;
    });

    FreeSpace* _freeSpaceBefore = nullptr;
    if (_freeSpaceBeforeIt != ctrlMemorySpace.end()) {
        _freeSpaceBefore = *_freeSpaceBeforeIt;

        ctrlMemorySpace.erase(_freeSpaceBeforeIt);
    }

    // Free space after the symbol
    auto _freeSpaceAfterIt = std::find_if(ctrlMemorySpace.begin(), ctrlMemorySpace.end(), [=](FreeSpace* _space) {
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
