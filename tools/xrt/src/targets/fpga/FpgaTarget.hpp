///
/// \file FpgaTarget.hpp
///
/// \brief Definition of class FpgaTarget.
#pragma once

#include <targets/common/Target.hpp>
#include <targets/fpga/Dma.hpp>
#include <targets/fpga/UioDevice.hpp>

#include <cstdint>

#include <gtest/gtest.h>

// Forward declarations
struct Arch;

///
/// \brief Represents the interface with the accelerator as implemented on a Xilinx FPGA.
///
/// This implementation assumes an AXI Dma core is connected to the main memory and to the accelerator stream interface.
/// It also assumes the AXI Dma core's registers are available via uio on the /dev/uio0 device.
/// It also assumes the accelerator's registers are available via uio on the /dev/uio1 device.
class FpgaTarget final : public Target {
    FRIEND_TEST(FpgaTargetTest, SaneArchitectureHashRawAccess);
    FRIEND_TEST(FpgaTargetTest, WriteIDRawAccess);

    static constexpr size_t cRegisterSpaceSize = 5012; // TODO: find out how much is actually mapped
    static constexpr char cUioDevicePath[]     = "/dev/uio1";

    UioDevice uioDevice_;
    Dma dma_;

    Arch& arch_;

  public:
    ///
    /// \breif Construct the FpgaTarget.
    ///
    /// \param arch Architecture structure. This will be written back with the acrhitecture read from the device.
    explicit FpgaTarget(Arch& arch);

    ///
    /// \breif Destroy the FpgaTarget.
    ~FpgaTarget() override;

    ///
    /// \breif Reset the accelerator state.
    ///
    /// This function does not need to be called explicitly at the beginning.
    void reset() override;

    std::uint32_t readRegister(std::uint32_t address) override;

    void writeRegister(std::uint32_t address, std::uint32_t data) override;

    void readMatrixBefore(MatrixView& view) override;

    void readMatrixAfter(MatrixView& view) override;

    void writeMatrixBefore(const MatrixView& view) override;

    void writeMatrixAfter(const MatrixView& view) override;
};
