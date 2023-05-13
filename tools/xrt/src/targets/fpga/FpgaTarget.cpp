//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <targets/fpga/FpgaTarget.h>
#include <targets/common/CodeGen.h>

//#ifndef XRT_NO_FPGA_TARGET

//static_assert(sizeof(uint32_t) == sizeof(size_t), "Not running on pynq board");

//-------------------------------------------------------------------------------------
FpgaTarget::FpgaTarget() {
    printf("Starting FpgaTarget...\n");
    //    uint32_t* XPU_POINTER_CONSTANT;
    //    uint64_t delay;
    unsigned int xpu_status_reg;

    //////////////////// code placed here for testing purposes.

    memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);

    //	long _pagesize = sysconf(_SC_PAGESIZE);
    //	std::cout << "_pagesize=[" << _pagesize << "]" << std::endl;

    XPU_POINTER_CONSTANT = (uint32_t *) mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor,
            XPU_BASE_ADDR);
    DMA_POINTER_CONSTANT = (uint32_t *) mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor,
            DMA_BASE_ADDR);
    data_in_ptr = (uint32_t *) mmap(nullptr, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED,
            memory_file_descriptor, 0x19000000);
    data_out_ptr = (uint32_t *) mmap(nullptr, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED,
            memory_file_descriptor, 0x1A000000);
    

    dma_reset(DMA_POINTER_CONSTANT);

    xpu_status_reg = AXI_LITE_read(XPU_POINTER_CONSTANT + XPU_STATUS_REG_ADDR_OFFSET);    // write program file
    printf("before loading program file : %x\n", xpu_status_reg);
    XPU_CALL_ADDRESS_RESULT_READY = 0x67000004;
    XPU_CALL_ADDRESS_WAIT_MATRIX = 0x67000006;

    //	readArrayData(0, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){

    //    loadCode(libraryCodeMain);

   
}

//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() {
    // unmap memory regions
    munmap(DMA_POINTER_CONSTANT, 65535);
    munmap(XPU_POINTER_CONSTANT, 4096);
    munmap(data_in_ptr, NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t));
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeInstruction(uint32_t _instruction)
{
    AXI_LITE_write(XPU_POINTER_CONSTANT, _instruction);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument)
{
    writeInstruction(makeInstruction(_instructionByte, _argument));
}


//-------------------------------------------------------------------------------------
void FpgaTarget::reset() {
}

//-------------------------------------------------------------------------------------
void FpgaTarget::runRuntime(uint32_t _address, uint32_t *_args) {
}

//-------------------------------------------------------------------------------------
void FpgaTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
void FpgaTarget::readRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("FpgaTarget.loadCode @%d, length=%d\n", _address, _length);
    //    printf("AXI xpu write program memory...\n");
    uint32_t *_addr = XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET;

    // pload and prun are currently part of example code

    //AXI_LITE_write(_addr, 0x6f000000); //pload
    for (uint32_t i = 0; i < _length; i++) {
        AXI_LITE_write(_addr, _code[i]);
    }
    //AXI_LITE_write(_addr, 0x67000000); //prun

    test_write_data();
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    // unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    // unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readArrayData(uint32_t _accAddress, uint32_t *_memAddress, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

    uint32_t _transferLength = (_lineStop - _lineStart) * (_columnStop - _columnStart);

    printf("Writing array data from %p at addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_memAddress), _accAddress, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(INSTR_get_array_matrix_wo_result_ready);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(INSTR_nop);
    
    DMA_XPU_read(DMA_POINTER_CONSTANT, reinterpret_cast<ptrdiff_t>(_memAddress), _transferLength * sizeof(uint32_t));
#if 0

    uint32_t _transferLength = (_lineStop - _lineStart) * (_columnStop - _columnStart);

    AXI_LITE_write(XPU_POINTER_CONSTANT,
            XPU_CALL_ADDRESS_RESULT_READY);//XPU_CALL_ADDRESS_RESULT_READY);    // apel primitiva de result ready
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);

    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET,
            0x7f000001); // get_matrix w/ result ready; //array_mem->fifo_out
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _lineStart);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _lineStop - _lineStart);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _columnStop - _columnStart);
    AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);

    // get data out; xpu -> dma -> ddr
    printf("xpu->dma: start load data out \n");
    DMA_XPU_write(DMA_POINTER_CONSTANT, _dataAddressDDR, _transferLength * sizeof(uint32_t));
    printf("xpu->dma: end load data out \n");
