//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
// FpgaTarget -- XPU AXI Driver
//-------------------------------------------------------------------------------------
/*
        example of basic program for the xpu system.
                write program file into xpu
                get data in into the xpu system using dma mechanism
                do computations (done automatically)
                get data out from the xpu system using dma mechanism
 */
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <sys/mman.h>
#include <termios.h>
#include <unistd.h>

#undef MAP_TYPE /* Ugly Hack */

#include <targets/common/Target.h>

#include <climits>

//-------------------------------------------------------------------------------------
// XPU defines

#define XPU_BASE_ADDR 0x40000000

#define DMA_TRANSFER_TIMEOUT 1 // seconds

// DMA defines
#define DMA_BASE_ADDR 0x40400000

#define DMA_MM2S_DMACR_OFFSET  0x00
#define DMA_MM2S_DMASR_OFFSET  0x04
#define DMA_MM2S_SA_LSB_OFFSET 0x18
#define DMA_MM2S_SA_MSB_OFFSET 0x1C
#define DMA_MM2S_LENGTH_OFFSET 0x28
#define DMA_S2MM_DMACR_OFFSET  0x30
#define DMA_S2MM_DMASR_OFFSET  0x34
#define DMA_S2MM_DA_LSB_OFFSET 0x48
#define DMA_S2MM_DA_MSB_OFFSET 0x4C
#define DMA_S2MM_LENGTH_OFFSET 0x58

// MM2S
//	MM2S_DMACR - control register
// rw
#define DMA_MM2S_DMACR_X_RS_LOC    0
#define DMA_MM2S_DMACR_X_RS_SIZE   1
#define DMA_MM2S_DMACR_X_RS_X_STOP 0
#define DMA_MM2S_DMACR_X_RS_X_RUN  1
// rw
#define DMA_MM2S_DMACR_X_RESET_LOC      2
#define DMA_MM2S_DMACR_X_RESET_SIZE     1
#define DMA_MM2S_DMACR_X_RESET_X_PAUSED 0
#define DMA_MM2S_DMACR_X_RESET_X_START  1
// rw
#define DMA_MM2S_DMACR_X_IOC_IrqEn_LOC        12
#define DMA_MM2S_DMACR_X_IOC_IrqEn_SIZE       1
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_ENABLED  1
// rw
#define DMA_MM2S_DMACR_X_Dly_IrqEn_LOC        13
#define DMA_MM2S_DMACR_X_Dly_IrqEn_SIZE       1
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_ENABLED  1
// rw
#define DMA_MM2S_DMACR_X_Err_IrqEn_LOC        14
#define DMA_MM2S_DMACR_X_Err_IrqEn_SIZE       1
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_ENABLED  1

//	MM2S_DMASR - status register
// ro
#define DMA_MM2S_DMASR_X_Halted_LOC       0
#define DMA_MM2S_DMASR_X_Halted_SIZE      1
#define DMA_MM2S_DMASR_X_Halted_X_RUNNING 0
#define DMA_MM2S_DMASR_X_Halted_X_HALTED  1
// ro
#define DMA_MM2S_DMASR_X_Idle_LOC        1
#define DMA_MM2S_DMASR_X_Idle_SIZE       1
#define DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_MM2S_DMASR_X_Idle_X_IDLE     1
// ro
#define DMA_MM2S_DMASR_X_SGIncld_LOC         3
#define DMA_MM2S_DMASR_X_SGIncld_SIZE        1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_EN     1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_not_EN 0
// ro
#define DMA_MM2S_DMASR_X_DMASlvErr_LOC               5
#define DMA_MM2S_DMASR_X_DMASlvErr_SIZE              1
#define DMA_MM2S_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMASlvErr_X_ERR_DETECTED    1
// ro
#define DMA_MM2S_DMASR_X_DMADecErr_LOC               6
#define DMA_MM2S_DMASR_X_DMADecErr_SIZE              1
#define DMA_MM2S_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMADecErr_X_ERR_DETECTED    1
// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_IOC_Irq_LOC                   12
#define DMA_MM2S_DMASR_X_IOC_Irq_SIZE                  1
#define DMA_MM2S_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED    1
// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_Err_Irq_LOC                   14
#define DMA_MM2S_DMASR_X_Err_Irq_SIZE                  1
#define DMA_MM2S_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_Err_Irq_X_ERR_INT_DETECTED    1

// MM2S_SA_LSB - source address
// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_LOC           0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_SIZE          32
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0

// MM2S_SA_MSB - source address
// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_LOC           0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_SIZE          32
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0

// MM2S_LENGTH
// R/W
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_LOC      0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_SIZE     26
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0

