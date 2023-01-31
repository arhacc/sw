
/* start : prim_copy_ctrl_mem_ctrl_mem
	name:
		prim_copy_ctrl_mem_ctrl_mem
	info:
		copy a block of data from one section of ctrl memory to another.
	requirements hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
	requirements variables/parameters locations:
		start address from ctrl mem from which data is copied (source), stored at: CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SOURCE_START_ADDR_offset_MEM_LOC
		start address from ctrl mem to which data is copied (dest), stored at: CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_DEST_START_ADDR_offset_MEM_LOC
		size of data to be copied (nr of entries), stored at: CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SIZE_MEM_LOC
	uses:
	    label 10_216 - function start;
	    label 10_217 - main loop;
//*/ 
//*	// code
	`define CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SOURCE_START_ADDR_offset_MEM_LOC (`CONTROLLER_INSTR_MEM_SIZE/2 - 1)
	`define CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_DEST_START_ADDR_offset_MEM_LOC (`CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SOURCE_START_ADDR_offset_MEM_LOC - 1)
	`define CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SIZE_MEM_LOC (`CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_DEST_START_ADDR_offset_MEM_LOC - 1)
	

LB(10216);
	cVSTACK_PUSH_LOAD(-1);																		NOP;
	cSETDEC;																					NOP;
	cADDRLOAD;																					NOP;
	cSTACK_DUPLICATE;																			NOP;
	cSTACK_DUPLICATE;																			NOP;
	cVSTACK_PUSH_LOAD(0);																		NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SOURCE_START_ADDR_offset_MEM_LOC);				NOP;
	cSEL_ADDRREG(1);																			NOP;
	cVSUB(1);																					NOP;
	cSADD;																						NOP;
	cADDRSTORE;																					NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_DEST_START_ADDR_offset_MEM_LOC);					NOP;
	cSEL_ADDRREG(2);																			NOP;
	cVSUB(1);																					NOP;
	cSADD;																						NOP;
	cADDRSTORE;																					NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_CTRL_MEM_CTRL_MEM_SIZE_MEM_LOC);									NOP;
	cVSUB(1);																					NOP;
	cSETCompVal;																				NOP;
	cVLOAD(0);																					NOP;
	
	LB(10217);
		cSEL_ADDRREG(1);																			NOP;
		cRISTACK_PUSH_LOAD(1);																		NOP;
		cSEL_ADDRREG(2);																			NOP;
		cRISTORE(1);																				NOP;
		cSTACK_POP;																					NOP;
		cBRNValDEC(10217);																			NOP;
	
	cSTACK_POP;																					NOP;
	cSEL_ADDRREG(0);																			NOP;
	
	cHALT;																						NOP;
//*/ //end : prim_copy_ctrl_mem_ctrl_mem























