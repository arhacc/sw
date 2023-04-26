//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <targets/fpga/FpgaTarget.h>

//-------------------------------------------------------------------------------------
void FpgaTarget::test_basic()
{
        printf("xpu: start program_file_load \n");
        XPU_write_program_file_1(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET);
        printf("xpu: end program_file_load \n");

    // load data in; ddr->dma->xpu
        printf("dma->xpu: start load data in \n");
        DMA_XPU_read(DMA_POINTER_CONSTANT, 0x19000000, NR_TRANSACTIONS * sizeof(uint32_t) );
        printf("dma->xpu: end load data in\n");

    
    // interrupt ack
        AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_WRITE_INT_ACK_ADDR, 1);
        uint32_t delay;
        for (delay = 0; delay < TIME_DELAY; delay++)
        {
            ;
        }
        unsigned int xpu_status_reg = AXI_LITE_read(XPU_POINTER_CONSTANT + XPU_STATUS_REG_ADDR_OFFSET);
        printf("after interrupt ack: status reg: %x\n", xpu_status_reg);

    // get data out; xpu -> dma -> ddr
        printf("xpu->dma: start load data out \n");
        DMA_XPU_write(DMA_POINTER_CONSTANT, 0x1A000000, NR_TRANSACTIONS * sizeof(uint32_t) );
        printf("xpu->dma: end load data out\n");

    // print results
        printf("Destination memory block: ");
        print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));
        printf("\n");
        printf("End first part\n");

    // part 2 of test. just write and read
    const uint32_t test_dimension = 64;// currently working only for size 64. to be investigated.

    uint32_t* _data_in_ptr1  = (uint32_t*)mmap(NULL, test_dimension * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x19100000);
    uint32_t* _data_out_ptr1 = (uint32_t*)mmap(NULL, test_dimension * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x1A100000);


    for(uint32_t i = 0; i < test_dimension; i++){
        _data_in_ptr1[i] = (i*3) << 6;
    }


    writeArrayData(0x19100000, 0, 400 , 401, 0, test_dimension);
    readArrayData(0x1A100000, 0, 400 , 401, 0, test_dimension);


    print_main_mem(_data_out_ptr1, test_dimension* sizeof(uint32_t), sizeof(uint32_t));
    printf("\n");

    printf("End second part\n");
    printf("All test programs done\n\n");

}

void FpgaTarget::test_write_and_run_code(uint32_t _address, uint32_t *_code, uint32_t _length)
{
    writeCode(_address, _code, _length);

    // Tests
        printf("xpu: start program_file_load \n");
        XPU_write_program_file_1(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET);
        printf("xpu: end program_file_load \n");

    // load data in; ddr->dma->xpu
        printf("dma->xpu: start load data in \n");
        DMA_XPU_read(DMA_POINTER_CONSTANT, 0x19000000, NR_TRANSACTIONS * sizeof(uint32_t) );
        printf("dma->xpu: end load data in\n");

    
    // interrupt ack
        AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_WRITE_INT_ACK_ADDR, 1);
        uint32_t delay;
        for (delay = 0; delay < TIME_DELAY; delay++)
        {
            ;
        }
        unsigned int xpu_status_reg = AXI_LITE_read(XPU_POINTER_CONSTANT + XPU_STATUS_REG_ADDR_OFFSET);
        printf("after interrupt ack: status reg: %x\n", xpu_status_reg);

    // get data out; xpu -> dma -> ddr
        printf("xpu->dma: start load data out \n");
        DMA_XPU_write(DMA_POINTER_CONSTANT, 0x1A000000, NR_TRANSACTIONS * sizeof(uint32_t) );
        printf("xpu->dma: end load data out\n");

    // print results
        printf("Destination memory block: ");
        print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));
        printf("\n");
        printf("End first part\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_1(uint32_t *addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
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

    AXI_LITE_write(addr, 0x00000000);    // function arguments for set addr regs
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

    AXI_LITE_write(addr, 0x00000001);    // function arguments for wait mat
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
}

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_2(uint32_t *addr) // data in ; no compute ; data out; addr regs: 0-0
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
}

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_3(uint32_t *addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
{    // extracted data out (GET_MATRIX to be used independently)

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

    AXI_LITE_write(addr, 0x00000001);
    AXI_LITE_write(addr, 0x00000000);

    AXI_LITE_write(addr, 0x6700000c);
    AXI_LITE_write(addr, 0x10000000);


    AXI_LITE_write(addr, 0x67000005);
    AXI_LITE_write(addr, 0x10000000);
}

//-------------------------------------------------------------------------------------
