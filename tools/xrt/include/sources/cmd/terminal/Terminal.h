//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <string>

//-------------------------------------------------------------------------------------
class Terminal {
    static std::vector<std::string> TERMINAL_COMMANDS;
public:
    Terminal() = delete;

    static bool isValidCommand(char **_argv);

    static std::string runCommand(char **_argv);
};
//-------------------------------------------------------------------------------------


