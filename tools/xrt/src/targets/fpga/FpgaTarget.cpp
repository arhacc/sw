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
FpgaTarget::FpgaTarget() {
	printf("Starting FpgaTarget...\n");
//    uint32_t* XPU_POINTER_CONSTANT;
//    uint64_t delay;
    unsigned int xpu_status_reg;



//////////////////// code placed here for testing purposes.


	int32_t memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);

//	long _pagesize = sysconf(_SC_PAGESIZE);
//	std::cout << "_pagesize=[" << _pagesize << "]" << std::endl;

	XPU_POINTER_CONSTANT = (uint32_t*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, memory_file_descriptor, XPU_BASE_ADDR );
	DMA_POINTER_CONSTANT = (uint32_t*)mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, DMA_BASE_ADDR);
	data_in_ptr  = (uint32_t*)mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x19000000);
	data_out_ptr = (uint32_t*)mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x1A000000);

    dma_reset(DMA_POINTER_CONSTANT);

    xpu_status_reg = *((volatile unsigned *)(XPU_POINTER_CONSTANT + XPU_STATUS_REG_ADDR_OFFSET));	// write program file
	printf("before loading program file : %x\n", xpu_status_reg);
    XPU_CALL_ADDRESS_RESULT_READY = 0x67000004;
	XPU_CALL_ADDRESS_WAIT_MATRIX = 0x67000006;

//	readArrayData(0, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){

//    loadCode(libraryCodeMain);

/*
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
	xpu_status_reg = AXI_LITE_read(XPU_POINTER_CONSTANT + XPU_STATUS_REG_ADDR_OFFSET);
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


	for(int i = 0; i < test_dimension; i++){
		_data_in_ptr1[i] = (i*3) << 6;
	}
	
	
	writeArrayData(0x19100000, 0, 400 , 401, 0, test_dimension);	
	readArrayData(0x1A100000, 0, 400 , 401, 0, test_dimension);
	

    print_main_mem(_data_out_ptr1, test_dimension* sizeof(uint32_t), sizeof(uint32_t));
    printf("\n");
	
	printf("End second part\n");	
	printf("All test programs done\n\n");
*/
}

//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() {
// unmap memory regions
    munmap(DMA_POINTER_CONSTANT, 65535);
    munmap(XPU_POINTER_CONSTANT,4096);
    munmap(data_in_ptr, NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t));
}


//-------------------------------------------------------------------------------------
void FpgaTarget::reset(){
}

//-------------------------------------------------------------------------------------
void FpgaTarget::runRuntime(uint32_t _address, uint32_t* _args) {
}

//-------------------------------------------------------------------------------------
void FpgaTarget::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress){

}

//-------------------------------------------------------------------------------------
void FpgaTarget::readRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
	printf("FpgaTarget.loadCode @%d, length=%d\n", _address, _length);
