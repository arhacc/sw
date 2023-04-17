
/* start : prim_copy_ctrl_acc_ctrl_mem
	name:
		prim_copy_ctrl_acc_ctrl_mem
	info:
		copy data from controller acc to a specific location in controller memory
	requiremens hardware:
		min RESOURCE_CTRL_CELL_STACK_SIZE 4
	requirements variables/parameters locations:
		param: mem addr at which to save acc data, stored at: CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_MEM_LOCATION
		ram: CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_TEMP
	uses:
	    label 10_200 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_TEMP (`CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_MEM_LOCATION-1)
	
LB(10200);
	cRSTORE(`CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_TEMP);				NOP;
	cPARAM;															NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_MEM_LOCATION);		NOP;
	cADDRLOAD;														NOP;
	cRSTACK_PUSH_LOAD(`CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_TEMP);		NOP;
	cSTACK_OVER;													NOP;
	cRADD(`CODE_PRIM_COPY_CTRL_ACC_CTRL_MEM_MEM_LOCATION);			NOP;
	cADDRSTORE;														NOP;
	cSTACK_POP;														NOP;
	cRSTORE(0);														NOP;
	cSTACK_SWAP;													NOP;
	cADDRSTORE;														NOP;
	cSTACK_POP;														NOP;
	
	cHALT;															NOP;
//*/ end : prim_copy_ctrl_acc_ctrl_mem






