// S2MM
//	S2MM_DMACR - control register
// rw
#define DMA_S2MM_DMACR_X_RS_LOC    0
#define DMA_S2MM_DMACR_X_RS_SIZE   1
#define DMA_S2MM_DMACR_X_RS_X_STOP 0
#define DMA_S2MM_DMACR_X_RS_X_RUN  1
// rw
#define DMA_S2MM_DMACR_X_RESET_LOC      2
#define DMA_S2MM_DMACR_X_RESET_SIZE     1
#define DMA_S2MM_DMACR_X_RESET_X_PAUSED 0
#define DMA_S2MM_DMACR_X_RESET_X_START  1
// rw
#define DMA_S2MM_DMACR_X_IOC_IrqEn_LOC        12
#define DMA_S2MM_DMACR_X_IOC_IrqEn_SIZE       1
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_ENABLED  1
// rw
#define DMA_S2MM_DMACR_X_Dly_IrqEn_LOC        13
#define DMA_S2MM_DMACR_X_Dly_IrqEn_SIZE       1
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_ENABLED  1
// rw
#define DMA_S2MM_DMACR_X_Err_IrqEn_LOC        14
#define DMA_S2MM_DMACR_X_Err_IrqEn_SIZE       1
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_ENABLED  1

//	S2MM_DMASR - status register
// ro
#define DMA_S2MM_DMASR_X_Halted_LOC       0
#define DMA_S2MM_DMASR_X_Halted_SIZE      1
#define DMA_S2MM_DMASR_X_Halted_X_RUNNING 0
#define DMA_S2MM_DMASR_X_Halted_X_HALTED  1
// ro
#define DMA_S2MM_DMASR_X_Idle_LOC        1
#define DMA_S2MM_DMASR_X_Idle_SIZE       1
#define DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_S2MM_DMASR_X_Idle_X_IDLE     1
// ro
#define DMA_S2MM_DMASR_X_SGIncld_LOC         3
#define DMA_S2MM_DMASR_X_SGIncld_SIZE        1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_EN     1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_not_EN 0
// ro
#define DMA_S2MM_DMASR_X_DMASlvErr_LOC               5
#define DMA_S2MM_DMASR_X_DMASlvErr_SIZE              1
#define DMA_S2MM_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMASlvErr_X_ERR_DETECTED    1
// ro
#define DMA_S2MM_DMASR_X_DMADecErr_LOC               6
#define DMA_S2MM_DMASR_X_DMADecErr_SIZE              1
#define DMA_S2MM_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMADecErr_X_ERR_DETECTED    1
// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_IOC_Irq_LOC                   12
#define DMA_S2MM_DMASR_X_IOC_Irq_SIZE                  1
#define DMA_S2MM_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED    1
// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_Err_Irq_LOC                   14
#define DMA_S2MM_DMASR_X_Err_Irq_SIZE                  1
#define DMA_S2MM_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_Err_Irq_X_ERR_INT_DETECTED    1

// S2MM_SA_LSB - source address
// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_LOC           0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_SIZE          32
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0

// S2MM_SA_MSB - source address
// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_LOC           0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_SIZE          32
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0

// S2MM_LENGTH
// R/W
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_LOC      0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_SIZE     26
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0

//-------------------------------------------------------------------------------------
class FpgaTarget : public Target {
    uint32_t* XPU_POINTER_CONSTANT;
    uint32_t* DMA_POINTER_CONSTANT;
    int32_t memory_file_descriptor;

    volatile uint32_t* io_matrix    = nullptr;
    uint32_t io_matrix_raw_position = 0;
    size_t io_matrix_max_size       = 0;

    const Arch& arch;

    static void AXI_LITE_write(uint32_t* _address, uint32_t value);

    static uint32_t AXI_LITE_read(const uint32_t* _address);

    static void AXI_LITE_set_bits(uint32_t* _address, uint32_t _mask);

    static void AXI_LITE_clear_bits(uint32_t* _address, uint32_t _mask);

    static void dma_mm2s_status(uint32_t* DMA_POINTER_CONSTANT);

    static void dma_s2mm_status(uint32_t* DMA_POINTER_CONSTANT);

    static void print_all_registers_mm2s(uint32_t* DMA_POINTER_CONSTANT, int tag);

    static void print_all_registers_s2mm(uint32_t* DMA_POINTER_CONSTANT, int tag);

    static void dma_mm2s_wait_transfers_complete(uint32_t* DMA_POINTER_CONSTANT);

    static void dma_s2mm_wait_transfers_complete(uint32_t* DMA_POINTER_CONSTANT);

    // Write from RAM to FPGA
    static void DMA_write(
        uint32_t* DMA_POINTER_CONSTANT,
        uint32_t ddr_start_addr,
        uint32_t transfer_length);

    // Read from FPGA to RAM
    static void DMA_read(
        uint32_t* DMA_POINTER_CONSTANT,
        uint32_t ddr_start_addr,
        uint32_t transfer_length);

    static void dma_reset(uint32_t* DMA_POINTER_CONSTANT);

    static void print_main_mem(uint32_t* address, int32_t nr_bytes, uint32_t word_size);

  public:
    FpgaTarget(Arch& _arch);

    ~FpgaTarget() override;

    void reset() override;

    uint32_t readRegister(uint32_t _address) override;

    void writeRegister(uint32_t _address, uint32_t _value) override;

    void writeInstruction(uint32_t _instruction) override;

    void getMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;

    void sendMatrixArray(
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns) override;
};

//-------------------------------------------------------------------------------------
