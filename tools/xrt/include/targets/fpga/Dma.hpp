//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include <targets/fpga/UioDevice.hpp>

// Forward declarations
class MatrixView;

class Dma {
private:
    static constexpr size_t cRegisterSpaceSize   = 0x5C;
    static constexpr char   cUioDevicePath[]     = "/dev/uio0";

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
    
    static constexpr std::uint32_t MM2S_DMACR_Reset = 1 << 2;
    static constexpr std::uint32_t S2MM_DMACR_Reset = 1 << 2;

    static constexpr std::uint32_t MM2S_DMASR_Halted  = 1 << 0;
    static constexpr std::uint32_t MM2S_DMASR_Idle    = 1 << 1;
    static constexpr std::uint32_t MM2S_DMASR_SGIncld = 1 << 3;
    static constexpr std::uint32_t S2MM_DMASR_Halted  = 1 << 0;
    static constexpr std::uint32_t S2MM_DMASR_Idle    = 1 << 1;
    static constexpr std::uint32_t S2MM_DMASR_SGIncld = 1 << 3;

    UioDevice uioDevice_;
  public:
    Dma();
    ~Dma();

    void reset();

    void beginReadTransfer(std::uintptr_t address, std::size_t length);
    void waitReadTransferDone();

    void beginWriteTransfer(std::uintptr_t address, std::size_t length);
    void waitWriteTransferDone();

    void blockingMatrixViewWrite(std::shared_ptr<const MatrixView> view);
    void blockingMatrixViewRead(std::shared_ptr<MatrixView> view);
};
