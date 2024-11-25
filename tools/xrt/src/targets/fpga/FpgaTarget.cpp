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

#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include <targets/common/Future.hpp>
#include <fmt/core.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
FpgaTarget::FpgaTarget(Arch& arch) : arch_(arch) {
    initRegisterSpace();
}

//-------------------------------------------------------------------------------------
void FpgaTarget::initRegisterSpace() {
    registerSpaceFd_ = open("/dev/uio0", O_RDWR);
    if (registerSpaceFd_ < 0) {
  throw std::runtime_error(fmt::format("Failed to open /dev/uio0 for accelerator registers: {}", strerror(errno)));
    }

    registerSpace_ = (volatile uint32_t *) mmap(
        nullptr,
        cRegisterSpaceSize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
  registerSpaceFd_,
        0
    );
    if (registerSpace_ == NULL) {
        auto errorMsg = fmt::format("Failed to mmap /dev/uio0 for accelerator registers: {}", strerror(errno));
        close(registerSpaceFd_);
        throw std::runtime_error(errorMsg);
    }
}


//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() {
    munmap((void *) registerSpace_, cRegisterSpaceSize);
    close(registerSpaceFd_);
}

//-------------------------------------------------------------------------------------
uint32_t FpgaTarget::readRegister(size_t addr) const {
    return registerSpace_[addr / sizeof(uint32_t)];
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeRegister(size_t addr, uint32_t value) const {
    registerSpace_[addr / sizeof(uint32_t)] = value;
}

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
      *registerReadFuture->dataLocation = readRegister(registerReadFuture->address);
      registerReadFuture->setDone();
    }

    if (registerWriteFuture != nullptr) {
      writeRegister(registerWriteFuture->address, registerWriteFuture->data);
      registerWriteFuture->setDone();
    }

    if (matrixViewReadFuture != nullptr) {
      throw std::runtime_error("Unimplemented: FpgaTarget: matrixViewReadFuture");
    }

    if (matrixViewWriteFuture != nullptr) {
      throw std::runtime_error("Unimplemented: FpgaTarget: matrixViewWriteFuture");
    }
}

//-------------------------------------------------------------------------------------
