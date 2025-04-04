///
/// \file Dma.hpp
///
/// \brief Definition of class Dma.
#pragma once

#include <targets/fpga/UioDevice.hpp>

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <thread>

// Forward declarations
class MatrixView;
class Future;

///
/// \brief Manages communication with the AXI Lite DMA.
///
/// https://docs.amd.com/r/en-US/pg021_axi_dma/Product-Specification
class Dma {
  public:
    enum class Type {
        Direct,
        ScatterGatherMC,
    };

  private:
    static constexpr size_t cRegisterSpaceSize = 0x5C;
    static constexpr char cUioDevicePath[]     = "/dev/uio0";

    // Direct mode registers
    static constexpr std::size_t MM2S_DMACR_ADDR  = 0x00; // control register
    static constexpr std::size_t MM2S_DMASR_ADDR  = 0x04; // status register
    static constexpr std::size_t MM2S_SA_ADDR     = 0x18; // source address
    static constexpr std::size_t MM2S_SA_MSB_ADDR = 0x1C; // source address MSB (unused for 32 bit)
    static constexpr std::size_t MM2S_LENGTH_ADDR = 0x28; // transfer length

    static constexpr std::size_t S2MM_DMACR_ADDR  = 0x30; // control register
    static constexpr std::size_t S2MM_DMASR_ADDR  = 0x34; // status register
    static constexpr std::size_t S2MM_DA_ADDR     = 0x48; // source address
    static constexpr std::size_t S2MM_DA_MSB_ADDR = 0x4C; // source address MSB (unused for 32 bit)
    static constexpr std::size_t S2MM_LENGTH_ADDR = 0x58; // transfer length

    // Direct mode register fields
    static constexpr std::uint32_t MM2S_DMACR_Reset = 1 << 2;
    static constexpr std::uint32_t S2MM_DMACR_Reset = 1 << 2;

    static constexpr std::uint32_t MM2S_DMASR_Halted  = 1 << 0;
    static constexpr std::uint32_t MM2S_DMASR_Idle    = 1 << 1;
    static constexpr std::uint32_t MM2S_DMASR_SGIncld = 1 << 3;
    static constexpr std::uint32_t S2MM_DMASR_Halted  = 1 << 0;
    static constexpr std::uint32_t S2MM_DMASR_Idle    = 1 << 1;
    static constexpr std::uint32_t S2MM_DMASR_SGIncld = 1 << 3;

    // Scatter gather mode registers
    // static constexpr std::size_t MM2S_DMACR_ADDR  = 0x00; // control register
    // static constexpr std::size_t MM2S_DMASR_ADDR  = 0x04; // status register
    static constexpr std::size_t MM2S_CURDESC      = 0x08; // current TX descriptor address
    static constexpr std::size_t MM2S_CURDESC_MSB  = 0x0C; // current TX descriptor address MSB (unused for 32 bit)
    static constexpr std::size_t MM2S_TAILDESC     = 0x10; // tail    TX descriptor address
    static constexpr std::size_t MM2S_TAILDESC_MSB = 0x14; // tail    TX descriptor address MSB (unused for 32 bit)

    static constexpr std::size_t SG_CTL = 0x2C;

    // static constexpr std::size_t S2MM_DMACR_ADDR  = 0x30; // control register
    // static constexpr std::size_t S2MM_DMASR_ADDR  = 0x34; // status register
    static constexpr std::size_t S2MM_CURDESC      = 0x38; // current RX descriptor address
    static constexpr std::size_t S2MM_CURDESC_MSB  = 0x3C; // current RX descriptor address MSB (unused for 32 bit)
    static constexpr std::size_t S2MM_TAILDESC     = 0x40; // tail    RX descriptor address
    static constexpr std::size_t S2MM_TAILDESC_MSB = 0x44; // tail    RX descriptor address MSB (unused for 32 bit)

    __attribute__((packed)) struct MCDescriptor {
        static constexpr std::uint32_t MC_STS_IE  = 1 << 28;
        static constexpr std::uint32_t MC_STS_SE  = 1 << 29;
        static constexpr std::uint32_t MC_STS_DE  = 1 << 30;
        static constexpr std::uint32_t MC_STS_Cmp = 1 << 31;

        std::uint32_t NEXTDESC;
        std::uint32_t NEXTDESC_MSB;
        std::uint32_t BUFFER_ADDRESS;
        std::uint32_t BUFFER_ADDRESS_MSB;
        std::uint32_t MC_CTL;
        std::uint32_t STRIDE_VSIZE;
        std::uint32_t HSIZE;
        std::uint32_t MC_STS;

        void zero(std::string_view descriptorName) volatile;
        void setNextDescriptor(std::string_view descriptorName, uintptr_t nextDescriptorPhysAddr) volatile;
        void setBufferAddress(std::string_view descriptorName, uintptr_t bufferPhysAddr) volatile;
        void setDimensions(
            std::string_view descriptorName,
            std::uint32_t hsize,
            std::uint32_t vsize,
            std::uint32_t stride,
            bool tx) volatile;

        [[nodiscard]] bool isDone() volatile const;
    };

    static constexpr std::size_t cMCDescriptorAlign = 16 * sizeof(std::uint32_t);

    UioDevice uioDevice_;
    Type type_;

    volatile MCDescriptor* txDescriptor_ = nullptr;
    volatile MCDescriptor* rxDescriptor_ = nullptr;

    void beginReadTransferScatterGatherMC(MatrixView& view);
    void waitReadTransferScatterGatherMC() const;

    void beginWriteTransferScatterGatherMC(const MatrixView& view);
    void waitWriteTransferScatterGatherMC() const;

  public:
    Dma();
    ~Dma();

    void reset();

    void beginReadMatrix(MatrixView& view);
    void waitReadMatrix() const;
    void beginWriteMatrix(const MatrixView& view);
    void waitWriteMatrix() const;
};
