

/* start : prim_reverse_array_mem_data
	name:
		prim_reverse_array_mem_data
	info:
		set addr reg for both controller and array.
	requirements hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (5)
	requirements variables/parameters locations:
		CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC
		CODE_PRIM_REVERSE_ARRAY_MEM_DATA_SIZE_MEM_LOC
		
	uses:
	    label 10_212 - function start;
		label 10_213 - main loop;
//*/ 
//*	// code
	`define CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC (`CONTROLLER_INSTR_MEM_SIZE/2 - 1)
	`define CODE_PRIM_REVERSE_ARRAY_MEM_DATA_SIZE_MEM_LOC (`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC - 1)
	
	`define CODE_PRIM_REVERSE_ARRAY_MEM_DATA_CTRL_RAM_MEM_END_LOC (`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_SIZE_MEM_LOC - 1)

LB(10212);
	cVSTACK_PUSH_LOAD(-1);															NOP;
	cSETDEC;																		NOP;
	cPARAM;																			NOP;
	cRSTORE(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC);			NOP;
	cPARAM;																			NOP;
	cRSTORE(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_SIZE_MEM_LOC);						NOP;
	cRADD(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC);				NOP;
	cVSUB(1);																		NOP;
	cRSTORE(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_CTRL_RAM_MEM_END_LOC);				NOP;
	cRLOAD(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_SIZE_MEM_LOC);							NOP;
	cSHRIGHT;																		NOP;
	cVSUB(1);																		NOP;
	cSETCompVal;																	NOP;
	cVLOAD(0); 																		NOP;
	LB(10213);
		cSTACK_DUPLICATE;																NOP;
		cSTACK_DUPLICATE;																NOP;
		cRADD(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_START_ADDR_offset_MEM_LOC);				NOP;
		cSTACK_SWAP;																	CRSWAP_ACC_MEM;
		cRRVSUB(`CODE_PRIM_REVERSE_ARRAY_MEM_DATA_CTRL_RAM_MEM_END_LOC);				NOP;
		cSTACK_POP;																		CRSWAP_ACC_MEM;
		cVADD(0);																		NOP; // needed to propaget acc through dsp
		cSTACK_POP;																		CRSWAP_ACC_MEM;
		cBRNValDEC(10213);																NOP;
	
	cHALT;																	NOP;
//*/ //end : prim_reverse_array_mem_data




























