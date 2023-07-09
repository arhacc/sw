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
#include <vector>
#include "transformers/Transformers.h"
#include "sources/common/Source.h"
#include <mutex>
#include <span>

//-------------------------------------------------------------------------------------
class MuxSource : public Source {
    Transformers *transformers;
    std::mutex mux;
public:
    MuxSource(Transformers *_transformers);

    ~MuxSource() override = default;

    void runCommand(std::string _name);

    void runCommand(std::span<const std::string> _argv);
};
//-------------------------------------------------------------------------------------


