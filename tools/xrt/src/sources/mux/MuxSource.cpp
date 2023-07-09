//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
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
void MuxSource::runCommand(std::string _command) {
    strTokenizer(std::move(_command));
    runCommand(argv);
}

//-------------------------------------------------------------------------------------
void MuxSource::runCommand(std::span<const std::string> _argv) {
    std::unique_lock lock(mux);

    assert(_argv.size() > 0);

    if (_argv[0] == "") {

    } else if (_argv[0] == "run") {
        assert(_argv.size() > 1);
        std::string _path(_argv[1]);
        transformers->run(_path);
    } else if (_argv[0] == "source") {
        std::string _path(_argv[1]);
        transformers->load(_path);
    } else if (_argv[0] == "exit" || _argv[0] == "quit" || _argv[0] == "q") {
        std::cout << "Exiting..." << std::endl;
        signalHandler(0);
    } else {
        std::cout << "Unrecognized command: " << _argv[0] << std::endl;
    }
}


//-------------------------------------------------------------------------------------
