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

#include "targets/fpga/Dma.hpp"
#include "targets/fpga/UioDevice.hpp"
#include <fcntl.h>
#include <gtest/gtest_prod.h>
#include <sys/mman.h>
#include <termios.h>
#include <unistd.h>
#include <gtest/gtest.h>

#include <targets/common/Target.hpp>

#include <climits>

// forward declarations
struct Arch;

//-------------------------------------------------------------------------------------
class FpgaTarget : public Target {
    FRIEND_TEST(FpgaTargetTest, SaneArchitectureHashRawAccess);
    FRIEND_TEST(FpgaTargetTest, WriteIDRawAccess);

    static constexpr size_t cRegisterSpaceSize   = 5012; // TODO: find out how much is actually mapped
    static constexpr char   cUioDevicePath[]     = "/dev/uio1";

    UioDevice uioDevice_;
    Dma dma_;

    Arch& arch_;

    volatile uint32_t *registerSpace_;
    int                registerSpaceFd_;

    void initRegisterSpace();

  public:
    explicit FpgaTarget(Arch& _arch);
    ~FpgaTarget() override;

    void reset() override;

    void process(std::shared_ptr<Future> _future) override;
};

//-------------------------------------------------------------------------------------
