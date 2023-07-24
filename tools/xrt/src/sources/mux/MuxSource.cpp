//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <mutex>
#include <sources/mux/MuxSource.h>
#include <cstring>
#include <common/Utils.h>
#include <sources/cmd/rxterm/terminal.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <stdexcept>
#include <utility>

#include <cinttypes>

//-------------------------------------------------------------------------------------
MuxSource::MuxSource(Transformers *_transformers) {
    transformers = _transformers;
}

//-------------------------------------------------------------------------------------
MuxCommandReturnValue MuxSource::runCommand(std::string _command) {
    strTokenizer(std::move(_command));
    return runCommand(argv);
}

//-------------------------------------------------------------------------------------
MuxCommandReturnValue MuxSource::runCommand(std::span<const std::string> _argv) {
    std::unique_lock lock(mux);

    assert(_argv.size() > 0);

    if (_argv[0] == "") {

        return {};
    } else if (_argv[0] == "run") {
        assert(_argv.size() > 1);
        std::string _path(_argv[1]);
        transformers->run(_path);

        return {};
    } else if (_argv[0] == "source") {
        std::string _path(_argv[1]);
        transformers->load(_path);

        return {};
    } else if (_argv[0] == "debug-get-array-data") {
       uint32_t _firstCell = std::stoi(_argv[1]);
       uint32_t _lastCell = std::stoi(_argv[2]);
       uint32_t _firstRow = std::stoi(_argv[3]);
       uint32_t _lastRow = std::stoi(_argv[4]);

       return transformers->debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);
    } else if (_argv[0] == "exit" || _argv[0] == "quit" || _argv[0] == "q") {
        fmt::println("Exiting...");
        signalHandler(0);

        return {};
    } else {
        fmt::println("Unrecognized command: {}", _argv[0]);

        throw std::runtime_error("Unrecognized command");
    }
}


//-------------------------------------------------------------------------------------
