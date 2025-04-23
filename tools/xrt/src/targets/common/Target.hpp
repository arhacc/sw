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

    virtual void reset() = 0;

    virtual std::uint32_t readRegister(std::uint32_t address) = 0;
    virtual void writeRegister(std::uint32_t address, std::uint32_t data) = 0;
    virtual void readMatrixBefore(MatrixView& view) = 0;
    virtual void readMatrixAfter(MatrixView& view) = 0;
    virtual void writeMatrixBefore(const MatrixView& view) = 0;
    virtual void writeMatrixAfter(const MatrixView& view) = 0;
};
