
/* start : prim_mat_mat_sub_hadamard_rectangle
	name:
		prim_mat_mat_sub_hadamard_rectangle
	info:
		Computes Hadamard (element wise) subraction between 2 matrices that both take up multiple lines in the complete array.
	requiremens hardware:
		`define ARRAY_NR_CELLS (16)
	requirements variables/parameters locations:
		4 parameters, stored at :
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_NR_LINES : stored in ctrl memory : numer of lines that each matrix has
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix 1 and matrix 0
			CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MATres_minus_MAT1_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 1
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_010 - function start;
	    label 11_011 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_NR_LINES (`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION -1)
	
	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_NR_LINES -1)
	`define CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MATres_minus_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_minus_MAT0_LOCATION -1)

LB(11_010);
	cPARAM;																			ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION);				NOP;	// mem[] = addr_mat0
	cPARAM;																			NOP;	
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION);				NOP;	// mem[] = addr_mat1;
	cPARAM;																			NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION);			NOP;	// mem[] = addr_mat_dest;
	cPARAM;																			NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_NR_LINES);					NOP;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION);				NOP;
	cRSUB(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION);					NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_minus_MAT0_LOCATION);	NOP;	// mem[] = addr_mat1 - addr_mat0
	
	cRLOAD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION);				NOP;
	cRSUB(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_LOCATION);					NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MATres_minus_MAT1_LOCATION);	NOP;	// mem[] = addr_mat_dest - addr_mat1;

	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_NR_LINES);						NOP;
	cVSUB(1);																		NOP;
	cSETCompVal;																	NOP;
	cVLOAD(0);																		NOP;
		
	LB(11_011);	
		cRADD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT0_LOCATION);					NOP;
		cRADD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT1_minus_MAT0_LOCATION);		CRLOAD;
		cRADD(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MATres_minus_MAT1_LOCATION);	CRSUB;
		cRSUB(`CODE_PRIM_MAT_MAT_SUB_HADAMARD_RECTANGLE_MAT_RES_LOCATION);				CRSTORE; 
		cBRNValDEC(11_011);																NOP;
	
	cSEL_ADDRREG(0);																ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_sub_hadamard_rectangle
	