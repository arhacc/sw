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
#include <common/log/Logger.hpp>

#include <cstring>

// Dma::TXDescriptorMC
void Dma::MCDescriptor::zero(std::string_view descriptorName) volatile {
    uintptr_t thisPhysAddr = gsAllocator->getPhysicalAddress(this);

    NEXTDESC = thisPhysAddr;
    if constexpr (sizeof thisPhysAddr > 4) {
        NEXTDESC_MSB = thisPhysAddr >> 32;
    } else {
        NEXTDESC_MSB = 0;
    }
    BUFFER_ADDRESS = 0;
    BUFFER_ADDRESS_MSB = 0;
    MC_CTL = 0b011 << 24; // ARCHACE to default value
    STRIDE_VSIZE = 0;
    HSIZE = 0;
    MC_STS = 0;

    logWork.println<Debug>("Zeroing descriptor {}", descriptorName);
}

void Dma::MCDescriptor::setNextDescriptor(std::string_view descriptorName, std::uintptr_t nextDescriptorPhysAddr) volatile {
    NEXTDESC = static_cast<std::uint32_t>(nextDescriptorPhysAddr);
    if constexpr (sizeof nextDescriptorPhysAddr > 4) {
        NEXTDESC_MSB = static_cast<std::uint32_t>(nextDescriptorPhysAddr >> 32);
    }

    logWork.println<Debug>("Seting NEXTDESC for descriptor {}: LSB={}, MSB={}", descriptorName, NEXTDESC, NEXTDESC_MSB);
}

void Dma::MCDescriptor::setBufferAddress(std::string_view descriptorName, std::uintptr_t bufferPhysAddr) volatile {
    BUFFER_ADDRESS = static_cast<std::uint32_t>(bufferPhysAddr);
    if constexpr (sizeof bufferPhysAddr > 4) {
        BUFFER_ADDRESS_MSB = static_cast<std::uint32_t>(bufferPhysAddr >> 32);
    }

    logWork.println<Debug>("Seting BUFFER_ADDRESS for descriptor {}: LSB={}, MSB={}", descriptorName, BUFFER_ADDRESS, BUFFER_ADDRESS_MSB);
}

void Dma::MCDescriptor::setDimensions(std::string_view descriptorName, std::uint32_t hsize, std::uint32_t vsize, std::uint32_t stride) volatile {
    constexpr std::uint32_t max16 = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t max13 = max16 >> 3;

    if (hsize > max16) {
        throw std::runtime_error(fmt::format("in tx descriptor hsize {} greater than max of {}", hsize, max16));
    }

    if (vsize > max13) {
        throw std::runtime_error(fmt::format("in tx descriptor vsize {} greater than max of {}", vsize, max13));
    }

    if (stride > max16) {
        throw std::runtime_error(fmt::format("in tx descriptor stride {} greater than max of {}", stride, max16));
    }

    STRIDE_VSIZE = (vsize << 19) | stride;
    HSIZE = hsize | (1 << 26) | (1 << 27);
    MC_CTL = 0;

    logWork.println<Debug>("Setting dimensions for buffer {}: hsize={}, vsize={}, stride={}, STRIDE_VSIZE={}, HSIZE={}", descriptorName, hsize, vsize, stride, STRIDE_VSIZE, HSIZE);
}

bool Dma::MCDescriptor::isDone() volatile const {
    if (MC_STS & MC_STS_IE) {
        throw std::runtime_error("DMA Internal Error");
    }

    if (MC_STS & MC_STS_SE) {
        throw std::runtime_error("DMA Slave Error");
    }

    if (MC_STS & MC_STS_DE) {
        throw std::runtime_error("DMA Decode Error");
    }

    if (MC_STS & MC_STS_Cmp) {
        return true;
    } else {
        return false;
    }
}

// Dma
Dma::Dma() : uioDevice_("Dma", cUioDevicePath, cRegisterSpaceSize) {
    reset();

    bool mm2sReportsSG = (uioDevice_.readRegister(MM2S_DMASR_ADDR) & MM2S_DMASR_SGIncld) != 0;
    bool s2mmReportsSG = (uioDevice_.readRegister(S2MM_DMASR_ADDR) & S2MM_DMASR_SGIncld) != 0;

    if (mm2sReportsSG != s2mmReportsSG) {
        throw std::runtime_error("MM2S_DMASR_SGIncld != S2MM_DMASR_SGIncld");
    }

    bool haveSG = mm2sReportsSG;

    type_ = haveSG ? Type::ScatterGatherMC : Type::Direct;

    if (type_ == Type::ScatterGatherMC) {
        txDescriptor_ = reinterpret_cast<volatile MCDescriptor *>(
            gsAllocator->allocate(std::max(sizeof(MCDescriptor), cMCDescriptorAlign))
        );

        rxDescriptor_ = reinterpret_cast<volatile MCDescriptor *>(
            gsAllocator->allocate(std::max(sizeof(MCDescriptor), cMCDescriptorAlign))
        );

        std::uintptr_t txDescriptorPhysAddr = gsAllocator->getPhysicalAddress(txDescriptor_);
        uioDevice_.writeRegister(MM2S_CURDESC, txDescriptorPhysAddr);
        if constexpr (sizeof(std::uintptr_t) > 4) {
            uioDevice_.writeRegister(MM2S_CURDESC_MSB, txDescriptorPhysAddr >> 32);
        }

        std::uintptr_t rxDescriptorPhysAddr = gsAllocator->getPhysicalAddress(rxDescriptor_);
        uioDevice_.writeRegister(S2MM_CURDESC, rxDescriptorPhysAddr);
        if constexpr (sizeof(std::uintptr_t) > 4) {
            uioDevice_.writeRegister(S2MM_CURDESC_MSB, rxDescriptorPhysAddr >> 32);
        }
    }
}

