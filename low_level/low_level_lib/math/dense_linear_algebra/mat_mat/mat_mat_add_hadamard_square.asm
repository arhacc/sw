	
/* start : prim_mat_mat_add_hadamard_square
	name:
		prim_mat_mat_add_hadamard_square
	info:
		Computes Hadamard (element wise) addition between 2 matrices that both take up the whole array.
	requiremens hardware:
		`define ARRAY_NR_CELLS (16)
	requirements variables/parameters locations:
		3 parameters, stored at :
			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix 1 and matrix 0
			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MATres_minus_MAT1_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 1
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_000 - function start;
	    label 11_001 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION -1) 
	
	`define CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MATres_minus_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_minus_MAT0_LOCATION -1)

LB(11_000);
	cPARAM;																		ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION);				NOP;	// mem[] = addr_mat0
	cPARAM;																		NOP;	
	cRSTORE(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION);				NOP;	// mem[] = addr_mat1;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION);			NOP;	// mem[] = addr_mat_dest;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION);				NOP;
	cRSUB(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION);				NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_minus_MAT0_LOCATION);	NOP;	// mem[] = addr_mat1 - addr_mat0
	
	cRLOAD(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION);			NOP;
	cRSUB(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION);				NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MATres_minus_MAT1_LOCATION);	NOP;	// mem[] = addr_mat_dest - addr_mat1;

	cVLOAD(-1);																	NOP;
	cSETDEC;																	NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);													NOP;
	cSETCompVal;																NOP;
	cVLOAD(0);																	NOP;
		
	LB(11_001);	
		cRADD(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION);				NOP;
		cRADD(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_minus_MAT0_LOCATION);		CRLOAD;
		cRADD(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MATres_minus_MAT1_LOCATION);	CRADD;
		cRSUB(`CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION);				CRSTORE; 
		cBRNValDEC(11_001);															NOP;
	
	cSEL_ADDRREG(0);															ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_add_hadamard_square






























































