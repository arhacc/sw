
/* start : prim_copy_ctrl_mem_ctrl_acc
	name:
		prim_copy_ctrl_mem_ctrl_acc
	info:
		copy data from a specific location in controller memory to controller acc
	requiremens hardware:
		min RESOURCE_CTRL_CELL_STACK_SIZE (4)
	requirements variables/parameters locations:
		param: mem addr at which to save acc data, stored at: CODE_PRIM_COPY_CTRL_MEM_CTRL_ACC_MEM_LOCATION
	uses:
	    label 10_201 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_CTRL_ACC_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	
LB(10201);
	cPARAM;															NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_CTRL_ACC_MEM_LOCATION);		NOP;
	cADDRLOAD;														NOP;
	cSTACK_DUPLICATE;												NOP;
	cRADD(`CODE_PRIM_COPY_CTRL_MEM_CTRL_ACC_MEM_LOCATION);			NOP;
	cADDRSTORE;														NOP;
	cSTACK_POP;														NOP;
	cRSTACK_PUSH_LOAD(0);											NOP;
	cSTACK_SWAP;													NOP;
	cADDRSTORE;														NOP;
	cSTACK_POP;														NOP;
	
	cHALT;															NOP;
//*/ end : prim_copy_ctrl_mem_ctrl_acc





