//    printf("AXI xpu write program memory...\n");
	uint32_t* _addr = XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET;
	AXI_LITE_write(_addr, 0x6f000000); //pload
	for(int i = 0; i < _length; i++){
		AXI_LITE_write(_addr, _code[i]);
	}
	AXI_LITE_write(_addr, 0x67000000); //prun
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
	// unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
	// unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readArrayData(uint32_t _dataAddressDDR, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
    
	uint32_t _transferLength = (_lineStop - _lineStart ) * ( _columnStop - _columnStart );
    
	AXI_LITE_write(XPU_POINTER_CONSTANT, XPU_CALL_ADDRESS_RESULT_READY);//XPU_CALL_ADDRESS_RESULT_READY);    // apel primitiva de result ready            
	AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);

	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x7f000001); // get_matrix w/ result ready; //array_mem->fifo_out  
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _lineStart);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _lineStop - _lineStart);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, _columnStop - _columnStart);
	AXI_LITE_write(XPU_POINTER_CONSTANT + XPU_FIFO_PROGRAM_ADDR_OFFSET, 0x10000000);

	// get data out; xpu -> dma -> ddr
	printf("xpu->dma: start load data out \n");
	DMA_XPU_write(DMA_POINTER_CONSTANT, _dataAddressDDR , _transferLength * sizeof(uint32_t) );
	printf("xpu->dma: end load data out \n");    
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeArrayData(uint32_t _dataAddressDDR, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
    	
    uint32_t _transferLength = (_lineStop - _lineStart ) * ( _columnStop - _columnStart );
    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x77000000); // SEND_MATRIX_ARRAY; data_in_fifo->array_cell_mem      
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _lineStart);                       // de la ce linie    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _lineStop - _lineStart);           // cate linii    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, _columnStop - _columnStart);       // cate coloane    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);

    AXI_LITE_write(XPU_POINTER_CONSTANT, XPU_CALL_ADDRESS_WAIT_MATRIX );       //apel primitiva de wait matrices        
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x10000000);
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x00000001);       //argument pt primitiva - cate matrici sa astepte, valoarea 1    
    AXI_LITE_write(XPU_POINTER_CONSTANT, 0x00000000);
	
    DMA_XPU_read(DMA_POINTER_CONSTANT, _dataAddressDDR, _transferLength * sizeof(uint32_t) ); 
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dump(std::string _addressString){
	unsigned int _address = std::stoul(_addressString, nullptr, 16);
	printf("FpgaTarget.dump @%x:\n", _address);

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void FpgaTarget::AXI_LITE_write(uint32_t* addr, uint32_t _value)
{
//	std::cout << "AXI_LITE_write: " << _value << std::endl;
    printf("AXI_LITE_write: 0x%08x\n", _value);	
	*((volatile unsigned *)(addr)) = _value;
	return;
}

//-------------------------------------------------------------------------------------
uint32_t FpgaTarget::AXI_LITE_read(uint32_t* addr)
{
	uint32_t return_value;
	return_value = *((volatile unsigned *)(addr));
	return return_value;
}

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_1(uint32_t* addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
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

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_2(uint32_t* addr) // data in ; no compute ; data out; addr regs: 0-0
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

//-------------------------------------------------------------------------------------
void FpgaTarget::XPU_write_program_file_3(uint32_t* addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
{	// extracted data out (GET_MATRIX to be used independently)

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
void FpgaTarget::dma_mm2s_status(uint32_t* DMA_POINTER_CONSTANT)
{
	uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET>>2));
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

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_s2mm_status(uint32_t* DMA_POINTER_CONSTANT)
{
	uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET>>2));
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

//-------------------------------------------------------------------------------------
void FpgaTarget::print_all_registers_mm2s(uint32_t* DMA_POINTER_CONSTANT, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA mm2s registers: tag: %d \n", tag);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET>>2) );
	printf("DMA mm2s: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET>>2) );
	printf("DMA mm2s: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET>>2) );
	printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET>>2) );
	printf("DMA mm2s: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET>>2) );
	printf("DMA mm2s: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA mm2s registers: \n");
	return;
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_all_registers_s2mm(uint32_t* DMA_POINTER_CONSTANT, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA s2mm registers: tag: %d\n", tag);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET>>2) );
	printf("DMA s2mm: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET>>2) );
	printf("DMA s2mm: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET>>2) );
	printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET>>2) );
	printf("DMA s2mm: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET>>2) );
	printf("DMA s2mm: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA s2mm registers: \n");
	return;
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_mm2s_wait_transfers_complete(uint32_t* DMA_POINTER_CONSTANT)
{
	uint32_t mm2s_status =  AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET>>2) );

    while( ( mm2s_status & (1<<DMA_MM2S_DMASR_X_Idle_LOC) ) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_mm2s_status(DMA_POINTER_CONSTANT);
        mm2s_status =  AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET>>2) );
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_s2mm_wait_transfers_complete(uint32_t* DMA_POINTER_CONSTANT)
{
	uint32_t s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET>>2) );

    while( ( s2mm_status & (1<<DMA_S2MM_DMASR_X_Idle_LOC) ) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_s2mm_status(DMA_POINTER_CONSTANT);
        s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET>>2) );
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_XPU_read(uint32_t* DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length)
{
	printf("Start MM2S function\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET>>2), 0);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
	printf("Writing source address\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET>>2), 0x00000000);
	dma_mm2s_status(DMA_POINTER_CONSTANT);
	printf("Starting MM2S channel\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET>>2), 0x0001);
	dma_mm2s_status(DMA_POINTER_CONSTANT);
	printf("Writing MM2S transfer length\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET>>2), transfer_length);
	dma_mm2s_status(DMA_POINTER_CONSTANT);
	printf("Waiting for MM2S to be done\n");
	dma_mm2s_wait_transfers_complete(DMA_POINTER_CONSTANT);
	dma_mm2s_status(DMA_POINTER_CONSTANT);
	printf("End MM2S function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_XPU_write(uint32_t* DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length )
{
	printf("Start S2MM function\n");
	printf("Writing destination address\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET>>2), 0x00000000);
	dma_s2mm_status(DMA_POINTER_CONSTANT);
	printf("Starting S2MM channel\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET>>2), 0x0001);
	dma_s2mm_status(DMA_POINTER_CONSTANT);
	printf("Writing S2MM transfer length\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET>>2), transfer_length);
	dma_s2mm_status(DMA_POINTER_CONSTANT);
	printf("Waiting for S2MM to be done\n");
	dma_s2mm_wait_transfers_complete(DMA_POINTER_CONSTANT);
	dma_s2mm_status(DMA_POINTER_CONSTANT);
	printf("End S2MM function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_reset(uint32_t* DMA_POINTER_CONSTANT)
{
	printf("Resetting DMA\n");
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET>>2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
	AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET>>2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
	dma_mm2s_status(DMA_POINTER_CONSTANT);
	dma_s2mm_status(DMA_POINTER_CONSTANT);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_main_mem(uint32_t* address, int32_t nr_bytes, uint32_t word_size)
{
//    char *p = static_cast<char *>(address);
    char *p = (char *) address;

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

//-------------------------------------------------------------------------------------
