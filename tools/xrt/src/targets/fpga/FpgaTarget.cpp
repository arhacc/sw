/// \file FpgaTarget.cpp
///
/// \brief Implementation of class FpgaTarget.
///
#include <common/arch/Arch.hpp>
#include <targets/common/Future.hpp>
#include <targets/fpga/FpgaTarget.hpp>

//-------------------------------------------------------------------------------------
FpgaTarget::FpgaTarget(Arch& arch) : uioDevice_("xpu", cUioDevicePath, cRegisterSpaceSize), arch_(arch) {}


//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() = default;

std::shared_ptr<Future> FpgaTarget::readRegisterAsync(const std::uint32_t address, std::uint32_t* dataLocation) {
    *dataLocation = uioDevice_.readRegister(address);

    return std::make_shared<NopFuture>();
}

std::shared_ptr<Future> FpgaTarget::writeRegisterAsync(std::uint32_t address, std::uint32_t data) {
    uioDevice_.writeRegister(address, data);

    return std::make_shared<NopFuture>();
}

std::shared_ptr<Future> FpgaTarget::readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) {
    return dma_.createReadMatrixViewFuture(view);
}

std::shared_ptr<Future> FpgaTarget::writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) {
    return dma_.createWriteMatrixViewFuture(view);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::reset() {
    
}
//-------------------------------------------------------------------------------------