Dma::~Dma() {
    if (type_ == Type::ScatterGatherMC) {
        gsAllocator->deallocate(txDescriptor_);
        gsAllocator->deallocate(rxDescriptor_);
    }
}

void Dma::reset() {
    // Reseting either MM2S or S2MM resets the entire DMA engine
    uioDevice_.writeRegister(MM2S_DMACR_ADDR, MM2S_DMACR_Reset);

    usleep(200 * 1000);

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMASR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMASR_ADDR, 0);
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
void Dma::beginWriteTransferDirect(std::uintptr_t physAddress, std::size_t length) {
    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMASR_ADDR, 0);

    uioDevice_.writeRegister(MM2S_SA_ADDR, physAddress);
    uioDevice_.writeRegister(MM2S_SA_MSB_ADDR, physAddress >> 32);

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 1);
    uioDevice_.writeRegister(MM2S_LENGTH_ADDR, (length % 8 == 0) ? length : ((length + 8) / 8) * 8);
    
}

//-------------------------------------------------------------------------------------
void Dma::waitWriteTransferDoneDirect() {
    while (!(uioDevice_.readRegister(MM2S_DMASR_ADDR) & MM2S_DMASR_Idle)) {}
}

//-------------------------------------------------------------------------------------
void Dma::beginReadTransferDirect(std::uintptr_t physAddress, std::size_t length) {
    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMASR_ADDR, 0);

    uioDevice_.writeRegister(S2MM_DA_ADDR, physAddress);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    uioDevice_.writeRegister(S2MM_DA_MSB_ADDR, physAddress >> 32);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));

    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 1);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    uioDevice_.writeRegister(S2MM_LENGTH_ADDR, (length % 8 == 0) ? length : ((length + 8) / 8) * 8);
    printStatusRegister(uioDevice_.readRegister(S2MM_DMASR_ADDR));
    
}

void Dma::beginWriteTransferScatterGatherMC(std::shared_ptr<const MatrixView> view) {
    txDescriptor_->zero("tx");
    txDescriptor_->setBufferAddress("tx", view->physicalAddress());
    txDescriptor_->setDimensions("tx", view->numColumns() * sizeof(uint32_t), view->numRows(), view->totalColumns() * sizeof(uint32_t));

    std::uintptr_t txDescriptorPhysAddr = gsAllocator->getPhysicalAddress(txDescriptor_);

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMASR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 1);

    uioDevice_.writeRegister(MM2S_TAILDESC, txDescriptorPhysAddr);

    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(MM2S_TAILDESC_MSB, txDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(MM2S_TAILDESC_MSB, 0);
    }
}

void Dma::waitWriteTransferScatterGatherMC() {
    while (!txDescriptor_->isDone()) {
        printf("Waiting MM2S\n");
    }

    
}

void Dma::beginReadTransferScatterGatherMC(std::shared_ptr<MatrixView> view) {
    rxDescriptor_->zero("rx");
    rxDescriptor_->setBufferAddress("rx", view->physicalAddress());
    rxDescriptor_->setDimensions("rx", view->numColumns() * sizeof(uint32_t), view->numRows(), view->totalColumns() * sizeof(uint32_t));

    std::uintptr_t rxDescriptorPhysAddr = gsAllocator->getPhysicalAddress(rxDescriptor_);

    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMASR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 1);

    uioDevice_.writeRegister(S2MM_TAILDESC, rxDescriptorPhysAddr);
    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(S2MM_TAILDESC_MSB, rxDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(S2MM_TAILDESC_MSB, 0);
    }
}

void Dma::waitReadTransferScatterGatherMC() {
    while (!rxDescriptor_->isDone()) {
        printf("Waiting S2MM\n");
    }
}

//-------------------------------------------------------------------------------------
void Dma::waitReadTransferDoneDirect() {
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
    if (type_ == Type::Direct) {
        for (std::size_t i = 0; i < view->numRows(); i++) {
            std::uintptr_t physAddress = gsAllocator->getPhysicalAddress(&view->at(i, 0));

            beginWriteTransferDirect(physAddress, view->numColumns() * sizeof(uint32_t));
            waitWriteTransferDoneDirect();
        }
    } else {
        beginWriteTransferScatterGatherMC(view);
        waitWriteTransferScatterGatherMC();
    }
}

//-------------------------------------------------------------------------------------
void Dma::blockingMatrixViewRead(std::shared_ptr<MatrixView> view) {
    if (type_ == Type::Direct) {
        for (std::size_t i = 0; i < view->numRows(); i++) {
            std::uintptr_t physAddress = gsAllocator->getPhysicalAddress(&view->at(i, 0));

            beginReadTransferDirect(physAddress, view->numColumns() * sizeof(uint32_t));
            waitReadTransferDoneDirect();
        }
    } else {
        beginReadTransferScatterGatherMC(view);
        waitReadTransferScatterGatherMC();
    }
}

//-------------------------------------------------------------------------------------
