///
/// \file FpgaTarget.cpp
///
/// \brief Implementation of class FpgaTarget.

#include <common/arch/Arch.hpp>
#include <targets/fpga/FpgaTarget.hpp>

FpgaTarget::FpgaTarget(Arch& arch) : uioDevice_("xpu", cUioDevicePath, cRegisterSpaceSize), arch_(arch) {}

FpgaTarget::~FpgaTarget() = default;

void FpgaTarget::reset() {
    dma_.reset();
}

std::uint32_t FpgaTarget::readRegister(const std::uint32_t address) {
    return uioDevice_.readRegister(address);
}

void FpgaTarget::writeRegister(const std::uint32_t address, const std::uint32_t data) {
    uioDevice_.writeRegister(address, data);
}

void FpgaTarget::readMatrixBefore(MatrixView& view) {
    dma_.beginReadMatrix(view);
}

void FpgaTarget::readMatrixAfter([[maybe_unused]] MatrixView& view) {
    dma_.waitReadMatrix();
}

void FpgaTarget::writeMatrixBefore(const MatrixView& view) {
    dma_.beginWriteMatrix(view);
}

void FpgaTarget::writeMatrixAfter([[maybe_unused]] const MatrixView& view) {
    dma_.waitWriteMatrix();
}

