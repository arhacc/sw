
/* start : prim_vec_transpose_vert2horiz
	name:
		prim_vec_transpose_vert2horiz
	info:
		a[0] 	b[0]	c[0] ...
        a[1]	b[1]	c[1] 
        a[2]	b[2]	c[2]
        a[3]	b[3]	c[3]
        .........
		transposes a vertical vector to a horizontal vector, stored at one mem addr in all cells
		vector is split in 2 sections: upper (cell index > position) + lower (cell index < position)
			each section is shifted and saved independently 
	requiremens hardware:
		RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG 1
	requirements variables/parameters locations:
		starting location of first vector stored at: VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset
		location of result stored at: VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset
		cell from which data is loaded, stored at: VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset
	uses:
	    label 10_222 - function start;
		label 10_223 - for loop - left shift 
		label 10_224 - for loop - right shift 
		label 10_225 - skip for loop if at cell is at one end
		label 10_226 - skip for loop if at cell is at one end
//*/ 
//*	// code			

	`define VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset (`VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset-1)
	`define VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset (`VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset-1)

LB(10_222);
	cPARAM;																				ADDR_REG_STACK_DUPLICATE;
	cRSTORE(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset);				NOP;
	cPARAM;																				NOP;
	cRSTORE(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset);					NOP;
	cPARAM;																				NOP;
	cRSTORE(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);					NOP;	
	cNOP;																				IXLOAD;
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset);					CSUB;
	cNOP;																				WHERENZERO;
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset);					CADDRINC;
	cRADD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);						ELSEWHERE;
	cNOP;																				NOP; 
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_RES_START_LOC_ADDR_offset);					CRLOAD;
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_VEC0_START_LOC_ADDR_offset);					CRSTORE;
	cVLOAD(-1);																			CADDRINC;
	cSETDEC;																			NOP;
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);						NOP;
	cSETCompVal;																		NOP;
	cVLOAD(0);																			NOP;
	cBRVal(10_225);																		NOP;
	NOP;																				RILOAD(0);
	
LB(10_223);				
		cGLSHIFT_WOB;																		NOP;
		cSRSTORE;																			RILOAD(1);
		cBRNValDEC(10_223);																	NOP;
	cNOP;																				ADDRINC(-1);	
	LB(10_225);
	
	cVLOAD(`ARRAY_NR_CELLS-1);															ELSEWHERE;
	cSETCompVal;																		SRLOAD;
	cRSUB(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);						RSTORE(0);		
	cNOP;																				ELSEWHERE;
	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);						CADDRINC;
	cNOP;																				NOP;
	cBRVal(10_226);																		RILOAD(0);
	cSRSTORE;																			NOP;
	cVADD(1);																			NOP;
	
	LB(10_224);				
		cGRSHIFT_WOB;																		RILOAD(-1);
		cSRSTORE;																			NOP;
		cBRNValDEC(10_224);																	NOP;	
	LB(10_226);

	cRLOAD(`VEC_TRANSPOSE_VERT2HORIZ_CONTR_START_CELL_ADDR_offset);						ACTIVATE;
	cNOP;																				IXLOAD;
	cNOP;																				CRVSUB;
	cNOP;																				WHERECARRY;
	cNOP;																				SRLOAD;
	cNOP;																				RSTORE(0);
	
	cSTACK_POP;																			ACTIVATE;
	cSEL_ADDRREG(0);																	ADDR_REG_STACK_POP;
	
	cHALT;																				NOP;
//*/ // end : prim_vec_transpose_vert2horiz


























