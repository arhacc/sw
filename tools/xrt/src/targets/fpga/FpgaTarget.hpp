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

    ///
    /// \breif Read a register from the accelerator.
    ///
    /// This is instant.
    ///
    /// \param address Address of register within the accelerator's address space.
    /// \param dataLocation Location (pointer) at which the read result will be stored.
    /// \return NopFuture. Programs which should also run on simulator should still wait on this.
    std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation) override;

    ///
    /// \breif Write a register from the accelerator.
    ///
    /// This is instant.
    ///
    /// \param address Address of register within the accelerator's address space.
    /// \param data Data to write to the register.
    ///  \return NopFuture. Programs which should also run on simulator should still wait on this.
    std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data) override;

    ///
    /// \breif Read a MatrixView from the accelerator data stream.
    ///
    /// This function only handles the stream data itself. The accelerator needs to be programmed to output data.
    /// The Driver class handles this.
    ///
    /// \param view MatrixView to write to.
    /// \return Future which indicates when the data has been read.
    std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) override;

    ///
    /// \breif Write a MatrixView to the accelerator data stream.
    ///
    /// This function only handles the stream data itself. The accelerator needs to be programmed to receive data.
    /// The Driver class handles this.
    ///
    /// \param view MatrixView to write.
    /// \return Future which indicates when the data has been written.
    std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) override;
};