#endif
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeArrayData(uint32_t _accAddress, uint32_t *_memAddress, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

    uint32_t _transferLength = (_lineStop - _lineStart) * (_columnStop - _columnStart);

    printf("Writing array data from %p at addr=0x%08" PRIx32 " lineStart= %" PRIx32 " lineStop = %" PRIx32
           " columnStart = %" PRIx32 " columnStop = %" PRIx32 "\n", static_cast<void *>(_memAddress), _accAddress, _lineStart, _lineStop, _columnStart, _columnStop);

    writeInstruction(INSTR_send_array_matrix_header);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(INSTR_nop);
    writeInstruction(0, _lineStop - _lineStart);
    writeInstruction(INSTR_nop);
    writeInstruction(_columnStop - _columnStart);
    writeInstruction(INSTR_nop);

    DMA_XPU_read(DMA_POINTER_CONSTANT, reinterpret_cast<ptrdiff_t>(_memAddress) , _transferLength * sizeof(uint32_t));

#if 0
    uint32_t _transferLength = (_lineStop - _lineStart) * (_columnStop - _columnStart);

    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x77000000); // SEND_MATRIX_ARRAY; data_in_fifo->array_cell_mem      
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _lineStart);                       // de la ce linie    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _lineStop - _lineStart);           // cate linii    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _columnStop - _columnStart);       // cate coloane    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);

    AXI_LITE_write(XPU_POINTER_CONSTANT, XPU_CALL_ADDRESS_WAIT_MATRIX);       //apel primitiva de wait matrices
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT,
            0x00000001);       //argument pt primitiva - cate matrici sa astepte, valoarea 1
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x00000000);

    DMA_XPU_read(DMA_POINTER_CONSTANT, _dataAddressDDR, _transferLength * sizeof(uint32_t));
#endif
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dump(const std::string &_addressString) {
    unsigned int _address = std::stoul(_addressString, nullptr, 16);
    printf("FpgaTarget.dump @%x:\n", _address);

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void FpgaTarget::AXI_LITE_write(uint32_t *addr, uint32_t _value) {
    //	std::cout << "AXI_LITE_write: " << _value << std::endl;
    printf("AXI_LITE_write: 0x%08x\n", _value);
    *((volatile unsigned *) (addr)) = _value;
}

//-------------------------------------------------------------------------------------
uint32_t FpgaTarget::AXI_LITE_read(const uint32_t *addr) {
    uint32_t return_value = 0;
    return_value = *((volatile unsigned *) (addr));
    return return_value;
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_mm2s_status(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));
    printf("MM2S status (addr offset: 0x%x status:0x%x): ", DMA_MM2S_DMASR_OFFSET, status_reg);

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
void FpgaTarget::dma_s2mm_status(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));
    printf("S2MM status (addr offset: 0x%x status: 0x%x): ", DMA_S2MM_DMASR_OFFSET, status_reg);

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
void FpgaTarget::print_all_registers_mm2s(uint32_t *DMA_POINTER_CONSTANT, int tag) {
    uint32_t register_read_value;

    printf("Printing all DMA mm2s registers: tag: %d \n", tag);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2));
    printf("DMA mm2s: control register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));
    printf("DMA mm2s: status register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET >> 2));
    printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET >> 2));
    printf("DMA mm2s: source addr msb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET >> 2));
    printf("DMA mm2s: transfer length: %x\n", register_read_value);

    printf("Finished printing all DMA mm2s registers: \n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_all_registers_s2mm(uint32_t *DMA_POINTER_CONSTANT, int tag) {
    uint32_t register_read_value;

    printf("Printing all DMA s2mm registers: tag: %d\n", tag);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2));
    printf("DMA s2mm: control register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));
    printf("DMA s2mm: status register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET >> 2));
    printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET >> 2));
    printf("DMA s2mm: source addr msb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET >> 2));
    printf("DMA s2mm: transfer length: %x\n", register_read_value);

    printf("Finished printing all DMA s2mm registers: \n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_mm2s_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t mm2s_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));

    while ((mm2s_status & (1 << DMA_MM2S_DMASR_X_Idle_LOC)) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE) {
        dma_mm2s_status(DMA_POINTER_CONSTANT);
        mm2s_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_s2mm_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));

    while ((s2mm_status & (1 << DMA_S2MM_DMASR_X_Idle_LOC)) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE) {
        dma_s2mm_status(DMA_POINTER_CONSTANT);
        s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_XPU_read(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length) {
    printf("Start MM2S function\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 0);
    dma_mm2s_status(DMA_POINTER_CONSTANT);

    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2), 0);
    dma_mm2s_status(DMA_POINTER_CONSTANT);

    printf("Writing source address\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET >> 2), ddr_start_addr);
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET >> 2), 0x00000000);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Starting MM2S channel\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 0x0001);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Writing MM2S transfer length\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET >> 2), transfer_length);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Waiting for MM2S to be done\n");
    dma_mm2s_wait_transfers_complete(DMA_POINTER_CONSTANT);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("End MM2S function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_XPU_write(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length) {
    printf("Start S2MM function\n");
    printf("Writing destination address\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET >> 2), ddr_start_addr);
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET >> 2), 0x00000000);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Starting S2MM channel\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2), 0x0001);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Writing S2MM transfer length\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET >> 2), transfer_length);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Waiting for S2MM to be done\n");
    dma_s2mm_wait_transfers_complete(DMA_POINTER_CONSTANT);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("End S2MM function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_reset(uint32_t *DMA_POINTER_CONSTANT) {
    printf("Resetting DMA\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_main_mem(uint32_t *address, int32_t nr_bytes, uint32_t word_size) {
    //    char *p = static_cast<char *>(address);
    char *p = (char *) address;

    for (int i = 0; i < nr_bytes; i++) {
        if ((i + 1) % word_size == 0) {
            for (uint32_t j = 0; j < word_size; j++) {
                printf("%02x", p[i - j]);
            }
            printf(" ");
        }

        if ((i + 1) % (word_size * 8) == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

//#endif
