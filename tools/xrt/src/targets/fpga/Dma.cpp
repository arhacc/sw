///
/// \file Dma.cpp
///
/// \brief Implementation of class Dma.
#include <common/allocator/SAllocator.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <targets/fpga/Dma.hpp>

#include <cstdint>

#include <unistd.h>

constexpr bool is64bit = sizeof(std::uintptr_t) > 4;

// Dma::TXDescriptorMC
void Dma::MCDescriptor::zero(std::string_view descriptorName) volatile {
    const uintptr_t thisPhysAddr = gsAllocator->getPhysicalAddress(this);

    NEXTDESC = thisPhysAddr;
    if constexpr (is64bit) {
        NEXTDESC_MSB = thisPhysAddr >> 32;
    } else {
        NEXTDESC_MSB = 0;
    }
    BUFFER_ADDRESS     = 0;
    BUFFER_ADDRESS_MSB = 0;
    MC_CTL             = 0b011 << 24; // ARCHACE to default value
    STRIDE_VSIZE       = 0;
    HSIZE              = 0;
    MC_STS             = 0;

    logWork.println<Debug>("Zeroing descriptor {}", descriptorName);
}

void Dma::MCDescriptor::setNextDescriptor(
    std::string_view descriptorName, const std::uintptr_t nextDescriptorPhysAddr) volatile {
    NEXTDESC = static_cast<std::uint32_t>(nextDescriptorPhysAddr);
    if constexpr (sizeof nextDescriptorPhysAddr > 4) {
        NEXTDESC_MSB = static_cast<std::uint32_t>(nextDescriptorPhysAddr >> 32);
    }

    logWork.println<Debug>("Seting NEXTDESC for descriptor {}: LSB={}, MSB={}", descriptorName, NEXTDESC, NEXTDESC_MSB);
}

void Dma::MCDescriptor::setBufferAddress(
    std::string_view descriptorName, const std::uintptr_t bufferPhysAddr) volatile {
    BUFFER_ADDRESS = static_cast<std::uint32_t>(bufferPhysAddr);
    if constexpr (sizeof bufferPhysAddr > 4) {
        BUFFER_ADDRESS_MSB = static_cast<std::uint32_t>(bufferPhysAddr >> 32);
    }

    logWork.println<Debug>(
        "Seting BUFFER_ADDRESS for descriptor {}: LSB={}, MSB={}", descriptorName, BUFFER_ADDRESS, BUFFER_ADDRESS_MSB);
}

