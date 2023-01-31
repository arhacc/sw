/*
	example of basic program for the xpu system.
		write program file into xpu
		get data in into the xpu system using dma mechanism
		do computations (done automatically)
		get data out from the xpu system using dma mechanism
 */





#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>





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







void AXI_LITE_write(void * addr, uint32_t value)
{
	*((volatile unsigned *)(addr)) = value;
	return;
}



uint32_t AXI_LITE_read(void * addr)
{
	uint32_t return_value;
	return_value = *((volatile unsigned *)(addr));
	return return_value;
}



void XPU_write_program_file_1(void * addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
{
	// algorithm:
		// data_out_from_dma = ixload + data_in_from_dma
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000000);	// function arguments for set addr regs
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000001);	// function arguments for wait mat
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x6700000c);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}



void XPU_write_program_file_2(void * addr) // data in ; no compute ; data out; addr regs: 0-0
{
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x67000004);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}



void dma_mm2s_status(uint32_t * dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2));
    printf("MM2S status (addr offset: 0x%x status:0x%x): ", DMA_MM2S_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
    {
    	printf("halted ");
    }
    else
    {
    	printf("running ");
    }

    if (status_reg & 0x00000002)
    {
    	printf("idle ");
    }
    if (status_reg & 0x00000008)
    {
    	printf("SG enabled ");
    }
    if (status_reg & 0x00000010)
    {
    	printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020)
    {
    	printf("DMA slave error ");
    }
    if (status_reg & 0x00000040)
    {
    	printf("DMA decode error ");
    }
    if (status_reg & 0x00001000)
    {
    	printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000)
    {
    	printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000)
    {
    	printf("Error interrupt request ");
    }

    printf("\n");
}



void dma_s2mm_status(uint32_t * dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2));
    printf("S2MM status (addr offset: 0x%x status: 0x%x): ", DMA_S2MM_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
	{
		printf("halted ");
	}
	else
	{
		printf("running ");
	}

	if (status_reg & 0x00000002)
	{
		printf("idle ");
	}
	if (status_reg & 0x00000008)
	{
		printf("SG enabled ");
	}
	if (status_reg & 0x00000010)
	{
		printf("DMA interrupt error ");
	}
	if (status_reg & 0x00000020)
	{
		printf("DMA slave error ");
	}
	if (status_reg & 0x00000040)
	{
		printf("DMA decode error ");
	}
	if (status_reg & 0x00001000)
	{
		printf("IOC interrupt request ");
	}
	if (status_reg & 0x00002000)
	{
		printf("Delay interrupt request ");
	}
	if (status_reg & 0x00004000)
	{
		printf("Error interrupt request ");
	}

    printf("\n");
}



void print_all_registers_mm2s(void * dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA mm2s registers: tag: %d \n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2) );
	printf("DMA mm2s: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
	printf("DMA mm2s: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2) );
	printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2) );
	printf("DMA mm2s: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2) );
	printf("DMA mm2s: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA mm2s registers: \n");
	return;
}



void print_all_registers_s2mm(void * dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA s2mm registers: tag: %d\n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2) );
	printf("DMA s2mm: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
	printf("DMA s2mm: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2) );
	printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2) );
	printf("DMA s2mm: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2) );
	printf("DMA s2mm: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA s2mm registers: \n");
	return;
}



void dma_mm2s_wait_transfers_complete(uint32_t * dma_ptr)
{
	uint32_t mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );

    while( ( mm2s_status & (1<<DMA_MM2S_DMASR_X_Idle_LOC) ) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_mm2s_status(dma_ptr);
        mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
    }
}



void dma_s2mm_wait_transfers_complete(uint32_t * dma_ptr)
{
	uint32_t s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );

    while( ( s2mm_status & (1<<DMA_S2MM_DMASR_X_Idle_LOC) ) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_s2mm_status(dma_ptr);
        s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
    }
}



