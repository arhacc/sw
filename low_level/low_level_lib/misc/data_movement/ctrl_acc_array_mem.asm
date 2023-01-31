/* start : prim_copy_ctrl_acc_array_mem
	name:
		prim_copy_ctrl_acc_array_mem
	info:
		copy data from controller acc to one mem location in array
	requiremens hardware:
		none
	requirements variables/parameters locations:
		param: mem addr (from array) at which to save acc data, stored at: CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_MEM_LOCATION
		ram: CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_TEMP
	uses:
	    label 10_203 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_MEM_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_TEMP (`CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_MEM_LOCATION-1)
	
LB(10203);
	cRSTORE(`CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_TEMP);				NOP;
	cPARAM;															CLOAD;
	cRSTORE(`CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_MEM_LOCATION);		NOP;
	cRLOAD(`CODE_PRIM_COPY_CTRL_ACC_ARRAY_MEM_TEMP);				CRSTORE;
	
	cHALT;															NOP;
//*/ end : prim_copy_ctrl_acc_array_mem