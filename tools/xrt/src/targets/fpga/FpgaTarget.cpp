//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/arch/Arch.hpp>
#include <targets/fpga/FpgaTarget.hpp>

#include <cstdlib>

#include <targets/common/Future.hpp>
#include <fmt/core.h>

//-------------------------------------------------------------------------------------
FpgaTarget::FpgaTarget(Arch& arch) : uioDevice_(cUioDevicePath, cRegisterSpaceSize), arch_(arch) {}


//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() = default;

//-------------------------------------------------------------------------------------
void FpgaTarget::reset() {
    
}

//-------------------------------------------------------------------------------------
void FpgaTarget::process(std::shared_ptr<Future> future) {
    auto registerReadFuture    = std::dynamic_pointer_cast<RegisterReadFuture>(future);
    auto registerWriteFuture   = std::dynamic_pointer_cast<RegisterWriteFuture>(future);
    auto matrixViewReadFuture  = std::dynamic_pointer_cast<MatrixViewReadFuture>(future);
    auto matrixViewWriteFuture = std::dynamic_pointer_cast<MatrixViewWriteFuture>(future);

    if (registerReadFuture != nullptr) {
        *registerReadFuture->dataLocation = uioDevice_.readRegister(registerReadFuture->address);
        registerReadFuture->setDone();
    }

    if (registerWriteFuture != nullptr) {
        uioDevice_.writeRegister(registerWriteFuture->address, registerWriteFuture->data);
        registerWriteFuture->setDone();
    }

    if (matrixViewReadFuture != nullptr) {
        dma_.blockingMatrixViewRead(matrixViewReadFuture->getMatrixView());
        matrixViewReadFuture->setDone();
    }

    if (matrixViewWriteFuture != nullptr) {
        dma_.blockingMatrixViewWrite(matrixViewWriteFuture->getMatrixView());
        matrixViewWriteFuture->setDone();
    }
}

//-------------------------------------------------------------------------------------
