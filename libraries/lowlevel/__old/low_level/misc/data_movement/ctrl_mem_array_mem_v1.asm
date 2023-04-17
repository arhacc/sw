
/* start : prim_copy_ctrl_mem_array_mem_v1
	name:
		prim_copy_ctrl_mem_array_mem_v1
	info:
		copy data from controller memory location to array acc. One data is copied to all accumulators
	requiremens hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
	requirements variables/parameters locations:
		param: mem addr from where to read data (controller), stored at: CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_READ_MEM_LOCATION
		param: mem addr where to store data (array), stored at: CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_WRITE_MEM_LOCATION
	uses:
	    label 10_207 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_READ_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_WRITE_MEM_LOCATION (`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_READ_MEM_LOCATION -1)
	
LB(10207);
	cADDRLOAD;															NOP;
	cSTACK_DUPLICATE;													NOP;
	cSTACK_DUPLICATE;													NOP;
	cPARAM;																NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_READ_MEM_LOCATION);	NOP;
	cSADD;																NOP;
	cADDRSTORE;															NOP;
	cSTACK_POP;															NOP;
	cRSTACK_PUSH_LOAD(0);												NOP;
	cSTACK_POP;															NOP;
	cADDRSTORE;															CLOAD;
	cPARAM;																NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_MEM_V1_WRITE_MEM_LOCATION);	CRSTORE;
	cNOP;																NOP;
	
	cHALT;																NOP;
//*/ end : prim_copy_ctrl_mem_array_mem_v1






























