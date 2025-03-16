///
/// \file Target.hpp
///
/// \brief Definition of class Target.

#pragma once

#include <memory>

class MatrixView;
class Future;

///
/// \brief Represents the interface with the accelerator.
///
/// This is a pure virtual class derived by classes that provide the interface with the accelerator on FPGA,
/// with the simulator etc.
///
/// This class only provides physical interface to the accelerator (read/write regs, read/write matrices).
/// Higher level processing is done in the Driver class.
class Target {
  public:
    virtual ~Target();

    ///
    /// \breif Reset the accelerator state.
    ///
    /// This function does not need to be called explicitly at the beginning.
    virtual void reset() = 0;

    ///
    /// \breif Read a register from the accelerator.
    ///
    /// This sometimes take time (on simulator).
    ///
    /// \param address Address of register within the accelerator's address space.
    /// \param dataLocation Location (pointer) at which the read result will be stored.
    /// \return Future which indicates when the register has been read.
    virtual std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation) = 0;

    ///
    /// \breif Write a register from the accelerator.
    ///
    /// This sometimes takes time (on simulator).
    ///
    /// \param address Address of register within the accelerator's address space.
    /// \param data Data to write to the register.
    /// \return Future which indicates when the register has been written.
    virtual std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data) = 0;

    ///
    /// \breif Read a MatrixView from the accelerator data stream.
    ///
    /// This generally takes time.
    ///
    /// This function only handles the stream data itself. The accelerator needs to be programmed to output data.
    /// The Driver class handles this.
    ///
    /// \param view MatrixView to write to.
    /// \return Future which indicates when the data has been read.
    virtual std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) = 0;

    ///
    /// \breif Write a MatrixView to the accelerator data stream.
    ///
    /// This generally takes time.
    ///
    /// This function only handles the stream data itself. The accelerator needs to be programmed to receive data.
    /// The Driver class handles this.
    ///
    /// \param view MatrixView to write.
    /// \return Future which indicates when the data has been written.
    virtual std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) = 0;
};
