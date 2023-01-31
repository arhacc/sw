//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#include <fmt/format.h>
#include "xsi_loader.h"
#include "defines.h"
#include "psimulation.h"
#include "parameters.h"
#include <targets/common/Target.h>
#include "targets/sim/XpuTestBench.h"
#include "targets/sim/xsi_loader.h"
//-------------------------------------------------------------------------------------
class SimTarget : public Target {

public:
    SimTarget();
    ~SimTarget();
    void init();
    void quit();
    void loadCode(uint32_t _address, uint32_t* _code, uint32_t _length);
    void loadData(uint32_t _address, uint32_t* _data, uint32_t _length);

private:
    XpuTestBench* xpuTestBench;
};

//-------------------------------------------------------------------------------------
