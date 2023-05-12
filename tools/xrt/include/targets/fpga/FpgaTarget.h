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

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

#undef MAP_TYPE /* Ugly Hack */

#include <climits>
#include <targets/common/Target.h>

//-------------------------------------------------------------------------------------
// XPU defines
#define XPU_NR_CELLS 64
#define XPU_BASE_ADDR 0x40000000
#define XPU_STATUS_REG_ADDR_OFFSET 0x0
#define XPU_FIFO_PROGRAM_ADDR_OFFSET 0x0

#define XPU_WRITE_INT_ACK_ADDR 4
#define XPU_READ_SEL_READABLE_REGS_ADDR 4
#define XPU_WRITE_SEL_READABLE_REGS_ADDR 8
#define XPU_READABLE_REGS_PROGRAM_COUNTER_ADDR 0
#define XPU_READABLE_REGS_CYCLE_COUNTER_ADDR 1
#define XPU_READABLE_REGS_ACCELERATOR_ID 2

// test related defines
#define NR_TRANSACTIONS XPU_NR_CELLS
#define TIME_DELAY 10000000

// DMA defines
#define DMA_BASE_ADDR 0x40400000

#define DMA_MM2S_DMACR_OFFSET 0x00
#define DMA_MM2S_DMASR_OFFSET 0x04
#define DMA_MM2S_SA_LSB_OFFSET 0x18
#define DMA_MM2S_SA_MSB_OFFSET 0x1C
#define DMA_MM2S_LENGTH_OFFSET 0x28
#define DMA_S2MM_DMACR_OFFSET 0x30
#define DMA_S2MM_DMASR_OFFSET 0x34
#define DMA_S2MM_DA_LSB_OFFSET 0x48
#define DMA_S2MM_DA_MSB_OFFSET 0x4C
#define DMA_S2MM_LENGTH_OFFSET 0x58

// MM2S
//	MM2S_DMACR - control register
// rw
#define DMA_MM2S_DMACR_X_RS_LOC 0
#define DMA_MM2S_DMACR_X_RS_SIZE 1
#define DMA_MM2S_DMACR_X_RS_X_STOP 0
#define DMA_MM2S_DMACR_X_RS_X_RUN 1
// rw
#define DMA_MM2S_DMACR_X_RESET_LOC 2
#define DMA_MM2S_DMACR_X_RESET_SIZE 1
#define DMA_MM2S_DMACR_X_RESET_X_PAUSED 0
#define DMA_MM2S_DMACR_X_RESET_X_START 1
// rw
#define DMA_MM2S_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_MM2S_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_ENABLED 1
// rw
#define DMA_MM2S_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_MM2S_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_ENABLED 1
// rw
#define DMA_MM2S_DMACR_X_Err_IrqEn_LOC 14
#define DMA_MM2S_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_ENABLED 1


//	MM2S_DMASR - status register
// ro
#define DMA_MM2S_DMASR_X_Halted_LOC 0
#define DMA_MM2S_DMASR_X_Halted_SIZE 1
#define DMA_MM2S_DMASR_X_Halted_X_RUNNING 0
#define DMA_MM2S_DMASR_X_Halted_X_HALTED 1
// ro
#define DMA_MM2S_DMASR_X_Idle_LOC 1
#define DMA_MM2S_DMASR_X_Idle_SIZE 1
#define DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_MM2S_DMASR_X_Idle_X_IDLE 1
// ro
#define DMA_MM2S_DMASR_X_SGIncld_LOC 3
#define DMA_MM2S_DMASR_X_SGIncld_SIZE 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_not_EN 0
// ro
#define DMA_MM2S_DMASR_X_DMASlvErr_LOC 5
#define DMA_MM2S_DMASR_X_DMASlvErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
// ro
#define DMA_MM2S_DMASR_X_DMADecErr_LOC 6
#define DMA_MM2S_DMASR_X_DMADecErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMADecErr_X_ERR_DETECTED 1
// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_IOC_Irq_LOC 12
#define DMA_MM2S_DMASR_X_IOC_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_Err_Irq_LOC 14
#define DMA_MM2S_DMASR_X_Err_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


// MM2S_SA_LSB - source address
// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


// MM2S_SA_MSB - source address
// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


// MM2S_LENGTH
// R/W
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0


// S2MM
//	S2MM_DMACR - control register
// rw
#define DMA_S2MM_DMACR_X_RS_LOC 0
#define DMA_S2MM_DMACR_X_RS_SIZE 1
#define DMA_S2MM_DMACR_X_RS_X_STOP 0
#define DMA_S2MM_DMACR_X_RS_X_RUN 1
// rw
#define DMA_S2MM_DMACR_X_RESET_LOC 2
#define DMA_S2MM_DMACR_X_RESET_SIZE 1
#define DMA_S2MM_DMACR_X_RESET_X_PAUSED 0
#define DMA_S2MM_DMACR_X_RESET_X_START 1
// rw
#define DMA_S2MM_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_S2MM_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_ENABLED 1
// rw
#define DMA_S2MM_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_S2MM_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_ENABLED 1
// rw
#define DMA_S2MM_DMACR_X_Err_IrqEn_LOC 14
#define DMA_S2MM_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_ENABLED 1


