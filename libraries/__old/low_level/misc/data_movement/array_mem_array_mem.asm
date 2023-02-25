
/* start : prim_copy_array_mem_array_mem
	name:
		prim_copy_array_mem_array_mem
	info:
		copy a block of data from one section of array memory to another.
	requirements hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
	requirements variables/parameters locations:
		start address from array mem from which data is copied (source), stored at: CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SOURCE_START_ADDR_offset_MEM_LOC
		start address from array mem to which data is copied (dest), stored at: CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_DEST_START_ADDR_offset_MEM_LOC
		size of data to be copied (nr of entries), stored at: CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SIZE_MEM_LOC
	uses:
	    label 10_214 - function start;
	    label 10_215 - main loop
//*/ 
//*	// code
	`define CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SOURCE_START_ADDR_offset_MEM_LOC (`CONTROLLER_INSTR_MEM_SIZE/2 - 1)
	`define CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_DEST_START_ADDR_offset_MEM_LOC (`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SOURCE_START_ADDR_offset_MEM_LOC - 1)
	`define CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SIZE_MEM_LOC (`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_DEST_START_ADDR_offset_MEM_LOC - 1)
	

LB(10214);
	cVSTACK_PUSH_LOAD(-1);																		NOP;
	cSETDEC;																					NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SOURCE_START_ADDR_offset_MEM_LOC);				NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_DEST_START_ADDR_offset_MEM_LOC);				NOP;
	cPARAM;																						NOP;
	cRSTORE(`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SIZE_MEM_LOC);									NOP;
	cVSUB(1);																					NOP;
	cSETCompVal;																				NOP;
	cVLOAD(0);																					ADDR_REG_STACK_DUPLICATE;
		
	LB(10215);
		cRSTACK_PUSH_LOAD(`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_SOURCE_START_ADDR_offset_MEM_LOC);	NOP;
		cRLOAD(`CODE_PRIM_COPY_ARRAY_MEM_ARRAY_MEM_DEST_START_ADDR_offset_MEM_LOC);					CRLOAD;
		cSTACK_POP;																					CRSTORE;
		cBRNValDEC(10215);																			ADDRINC(1);
		
	cSTACK_POP;																					ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);																			NOP;
	
	cHALT;																						NOP;
//*/ //end : prim_copy_array_mem_array_mem




