void Dma::MCDescriptor::setDimensions(
    const std::string_view descriptorName,
    const std::uint32_t hsize,
    const std::uint32_t vsize,
    const std::uint32_t stride,
    const bool tx) volatile {
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
    if (tx) {
        HSIZE = hsize | (1 << 26) | (1 << 27);
    } else {
        HSIZE = hsize;
    }
    MC_CTL = 0;

    logWork.println<Debug>(
        "Setting dimensions for buffer {}: hsize={}, vsize={}, stride={}, STRIDE_VSIZE={}, HSIZE={}",
        descriptorName,
        hsize,
        vsize,
        stride,
        STRIDE_VSIZE,
        HSIZE);
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
Dma::Dma() : uioDevice_(UioDevice::fromName("Dma", cRegisterSpaceSize)) {
    reset();

    bool mm2sReportsSG = (uioDevice_.readRegister(MM2S_DMASR_ADDR) & MM2S_DMASR_SGIncld) != 0;
    bool s2mmReportsSG = (uioDevice_.readRegister(S2MM_DMASR_ADDR) & S2MM_DMASR_SGIncld) != 0;

    if (mm2sReportsSG != s2mmReportsSG) {
        throw std::runtime_error("MM2S_DMASR_SGIncld != S2MM_DMASR_SGIncld");
    }

    bool haveSG = mm2sReportsSG;

    type_ = haveSG ? Type::ScatterGatherMC : Type::Direct;

    if (type_ == Type::ScatterGatherMC) {
        txDescriptor_ = reinterpret_cast<volatile MCDescriptor*>(
            gsAllocator->allocate(std::max(sizeof(MCDescriptor), cMCDescriptorAlign)));

        rxDescriptor_ = reinterpret_cast<volatile MCDescriptor*>(
            gsAllocator->allocate(std::max(sizeof(MCDescriptor), cMCDescriptorAlign)));

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

void Dma::beginReadMatrix(MatrixView& view) {
    if (type_ == Type::ScatterGatherMC) {
        beginReadTransferScatterGatherMC(view);
    } else {
        throw std::runtime_error("Dma: unimplemented");
    }
}

void Dma::waitReadMatrix() const {
    if (type_ == Type::ScatterGatherMC) {
        waitReadTransferScatterGatherMC();
    } else {
        throw std::runtime_error("Dma: unimplemented");
    }
}

void Dma::beginWriteMatrix(const MatrixView& view) {
    if (type_ == Type::ScatterGatherMC) {
        beginWriteTransferScatterGatherMC(view);
    } else {
        throw std::runtime_error("Dma: unimplemented");
    }
}

void Dma::waitWriteMatrix() const {
    if (type_ == Type::ScatterGatherMC) {
        waitWriteTransferScatterGatherMC();
    } else {
        throw std::runtime_error("Dma: unimplemented");
    }
}

void Dma::beginWriteTransferScatterGatherMC(const MatrixView& view) {
    txDescriptor_->zero("tx");
    txDescriptor_->setBufferAddress("tx", view.physicalAddress());
    txDescriptor_->setDimensions(
        "tx", view.numColumns() * sizeof(uint32_t), view.numRows(), view.totalColumns() * sizeof(uint32_t), true);

    const std::uintptr_t txDescriptorPhysAddr = gsAllocator->getPhysicalAddress(txDescriptor_);

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 0);
    uioDevice_.writeRegister(MM2S_DMASR_ADDR, 0);

    uioDevice_.writeRegister(MM2S_CURDESC, txDescriptorPhysAddr);
    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(MM2S_CURDESC_MSB, txDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(MM2S_CURDESC_MSB, 0);
    }

    uioDevice_.writeRegister(MM2S_DMACR_ADDR, 1);

    uioDevice_.writeRegister(MM2S_TAILDESC, txDescriptorPhysAddr);

    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(MM2S_TAILDESC_MSB, txDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(MM2S_TAILDESC_MSB, 0);
    }
}

void Dma::waitWriteTransferScatterGatherMC() const {
    logWork.println<Debug>("Waiting MM2S");
    while (!txDescriptor_->isDone()) {
    }
}

void Dma::beginReadTransferScatterGatherMC(MatrixView& view) {
    rxDescriptor_->zero("rx");
    rxDescriptor_->setBufferAddress("rx", view.physicalAddress());
    rxDescriptor_->setDimensions(
        "rx", view.numColumns() * sizeof(uint32_t), view.numRows(), view.totalColumns() * sizeof(uint32_t), false);

    const std::uintptr_t rxDescriptorPhysAddr = gsAllocator->getPhysicalAddress(rxDescriptor_);

    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_DMASR_ADDR, 0);
    uioDevice_.writeRegister(S2MM_CURDESC, rxDescriptorPhysAddr);
    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(S2MM_CURDESC_MSB, rxDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(S2MM_CURDESC_MSB, 0);
    }
    uioDevice_.writeRegister(S2MM_DMACR_ADDR, 1);

    uioDevice_.writeRegister(S2MM_TAILDESC, rxDescriptorPhysAddr);
    if constexpr (sizeof(std::uintptr_t) > 4) {
        uioDevice_.writeRegister(S2MM_TAILDESC_MSB, rxDescriptorPhysAddr >> 32);
    } else {
        uioDevice_.writeRegister(S2MM_TAILDESC_MSB, 0);
    }
}

void Dma::waitReadTransferScatterGatherMC() const {
    logWork.println<Debug>("Waiting S2MM");

    while (!rxDescriptor_->isDone()) {
    }
}
