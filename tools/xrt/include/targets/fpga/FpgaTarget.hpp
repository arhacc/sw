//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
// FpgaTarget -- XPU AXI Driver
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <termios.h>
#include <unistd.h>

#undef MAP_TYPE /* Ugly Hack */

#include <targets/common/Target.hpp>

#include <climits>

// forward declarations
struct Arch;

//-------------------------------------------------------------------------------------
class FpgaTarget : public Target {
    static constexpr size_t cRegisterSpaceSize   = 5012; // TODO: find out how much is actually mapped

    Arch& arch_;

    volatile uint32_t *registerSpace_;
    int                registerSpaceFd_;

    void initRegisterSpace();

  public:
    uint32_t readRegister(size_t addr) const;
    void writeRegister(size_t addr, uint32_t value) const;

    FpgaTarget(Arch& _arch);
    ~FpgaTarget() override;

    void reset() override;

    void process(std::shared_ptr<Future> _future) override;
};

//-------------------------------------------------------------------------------------
