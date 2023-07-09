//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdio>
#include <reproc++/reproc.hpp>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include <reproc++/run.hpp>
#include "sources/cmd/terminal/Terminal.h"
#include <fmt/format.h>
#include <filesystem>

std::vector<std::string> Terminal::TERMINAL_COMMANDS{"ll", "ls", "pwd", "cd", "w"};

//-------------------------------------------------------------------------------------
bool Terminal::isValidCommand(std::span<const std::string> _argv) {
    const std::string& _firstArg = _argv[0];
    return std::any_of(TERMINAL_COMMANDS.begin(), TERMINAL_COMMANDS.end(),
            [&_firstArg](const auto &s) { return s == _firstArg; });
}

//-------------------------------------------------------------------------------------
std::string Terminal::runCommand(std::span<const std::string> _argv) {
    if (_argv[0] == "cd") {
        if (_argv.size() == 1) {
            throw std::runtime_error("cd: missing operand");
        } else if (_argv.size() > 2) {
            throw std::runtime_error("cd: too many arguments");
        } else {
            std::filesystem::current_path(_argv[1]);
            return "";
        }
    }

    reproc::process _process;

    std::error_code _ec = _process.start(_argv);
    if (_ec) {
        throw std::system_error(_ec);
    }

    std::string _output;
    reproc::sink::string sink(_output);

    _ec = reproc::drain(_process, sink, sink);
    if (_ec) {
        throw std::system_error(_ec);
    }

    int _status;
    std::tie(_status, _ec) = _process.wait(reproc::infinite);
    if (_ec) {
        throw std::system_error(_ec);
    } else if (_status != 0) {
        throw std::runtime_error(fmt::format("subrocess {} failed with error code {}", _argv[0], _status));
    }

    return _output;
}
//-------------------------------------------------------------------------------------
