//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/log/Logger.hpp>
#include <transformers/midlevel/library/MidLevelLibrary.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>
#include <memory>
#include <stdexcept>

#include "common/Utils.hpp"
#include <dynload.h>
#include <fmt/format.h>

//-------------------------------------------------------------------------------------
MidLevelLibrary::MidLevelLibrary(const char* _filename) : filename(_filename) {
    lib = dlLoadLibrary(_filename);
    if (lib == nullptr) {
        throw std::runtime_error(fmt::format("Could not open shared library file: {}", _filename));
    }

    DLSyms *_symbols = dlSymsInit(_filename);
    if (_symbols == nullptr) {
        throw std::runtime_error(fmt::format("Could not load symbols from shared libary file: {}", _filename));
    }

    logInit.print(fmt::format("Loading mid level library: {}\n", _filename));

    int _numOfSymbols = dlSymsCount(_symbols);
    for (int _i = 0; _i < _numOfSymbols; _i++) {
        const char *_symbol = dlSymsName(_symbols, _i);
        if (beginsWith(_symbol, cMidLevelDescriptionSymbolPrefix)) {
            auto _function = std::make_unique<MidLevelFunction>(lib, _symbol);
            functions.push_back(std::move(_function));
        }
    }

    dlSymsCleanup(_symbols);
}

//-------------------------------------------------------------------------------------
MidLevelLibrary::~MidLevelLibrary() {
    dlFreeLibrary(lib);
}

//-------------------------------------------------------------------------------------