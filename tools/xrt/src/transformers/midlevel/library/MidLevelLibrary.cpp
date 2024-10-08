//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>
#include <transformers/midlevel/library/MidLevelLibrary.hpp>

#include <memory>
#include <stdexcept>

#include <dynload.h>
#include <fmt/format.h>

//-------------------------------------------------------------------------------------
MidLevelLibrary::MidLevelLibrary(const char* _filename, Manager* _manager) : filename(_filename) {
    lib = dlLoadLibrary(_filename);
    if (lib == nullptr) {
        throw std::runtime_error(fmt::format("Could not open shared library file: {}", _filename));
    }

    DLSyms* _symbols = dlSymsInit(_filename);
    if (_symbols == nullptr) {
        throw std::runtime_error(fmt::format("Could not load symbols from shared libary file: {}", _filename));
    }

    logInit.println<InfoMedium>("Loading mid level library: {}", _filename);

    int _numOfSymbols = dlSymsCount(_symbols);
    for (int _i = 0; _i < _numOfSymbols; _i++) {
        const char* _symbol = dlSymsName(_symbols, _i);
        if (beginsWith(_symbol, cMidLevelDescriptionSymbolPrefix)) {
            auto _function = std::make_unique<MidLevelFunction>(lib, _symbol, _manager);
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
std::unique_ptr<MidLevelFunction>& MidLevelLibrary::getFunction(std::string_view _opName) {
  for (auto& _function : functions) {
    if (_function->match(_opName)) {
      return _function;
    }
  }

  throw std::runtime_error(fmt::format("MidLevelLibrary: Not found {}", _opName));
}

//-------------------------------------------------------------------------------------

