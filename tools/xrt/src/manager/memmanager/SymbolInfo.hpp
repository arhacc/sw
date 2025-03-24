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
#include <vector>

struct SymbolInfo {
    uint32_t address;        /// starting point of symbol
    uint32_t length;         /// length of symbol
    std::string name;        /// symbol name
    uint64_t timeLastUsedMs; /// last time the symbol was used
    bool sticky;             /// disallows unloading
};
