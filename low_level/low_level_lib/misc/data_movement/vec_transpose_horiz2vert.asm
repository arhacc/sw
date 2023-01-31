
/* start : prim_vec_transpose_horiz2vert
	name:
		prim_vec_transpose_horiz2vert
	info:
		transposes a horizontal vector to a vertical vector, stored in one particular cell. (works only for full horizontal)
		a[0] 	b[0]	c[0] ...
        a[1]	b[1]	c[1] 
        a[2]	b[2]	c[2]
        a[3]	b[3]	c[3]
        .........
		vector is split in 2 sections: upper (cell index > position) + lower (cell index < position)
			each secttion is shifted and saved independently 
				saves upper part first (at upper addresses) 
				subtracts vec size from addr_reg 
					(so addr_reg ends up at the first store location), 
				saves lower part 
	requirements hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG 1
	requirements variables/parameters locations:									
		data start address, stored at: CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_VEC0_START_LOC_ADDR_offset
		result start address (dest), stored at: CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset
		cell index at which to save the data, stored at: CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_DEST_CELL_ADDR_offset
	uses:
	    label 10_218 - function start;
	   	label 10_219 - for loop - right shift (lower part of vector) 
		label 10_220 - for loop - left shift (upper part of vector)
		label 10_221 - skip for loop if at cell is at one end;
//*/ 
//*	// code
	`define CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_VEC0_START_LOC_ADDR_offset (`CONTROLLER_INSTR_MEM_SIZE/2 - 1)
	`define CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset (`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_VEC0_START_LOC_ADDR_offset - 1)
	`define CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_DEST_CELL_ADDR_offset (`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset - 1)
	

LB(10218);
	cPARAM;																				NOP;
	cRSTORE(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_VEC0_START_LOC_ADDR_offset);			ADDR_REG_STACK_DUPLICATE;
	cPARAM;																				CRLOAD;
	cRSTORE(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset);			NOP;
	cPARAM;																				NOP;	
	cRSTORE(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_DEST_CELL_ADDR_offset);				NOP;
	cSRSTORE;																			IXLOAD;
	cNOP;																				CRVSUB;
	cVLOAD(-1);																			WHERENZERO;
	cSETDEC;																				SRLOAD;
	cRLOAD(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset);				ELSEWHERE;
	cRADD(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_DEST_CELL_ADDR_offset);						SRLOAD;
	cSETCompVal;																			ADDRINC(1);
	cRLOAD(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_RES_START_LOC_ADDR_offset);					CADDRINC;
LB(10219);
		cGRSHIFT_WOB;																		SRLOAD;
		cBRNValDEC(10219);																	RISTORE(-1);
	
	cNOP;																				ELSEWHERE;
	cSRSTORE;																			ELSEWHERE;	
	
	cRLOAD(`CODE_VEC_TRANSPOSE_HORIZ2VERT_CONTR_DEST_CELL_ADDR_offset);					NOP;
	cVRVSUB(`ARRAY_NR_CELLS-1);															CADDRINC;
	cSETCompVal;																		NOP;
	cVLOAD(0);																			NOP;
	cBRVal(10_221);																		NOP;
	cVLOAD(1);																			NOP;
	cGLSHIFT_WOB;																		NOP;
	
LB(10220);
		cGLSHIFT_WOB;																		SRLOAD;
		cBRNValDEC(10220);																	RISTORE(1);
		
LB(10_221);
	cNOP;																				ACTIVATE;
	
	cSTACK_POP;																			ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);																	NOP;
	
	cHALT;																				NOP;
//*/ //end : prim_vec_transpose_horiz2vert


























