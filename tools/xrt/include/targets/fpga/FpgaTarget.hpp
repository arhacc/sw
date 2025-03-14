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
class FpgaTarget final : public Target {
    FRIEND_TEST(FpgaTargetTest, SaneArchitectureHashRawAccess);
    FRIEND_TEST(FpgaTargetTest, WriteIDRawAccess);

    static constexpr size_t cRegisterSpaceSize   = 5012; // TODO: find out how much is actually mapped
    static constexpr char   cUioDevicePath[]     = "/dev/uio1";

    UioDevice uioDevice_;
    Dma dma_;

    Arch& arch_;

  public:
    explicit FpgaTarget(Arch& arch);
    ~FpgaTarget() override;

    void reset() override;

    std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation) override;
    std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data) override;
    std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) override;
    std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) override;
};

//-------------------------------------------------------------------------------------
