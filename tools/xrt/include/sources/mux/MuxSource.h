//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <common/Globals.h>
#include "transformers/Transformers.h"
#include "sources/common/Source.h"

//-------------------------------------------------------------------------------------
class MuxSource : public Source {
    Transformers *transformers;
public:
    MuxSource(Transformers *_transformers);

    ~MuxSource() override = default;

    void runCommand(std::string _name);

    void runCommand(char **_argv);
};
//-------------------------------------------------------------------------------------


