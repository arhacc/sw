/* start : prim_copy_ctrl_mem_array_acc_v2
	name:
		prim_copy_ctrl_mem_array_acc_v2
	info:
		copy data from controller memory locations to array accs. Each data from each controller memory location reaches one array accumulator.
	requiremens hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
	requirements variables/parameters locations:
		param: mem addr from where to start reading data, stored at: CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V2_MEM_LOCATION
	uses:
	    label 10_205 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V2_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	
LB(10205);
	cADDRLOAD;														NOP;
	cSTACK_DUPLICATE;												NOP;
	cSTACK_DUPLICATE;												NOP;
	cPARAM;															NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V2_MEM_LOCATION);	NOP;
	cSADD;															NOP;
	cVSUB(1);														NOP;
	cADDRSTORE;														NOP;
	cVLOAD(-1);														NOP;
	cSETDEC;														NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);										NOP;
	cSETCompVal;													NOP;
	cVLOAD(0);														NOP;
	
	LB(10206);
		cRISTACK_PUSH_LOAD(1);											NOP;
		cCLEFTINS;														NOP;
		cSTACK_POP;														NOP;
		cBRNValDEC(10206);												NOP;
	
	cSTACK_POP;														SRLOAD;
	cADDRSTORE;														NOP;
	
	cHALT;																	NOP;
//*/ end : prim_copy_ctrl_mem_array_acc_v1





