//	S2MM_DMASR - status register
// ro
#define DMA_S2MM_DMASR_X_Halted_LOC 0
#define DMA_S2MM_DMASR_X_Halted_SIZE 1
#define DMA_S2MM_DMASR_X_Halted_X_RUNNING 0
#define DMA_S2MM_DMASR_X_Halted_X_HALTED 1
// ro
#define DMA_S2MM_DMASR_X_Idle_LOC 1
#define DMA_S2MM_DMASR_X_Idle_SIZE 1
#define DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_S2MM_DMASR_X_Idle_X_IDLE 1
// ro
#define DMA_S2MM_DMASR_X_SGIncld_LOC 3
#define DMA_S2MM_DMASR_X_SGIncld_SIZE 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_not_EN 0
// ro
#define DMA_S2MM_DMASR_X_DMASlvErr_LOC 5
#define DMA_S2MM_DMASR_X_DMASlvErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
// ro
#define DMA_S2MM_DMASR_X_DMADecErr_LOC 6
#define DMA_S2MM_DMASR_X_DMADecErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMADecErr_X_ERR_DETECTED 1
// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_IOC_Irq_LOC 12
#define DMA_S2MM_DMASR_X_IOC_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_Err_Irq_LOC 14
#define DMA_S2MM_DMASR_X_Err_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


// S2MM_SA_LSB - source address
// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


// S2MM_SA_MSB - source address
// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


// S2MM_LENGTH
// R/W
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0

//-------------------------------------------------------------------------------------
class FpgaTarget : public Target {
    uint32_t *XPU_POINTER_CONSTANT;
    uint32_t *DMA_POINTER_CONSTANT;
    uint32_t XPU_CALL_ADDRESS_RESULT_READY;
    uint32_t XPU_CALL_ADDRESS_WAIT_MATRIX;
    //	uint32_t* DMA_POINTER_CONSTANT;
    uint32_t *data_in_ptr;
    uint32_t *data_out_ptr;
    int32_t memory_file_descriptor;
    
    static void AXI_LITE_write(uint32_t *_address, uint32_t value);

    static uint32_t AXI_LITE_read(const uint32_t *_address);

    static void dma_mm2s_status(uint32_t *DMA_POINTER_CONSTANT);

    static void dma_s2mm_status(uint32_t *DMA_POINTER_CONSTANT);

    static void print_all_registers_mm2s(uint32_t *DMA_POINTER_CONSTANT, int tag);

    static void print_all_registers_s2mm(uint32_t *DMA_POINTER_CONSTANT, int tag);

    static void dma_mm2s_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT);

    static void dma_s2mm_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT);

    static void DMA_XPU_read(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length);

    static void DMA_XPU_write(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length);

    static void dma_reset(uint32_t *DMA_POINTER_CONSTANT);

    static void print_main_mem(uint32_t *address, int32_t nr_bytes, uint32_t word_size);
    //	void print_all_registers_mm2s(uint32_t* DMA_POINTER_CONSTANT, int tag);
    //	void print_all_registers_s2mm(uint32_t* DMA_POINTER_CONSTANT, int tag);

    //	void loadCode(uint32_t _address, uint32_t* _code, uint32_t _length);
    //	void loadData(uint32_t _address, uint32_t* _data, uint32_t _length);

    inline void writeInstruction(uint32_t _instruction);
    inline void writeInstruction(uint8_t _instructionByte, uint32_t _argument);

public:
    FpgaTarget();

    ~FpgaTarget() override;

    void reset() override;

    void runRuntime(uint32_t _address, uint32_t *_args) override;

    void runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) override;

    void readRegister(uint32_t _address, uint32_t _register) override;

    void writeRegister(uint32_t _address, uint32_t _register) override;

    void writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) override;

    void readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) override;

    void writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
            uint32_t _columnStart, uint32_t _columnStop) override;

    void readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop) override;

    void writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop, uint32_t _columnStart,
            uint32_t _columnStop) override;

    void dump(const std::string &_address) override;

    // TESTS
    static void XPU_write_program_file_1(uint32_t *addr); // data in ; ixload+ data in ; data out; addr regs: 0-100
    static void XPU_write_program_file_2(uint32_t *addr); // data in ; no compute ; data out; addr regs: 0-0
    static void XPU_write_program_file_3(uint32_t *addr); // data in ; no compute ; data out; addr regs: 0-0
    void test_basic();
    void test_write_data();
    void test_write_and_run_code(uint32_t _address, uint32_t *_code, uint32_t _length);

};

//-------------------------------------------------------------------------------------






