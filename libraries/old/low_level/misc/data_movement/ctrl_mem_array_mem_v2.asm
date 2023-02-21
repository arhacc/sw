/* start : prim_copy_ctrl_mem_array_mem_v2
	name:
		prim_copy_ctrl_mem_array_mem_v2
	info:
		copy data from controller memory location to array acc. One data is copied to all accumulators
	requiremens hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
	requirements variables/parameters locations:
		param: mem addr from where to start reading data, stored at: CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_MEM_LOCATION
		
	uses:
	    label 10_208 - function start;
		label 10_209 - global data shift register load loop.
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_READ_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_WRITE_MEM_LOCATION (`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_READ_MEM_LOCATION -1)
		
LB(10208);
	cADDRLOAD;																NOP;
	cSTACK_DUPLICATE;														NOP;
	cSTACK_DUPLICATE;														NOP;
	cPARAM;																	NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_READ_MEM_LOCATION);		NOP;
	cSADD;																	NOP;
	cVSUB(1);																NOP;
	cADDRSTORE;																NOP;
	cVLOAD(-1);																NOP;
	cSETDEC;																NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);												NOP;
	cSETCompVal;															NOP;
	cVLOAD(0);																NOP;
	
	LB(10209);
		cRISTACK_PUSH_LOAD(1);													NOP;
		cCLEFTINS;																NOP;
		cSTACK_POP;																NOP;
		cBRNValDEC(10209);														NOP;
	
	cPARAM;																	SRLOAD;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V2_WRITE_MEM_LOCATION);		CRSTORE;
	cSTACK_POP;																NOP;
	cADDRSTORE;																NOP;
	
	cHALT;																	NOP;
//*/ end : prim_copy_ctrl_mem_array_mem_v2
