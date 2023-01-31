/* start : prim_reverse_ctrl_mem_data
	name:
		prim_reverse_ctrl_mem_data
	info:
		set addr reg for both controller and array.
	requirements hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (5)
	requirements variables/parameters locations:
		start address from controller mem from which to reverse data, stored at: CODE_PRIM_REVERSE_CTRL_MEM_DATA_START_ADDR_offset_MEM_LOC
		amount of data to be reversed, stored at: CODE_PRIM_REVERSE_CTRL_MEM_DATA_SIZE_MEM_LOC
		
	uses:
	    label 10_210 - function start;
//*/ 
//*	// code
	`define CODE_PRIM_REVERSE_CTRL_MEM_DATA_START_ADDR_offset_MEM_LOC (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_REVERSE_CTRL_MEM_DATA_SIZE_MEM_LOC (`CODE_PRIM_REVERSE_CTRL_MEM_DATA_START_ADDR_offset_MEM_LOC -1)
	


LB(10210);
	cVSTACK_PUSH_LOAD(1);													NOP;
	cSETDEC;																NOP;
	cADDRLOAD;																NOP;
	cSTACK_DUPLICATE;														NOP;
	cSTACK_DUPLICATE;														NOP;
	cPARAM;																	NOP;
	cRSTORE(`CODE_PRIM_REVERSE_CTRL_MEM_DATA_START_ADDR_offset_MEM_LOC);	NOP;
	cSEL_ADDRREG(1);														NOP;
	cSADD;																	NOP;
	cADDRSTORE;																NOP;
	cSTACK_DUPLICATE;														NOP;
	cSTACK_DUPLICATE;														NOP;
	cPARAM;																	NOP;
	cRSTORE(`CODE_PRIM_REVERSE_CTRL_MEM_DATA_SIZE_MEM_LOC);					NOP;
	cSEL_ADDRREG(2);														NOP;
	cSADD;																	NOP;
	cADDRSTORE;																NOP;
	cSSUB;																	NOP;
	cSHRIGHT;																NOP;
	cVSUB(1);																NOP;
	cSEL_ADDRREG(1);														NOP;
	
	LB(10211);
		cRISWAP_ACC_MEM(0);														NOP;
		cSEL_ADDRREG(2);														NOP;
		cRISWAP_ACC_MEM(-1);													NOP;
		cSEL_ADDRREG(1);														NOP;
		cRISWAP_ACC_MEM(0);														NOP;
		cADDRINC(1);															NOP;
		cBRNZDEC(10211);														NOP;
	
	cSTACK_POP;																NOP;
	cSEL_ADDRREG(0);														NOP;
	
	cHALT;																	NOP;
//*/ //end : prim_reverse_ctrl_mem_data






















