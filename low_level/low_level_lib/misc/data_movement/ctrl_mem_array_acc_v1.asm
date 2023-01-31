
/* start : prim_copy_ctrl_mem_array_acc_v1
	name:
		prim_copy_ctrl_mem_array_acc_v1
	info:
		copy data from controller memory location to array acc. One data is copied to all accumulators
	requiremens hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (4)
	requirements variables/parameters locations:
		param: controller mem addr form where to get acc data, stored at: CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V1_MEM_LOCATION
	uses:
	    label 10_204 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V1_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	
LB(10204);
	cADDRLOAD;														NOP;
	cSTACK_DUPLICATE;												NOP;
	cSTACK_DUPLICATE;												NOP;
	cPARAM;															NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_ARRAY_ACC_V1_MEM_LOCATION);	NOP;
	cSADD;															NOP;
	cADDRSTORE;														NOP;
	cSTACK_POP;														NOP;
	cRSTACK_PUSH_LOAD(0);											NOP;
	cSTACK_POP;														NOP;
	cADDRSTORE;														CLOAD;
	
	cHALT;															NOP;
//*/ end : prim_copy_ctrl_mem_array_acc_v1
























