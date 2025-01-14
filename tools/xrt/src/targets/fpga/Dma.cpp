//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/fpga/Dma.hpp>
#include <cstdint>
#include "common/allocator/SAllocator.hpp"
#include "common/types/Matrix.hpp"
#include <unistd.h>

//-------------------------------------------------------------------------------------
Dma::Dma() : uioDevice_(cUioDevicePath, cRegisterSpaceSize)
{
    reset();
}

//-------------------------------------------------------------------------------------
Dma::~Dma() = default;

void Dma::reset() {
    // Reseting either MM2S or S2MM resets the entire DMA engine
    uioDevice_.writeRegister(MM2S_DMACR_ADDR, MM2S_DMACR_Reset);

    usleep(200 * 1000);
}

static void printStatusRegister(uint32_t status_reg) {
    printf(
        "S2MM status (addr offset status: 0x%x): ", status_reg);

    if (status_reg & 0x00000001) {
        printf("halted ");
    } else {
        printf("running ");
    }

    if (status_reg & 0x00000002) {
        printf("idle ");
    }
    if (status_reg & 0x00000008) {
        printf("SG enabled ");
    }
    if (status_reg & 0x00000010) {
        printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020) {
        printf("DMA slave error ");
    }
    if (status_reg & 0x00000040) {
        printf("DMA decode error ");
    }
    if (status_reg & 0x00001000) {
        printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000) {
        printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000) {
        printf("Error interrupt request ");
    }

    printf("\n");
}

//-------------------------------------------------------------------------------------
void Dma::beginWriteTransfer(std::uintptr_t physAddress, std::size_t length) {
    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMASR_ADDR, 0);

    uioDevice_.writeRegister(MM2S_SA_ADDR, physAddress);
    uioDevice_.writeRegister(MM2S_SA_MSB_ADDR, physAddress >> 32);

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 1);
    uioDevice_.writeRegister(MM2S_LENGTH_ADDR, (length % 8 == 0) ? length : (length + 8) / 8);
}

//-------------------------------------------------------------------------------------
void Dma::waitWriteTransferDone() {
    while (!(uioDevice_.readRegister(MM2S_DMASR_ADDR) & MM2S_DMASR_Idle)) {}
}

//-------------------------------------------------------------------------------------
void Dma::beginReadTransfer(std::uintptr_t physAddress, std::size_t length) {
    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMASR_ADDR, 0);

    uioDevice_.writeRegister(S2MM_DA_ADDR, physAddress);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    uioDevice_.writeRegister(S2MM_DA_MSB_ADDR, physAddress >> 32);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));

    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 1);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    uioDevice_.writeRegister(S2MM_LENGTH_ADDR, (length % 8 == 0) ? length : (length + 8) / 8);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
}

//-------------------------------------------------------------------------------------
void Dma::waitReadTransferDone() {
    printf("Begin wait for status register\n");
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    while (!(uioDevice_.readRegister(S2MM_DMASR_ADDR) & S2MM_DMASR_Idle)) {
        printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    }
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    printf("End wait for status register\n");
}

//-------------------------------------------------------------------------------------
void Dma::blockingMatrixViewWrite(std::shared_ptr<const MatrixView> view) {
    for (std::size_t i = 0; i < view->numRows(); i++) {
        std::uintptr_t physAddress = gsAllocator->getPhysicalAddress(
            const_cast<volatile void *>(
              reinterpret_cast<const volatile void *>(
                &view->at(i, 0)
              )
            )
          );

        beginWriteTransfer(physAddress, view->numColumns() * sizeof(uint32_t));
        waitWriteTransferDone();
    }
}

//-------------------------------------------------------------------------------------
void Dma::blockingMatrixViewRead(std::shared_ptr<MatrixView> view) {
    for (std::size_t i = 0; i < view->numRows(); i++) {
        std::uintptr_t physAddress = gsAllocator->getPhysicalAddress(
            reinterpret_cast<volatile void *>(
              &view->at(i, 0)
            )
          );

        beginReadTransfer(physAddress, view->numColumns() * sizeof(uint32_t));
        waitReadTransferDone();
    }
}

//-------------------------------------------------------------------------------------
