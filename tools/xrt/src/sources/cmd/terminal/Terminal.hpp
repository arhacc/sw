//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <span>
#include <string>
#include <vector>

#include <reproc++/run.hpp>

//-------------------------------------------------------------------------------------
class Terminal {
    static std::vector<std::string> TERMINAL_COMMANDS;

  public:
    Terminal() = delete;

    static bool isValidCommand(std::span<const std::string> _argv);

    static std::string runCommand(std::span<const std::string> _argv);
};
//-------------------------------------------------------------------------------------
