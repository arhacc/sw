//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Constants.hpp>
#include <common/Utils.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <sources/cmd/rxterm/terminal.hpp>
#include <sources/common/Source.hpp>
#include <sources/mux/MuxSource.hpp>
#include <transformers/Transformers.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <string_view>

#include "transformers/common/resourcefetcher/ResourceFetcher.hpp"
#include <fmt/printf.h>
#include <pthread.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
MuxSource::MuxSource(Transformers* _transformers) {
    transformers = _transformers;
}


//-------------------------------------------------------------------------------------
int MuxSource::run(
    const ResourceIdentifier& _path,
    const std::unordered_map<std::string, ResourceIdentifier>& _inputs,
    std::unordered_map<std::string, ResourceIdentifier>& _outputs
) {
    std::unique_lock lock(mux);

    return transformers->run(_path, _inputs, _outputs);
}


//-------------------------------------------------------------------------------------
std::vector<uint32_t>
MuxSource::debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    std::unique_lock lock(mux);

    return transformers->debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> MuxSource::debugGetArrayRegs(uint32_t _firstCell, uint32_t _lastCell) {
    std::unique_lock lock(mux);

    return transformers->debugGetArrayRegs(_firstCell, _lastCell);
}

//-------------------------------------------------------------------------------------
void MuxSource::debugPutArrayData(
    uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow, std::span<const uint32_t> _data) {
    std::unique_lock lock(mux);

    return transformers->debugPutArrayData(_firstCell, _lastCell, _firstRow, _lastRow, _data);
}

//-------------------------------------------------------------------------------------
unsigned MuxSource::debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber) {
    std::unique_lock lock(mux);

    return transformers->debugSetBreakpoint(_functionName, _lineNumber);
}

//-------------------------------------------------------------------------------------
unsigned MuxSource::debugContinue() {
    std::unique_lock lock(mux);

    return transformers->debugContinue();
}



// //-------------------------------------------------------------------------------------
// unsigned MuxSource::getActiveBreakpointID() {
//     std::unique_lock lock(mux);

//     return transformers->getActiveBreakpointID();
// }

//-------------------------------------------------------------------------------------
void MuxSource::registerFetcher(std::unique_ptr<ResourceFetcher> _resourceFetcher) {
    std::unique_lock lock(mux);

    transformers->registerFetcher(std::move(_resourceFetcher));
}

// //-------------------------------------------------------------------------------------
// MuxCommandReturnValue MuxSource::runCommand(std::string _command) {
//     strTokenizer(std::move(_command));
//     return runCommand(argv);
// }

// void MuxSource

// //-------------------------------------------------------------------------------------
// MuxCommandReturnValue MuxSource::runCommand(std::span<const std::string> _argv) {
//     std::unique_lock lock(mux);

//     fmt::print("Running command: ");
//     for (const auto& _arg : _argv) {
//         fmt::print("{} ", _arg);
//     }
//     fmt::println("");

//     assert(_argv.size() > 0);

//     if (_argv[0] == "") {
//         return {};
//     } else if (_argv[0] == "run") {
//         assert(_argv.size() > 1);
//         std::string _path(_argv[1]);
//         uint32_t ret = transformers->run(_path);

//         return std::vector<uint32_t>{ret};
//     } else if (_argv[0] == "source") {
//         std::string _path(_argv[1]);
//         transformers->load(_path);

//         return {};
//     } else if (_argv[0] == "debug-get-array-data") {
//         uint32_t _firstCell = std::stoi(_argv[1]);
//         uint32_t _lastCell  = std::stoi(_argv[2]);
//         uint32_t _firstRow  = std::stoi(_argv[3]);
//         uint32_t _lastRow   = std::stoi(_argv[4]);

//         return transformers->debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);

//     } else if (_argv[0] == "debug-put-array-data") {
//         uint32_t _firstCell = std::stoi(_argv[1]);
//         uint32_t _lastCell  = std::stoi(_argv[2]);
//         uint32_t _firstRow  = std::stoi(_argv[3]);
//         uint32_t _lastRow   = std::stoi(_argv[4]);

//         uint32_t _numWords = (_lastCell - _firstCell + 1) * (_lastRow - _firstRow + 1);

//         assert(_argv.size() == _numWords + 5);

//         std::vector<uint32_t> _words;

//         for (uint32_t i = 0; i < _numWords; i++) {
//             _words.push_back(std::stoi(_argv[i + 5]));
//         }

//         transformers->debugPutArrayData(_firstCell, _lastCell, _firstRow, _lastRow, _words);

//         return {};
//     } else if (_argv[0] == "debug-get-array-registers") {
//         uint32_t _firstCell = std::stoi(_argv[1]);
//         uint32_t _lastCell  = std::stoi(_argv[2]);

//         return transformers->debugGetArrayRegs(_firstCell, _lastCell);

//     } else if (_argv[0] == "debug-set-breakpoint") {
//         std::string_view _functionName = _argv[1];
//         uint32_t _lineNumber           = std::stoi(_argv[2]);

//         unsigned _breakpointID = transformers->debugSetBreakpoint(_functionName, _lineNumber);

//         return std::vector<uint32_t>{_breakpointID};
//     } else if (_argv[0] == "debug-continue") {
//         transformers->debugContinue();

//         return {};
//     } else if (_argv[0] == "debug-get-active-breakpoint") {
//         return std::vector<uint32_t>{transformers->getActiveBreakpointID()};
//     } else if (_argv[0] == "exit" || _argv[0] == "quit" || _argv[0] == "q") {
//         fmt::println("Exiting...");
//         std::exit(0);

//         return {};
//     } else {
//         fmt::println("Unrecognized command: {}", _argv[0]);

//         throw std::runtime_error("Unrecognized command");
//     }
// }

//-------------------------------------------------------------------------------------