void DMA_XPU_read(uint32_t * dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length)
{

	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0);
    dma_mm2s_status(dma_ptr);
	printf("Writing source address\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2), 0x00000000);
	dma_mm2s_status(dma_ptr);
	printf("Starting MM2S channel\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0x0001);
	dma_mm2s_status(dma_ptr);
	printf("Writing MM2S transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2), transfer_length);
	dma_mm2s_status(dma_ptr);
	printf("Waiting for MM2S to be done\n");
	dma_mm2s_wait_transfers_complete(dma_ptr);
	dma_mm2s_status(dma_ptr);
}



void DMA_XPU_write(uint32_t * dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length )
{
	printf("Writing destination address\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2), 0x00000000);
	dma_s2mm_status(dma_ptr);
	printf("Starting S2MM channel\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 0x0001);
	dma_s2mm_status(dma_ptr);
	printf("Writing S2MM transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2), transfer_length);
	dma_s2mm_status(dma_ptr);
	printf("Waiting for S2MM to be done\n");
	dma_s2mm_wait_transfers_complete(dma_ptr);
	dma_s2mm_status(dma_ptr);
}



void dma_reset(uint32_t * dma_ptr)
{
	printf("Resetting DMA\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
	dma_mm2s_status(dma_ptr);
	dma_s2mm_status(dma_ptr);
}



void print_main_mem(void* address, int32_t nr_bytes, uint32_t word_size)
{
    char *p = address;

    for (int i = 0; i < nr_bytes; i++)
    {
        if ( (i+1) % word_size == 0)
        {
        	for(int j = 0; j < word_size ; j++)
        	{
        		printf("%02x", p[i-j]);
        	}
        	printf(" ");
        }

        if( (i+1) % (word_size *8) == 0)
		{
			printf("\n");
		}
    }

    printf("\n");
}





int main()
{
    void *xpu_ptr;
    uint64_t delay;
	unsigned int xpu_status_reg = 0x0;


	int32_t memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);


	xpu_ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, memory_file_descriptor, XPU_BASE_ADDR );
	uint32_t * dma_ptr = mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, DMA_BASE_ADDR);
	uint32_t * data_in_ptr  = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x19000000);
	uint32_t * data_out_ptr = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x1A000000);



    for(int i = 0 ; i < NR_TRANSACTIONS; i++ )					// data in generation
    {
    	data_in_ptr[i]= 50;
    }

    memset(data_out_ptr, 0, NR_TRANSACTIONS * sizeof(uint32_t) ); // Clear destination block

    printf("AXI xpu write program + run + dma test.\n");

    printf("Source memory block:      ");
    print_main_mem(data_in_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));

    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));



    dma_reset(dma_ptr);

    xpu_status_reg = *((volatile unsigned *)(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET));	// write program file
	printf("before loading program file : %x\n", xpu_status_reg);
	printf("xpu: start program_file_load \n");
	XPU_write_program_file_1(xpu_ptr + XPU_FIFO_PROGRAM_ADDR_OFFSET);
	printf("xpu: end program_file_load \n");

    																					// load data in; ddr->dma->xpu
	printf("dma->xpu: start load data in \n");
	DMA_XPU_read(dma_ptr, 0x19000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("dma->xpu: end load data in\n");

																						// interrupt ack
	AXI_LITE_write(xpu_ptr + XPU_WRITE_INT_ACK_ADDR,1);
	for (delay = 0; delay < TIME_DELAY; delay++)
	{
		;
	}
	xpu_status_reg = AXI_LITE_read(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET);
	printf("after interrupt ack: status reg: %x\n", xpu_status_reg);

																						// get data out; xpu -> dma -> ddr
	printf("xpu->dma: start load data out \n");
	DMA_XPU_write(dma_ptr, 0x1A000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("xpu->dma: end load data out\n");

																						// print results
    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));
    printf("\n");

    																					// unmap memory regions
    munmap(dma_ptr,65535);
    munmap(data_in_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(data_out_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(xpu_ptr,4096);



    return 0;
}






