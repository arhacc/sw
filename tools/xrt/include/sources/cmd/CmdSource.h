//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <common/Globals.h>
#include "sources/mux/MuxSource.h"
#include "sources/cmd/terminal/Terminal.h"

//-------------------------------------------------------------------------------------
class CmdSource : public Source {
    MuxSource *muxSource;

    static void printResult(MuxCommandReturnValue&& _result);

public:
    static void initShell();

    static void progress();

    static std::string get_input(const std::string &_prompt);

    CmdSource(MuxSource *_muxSource);

    ~CmdSource() override = default;

    [[noreturn]] void run();

    void runCommand(std::string _name);
};
//-------------------------------------------------------------------------------------


