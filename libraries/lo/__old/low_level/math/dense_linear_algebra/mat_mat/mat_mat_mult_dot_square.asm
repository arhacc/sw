

// code varies with size due to reduce network 








			// 16 cells 
			
/* start : prim_mat_mat_mult_dot_square
	name:
		prim_mat_mat_mult_dot_square
	info:
		Computes dot multiplication between 2 matrices that both take up lines in all the array cells.
		Assumes second matrix is already transposed.
	requiremens hardware:
		`define ARRAY_NR_CELLS (16)
		`define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_CELL_HAS_STACK (1) 
		`define RESOURCE_ARRAY_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_REDUCE_NET (1)
		`define RESOURCE_NET_HAS_OPERATION_ADD (1)
	requirements variables/parameters locations:
		3 parameters, stored at :
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION : store in ctrl memory : currend location of mat0 read line computed as initial + i
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 0
																				// additional "-1" needed to sync addresses properly
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_012 - function start;
	    label 11_013 - main loop 
	    label 11_014 - function end
//*/ 
//*	// code

	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION -1)
	
LB(11_012);
	cPARAM;																			ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						SCANADD(0);	// mem[] = addr_mat0
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			CLOAD;
	cPARAM;																			ADDRINC_ACC;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;	// mem[] = addr_mat1;
	cPARAM;																			RLOAD(0);
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					STACK_DUPLICATE;	// mem[] = addr_mat_dest;
	cVSUB(1);																		ADDR_REG_STACK_POP;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					ADDR_REG_STACK_DUPLICATE;
	cRSUB(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						NOP;
	cVSUB(1);																		NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);		NOP;
	
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);														NOP;
	cNOP;																			CLOAD;
	cSETCompVal;																	VSUB(1);
	cVLOAD(0);																		ADDRINC_ACC;
	cSTACK_DUPLICATE;																STACK_LOAD_LAYER1;
	
	LB(11_013);			// inner loop
		cRIGHT_REDINS;																	RIMULT(1);
		cBRNValDEC(11_013);																STACK_LOAD_LAYER1;
	// outer loop
	cRIGHT_REDINS;																	ADDR_REG_STACK_POP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			ADDR_REG_STACK_DUPLICATE;
	cRIGHT_REDINS;																	NOP;
	cVADD(1);																		NOP;
	cRIGHT_REDINS;																	CRLOAD;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			VSTACK_PUSH_LOAD(0);
	cRADD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);			SRLOAD;
	cSTACK_LOAD_LAYER1;																CRSTORE;
	cVSUB(`ARRAY_NR_CELLS-1);														STACK_LOAD_LAYER1;
	cBRZ(11_014);																	NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cNOP;																			NOP;
	cSTACK_POP;																		CLOAD;					// asta il pot da mai sus ?
	cVADD(1);																		VSUB(1); 
	cVSTACK_PUSH_LOAD(0);															ADDRINC_ACC;
	cJMP(11_013);																	STACK_LOAD_LAYER1;
	
LB(11_014);	
	cSEL_ADDRREG(0);																ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_mult_dot_square




				
				//32 cells
				
/* start : prim_mat_mat_mult_dot_square
	name:
		prim_mat_mat_mult_dot_square
	info:
		Computes dot multiplication between 2 matrices that both take up lines in all the array cells.
		Assumes second matrix is already transposed.
	requiremens hardware:
		`define ARRAY_NR_CELLS (32)
		`define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_CELL_HAS_STACK (1) 
		`define RESOURCE_ARRAY_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_REDUCE_NET (1)
		`define RESOURCE_NET_HAS_OPERATION_ADD (1)
	requirements variables/parameters locations:
		3 parameters, stored at :
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION : store in ctrl memory : currend location of mat0 read line computed as initial + i
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 0
																				// additional "-1" needed to sync addresses properly
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_012 - function start;
	    label 11_013 - main loop 
	    label 11_014 - function end
//*/ 
//*	// code

	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION -1)
	
LB(11_012);
	cPARAM;																			ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						SCANADD(0);	// mem[] = addr_mat0
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			CLOAD;
	cPARAM;																			ADDRINC_ACC;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;	// mem[] = addr_mat1;
	cPARAM;																			RLOAD(0);
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					STACK_DUPLICATE;	// mem[] = addr_mat_dest;
	cVSUB(1);																		ADDR_REG_STACK_POP;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					ADDR_REG_STACK_DUPLICATE;
	cRSUB(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						NOP;
	cVSUB(1);																		NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);		NOP;
	
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);														NOP;
	cNOP;																			CLOAD;
	cSETCompVal;																	VSUB(1);
	cVLOAD(0);																		ADDRINC_ACC;
	cSTACK_DUPLICATE;																NOP;
	
	LB(11_013);			// inner loop
		cRIGHT_REDINS;																	STACK_LOAD_LAYER1;
		cBRNValDEC(11_013);																RIMULT(1);
	// outer loop
	cRIGHT_REDINS;																	ADDR_REG_STACK_POP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			ADDR_REG_STACK_DUPLICATE;
	cRIGHT_REDINS;																	NOP;
	cVADD(1);																		NOP;
	cRIGHT_REDINS;																	CRLOAD;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			VSTACK_PUSH_LOAD(0);
	cRIGHT_REDINS;																	NOP;
	cRADD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);			SRLOAD;
	cSTACK_LOAD_LAYER1;																CRSTORE;
	cVSUB(`ARRAY_NR_CELLS-1);														STACK_LOAD_LAYER1;
	cBRZ(11_014);																	NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cNOP;																			NOP;
	cSTACK_POP;																		CLOAD;															// asta il pot da mai sus ?
	cVADD(1);																		VSUB(1); 
	cVSTACK_PUSH_LOAD(0);															ADDRINC_ACC;
	cJMP(11_013);																	STACK_LOAD_LAYER1;
	
LB(11_014);	
	cSEL_ADDRREG(0);																ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_mult_dot_square






				// 64 cells 

/* start : prim_mat_mat_mult_dot_square
	name:
		prim_mat_mat_mult_dot_square
	info:
		Computes dot multiplication between 2 matrices that both take up lines in all the array cells.
		Assumes second matrix is already transposed.
	requiremens hardware:
		`define ARRAY_NR_CELLS (64)
		`define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_CELL_HAS_STACK (1) 
		`define RESOURCE_ARRAY_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_REDUCE_NET (1)
		`define RESOURCE_NET_HAS_OPERATION_ADD (1)
	requirements variables/parameters locations:
		3 parameters, stored at :
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION : store in ctrl memory : currend location of mat0 read line computed as initial + i
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 0
																				// additional "-1" needed to sync addresses properly
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_012 - function start;
	    label 11_013 - main loop 
	    label 11_014 - function end
//*/ 
//*	// code

	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION -1)
	
LB(11_012);
	cPARAM;																			ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						SCANADD(0);	// mem[] = addr_mat0
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			CLOAD;
	cPARAM;																			ADDRINC_ACC;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;	// mem[] = addr_mat1;
	cPARAM;																			RLOAD(0);
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					STACK_DUPLICATE;	// mem[] = addr_mat_dest;
	cVSUB(1);																		ADDR_REG_STACK_POP;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					ADDR_REG_STACK_DUPLICATE;
	cRSUB(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						NOP;
	cVSUB(1);																		NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);		NOP;
	
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);														NOP;
	cNOP;																			CLOAD;
	cSETCompVal;																	VSUB(1);
	cVLOAD(0);																		ADDRINC_ACC;
	cSTACK_DUPLICATE;																STACK_LOAD_LAYER1;
	
	LB(11_013);			// inner loop
		cRIGHT_REDINS;																	RIMULT(1);
		cBRNValDEC(11_013);																STACK_LOAD_LAYER1;
	// outer loop
	cRIGHT_REDINS;																	ADDR_REG_STACK_POP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			ADDR_REG_STACK_DUPLICATE;
	cRIGHT_REDINS;																	NOP;
	cVADD(1);																		NOP;
	cRIGHT_REDINS;																	CRLOAD;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			VSTACK_PUSH_LOAD(0);
	cRIGHT_REDINS;																	NOP;
	cRADD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);			SRLOAD;
	cSTACK_LOAD_LAYER1;																CRSTORE;
	cVSUB(`ARRAY_NR_CELLS-1);														STACK_LOAD_LAYER1;
	cBRZ(11_014);																	NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cNOP;																			NOP;
	cSTACK_POP;																		CLOAD;					// asta il pot da mai sus ?
	cVADD(1);																		VSUB(1); 
	cVSTACK_PUSH_LOAD(0);															ADDRINC_ACC;
	cJMP(11_013);																	STACK_LOAD_LAYER1;
	
LB(11_014);	
	cSEL_ADDRREG(0);																ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_mult_dot_square







				// 128 cells 

/* start : prim_mat_mat_mult_dot_square
	name:
		prim_mat_mat_mult_dot_square
	info:
		Computes dot multiplication between 2 matrices that both take up lines in all the array cells.
		Assumes second matrix is already transposed.
	requiremens hardware:
		`define ARRAY_NR_CELLS (128)
		`define RESOURCE_CTRL_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_CELL_HAS_STACK (1) 
		`define RESOURCE_ARRAY_CELL_STACK_SIZE (4)
		`define RESOURCE_ARRAY_HAS_REDUCE_NET (1)
		`define RESOURCE_NET_HAS_OPERATION_ADD (1)
	requirements variables/parameters locations:
		3 parameters, stored at :
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
		2 ram locations stored at:
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION : store in ctrl memory : currend location of mat0 read line computed as initial + i
			CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION : stored in ctrl memory : difference of relative address between matrix result and matrix 0
																				// additional "-1" needed to sync addresses properly
		requires 2 matrices :
			mat data in 0
			mat data in 1
	uses:
	    label 11_012 - function start;
	    label 11_013 - main loop 
	    label 11_014 - function end
//*/ 
//*	// code

	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION -1)
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION -1) 
	`define CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION (`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION -1)
	
LB(11_012);
	cPARAM;																			ADDR_REG_STACK_DUPLICATE;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						SCANADD(0);	// mem[] = addr_mat0
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			CLOAD;
	cPARAM;																			ADDRINC_ACC;	
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;	// mem[] = addr_mat1;
	cPARAM;																			RLOAD(0);
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					STACK_DUPLICATE;	// mem[] = addr_mat_dest;
	cVSUB(1);																		ADDR_REG_STACK_POP;
	
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_LOCATION);					ADDR_REG_STACK_DUPLICATE;
	cRSUB(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_LOCATION);						NOP;
	cVSUB(1);																		NOP;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);		NOP;
	
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cVLOAD(`ARRAY_NR_CELLS-1);														NOP;
	cNOP;																			CLOAD;
	cSETCompVal;																	VSUB(1);
	cVLOAD(0);																		ADDRINC_ACC;
	cSTACK_DUPLICATE;																NOP;
	
	LB(11_013);			// inner loop
		cRIGHT_REDINS;																	STACK_LOAD_LAYER1;
		cBRNValDEC(11_013);																RIMULT(1);
	// outer loop
	cRIGHT_REDINS;																	ADDR_REG_STACK_POP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			ADDR_REG_STACK_DUPLICATE;
	cRIGHT_REDINS;																	NOP;
	cVADD(1);																		NOP;
	cRIGHT_REDINS;																	CRLOAD;
	cRSTORE(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT0_current_val_LOCATION);			VSTACK_PUSH_LOAD(0);
	cRIGHT_REDINS;																	NOP;
	cRADD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT_RES_minus_MAT0_LOCATION);			NOP;
	cRIGHT_REDINS;																	NOP;
	cNOP;																			SRLOAD;
	cSTACK_LOAD_LAYER1;																CRSTORE;
	cVSUB(`ARRAY_NR_CELLS-1);														STACK_LOAD_LAYER1;
	cBRZ(11_014);																	NOP;
	cRLOAD(`CODE_PRIM_MAT_MAT_MULT_DOT_SQUARE_MAT1_LOCATION);						NOP;
	cNOP;																			NOP;
	cSTACK_POP;																		CLOAD;															// asta il pot da mai sus ?
	cVADD(1);																		VSUB(1); 
	cVSTACK_PUSH_LOAD(0);															ADDRINC_ACC;
	cJMP(11_013);																	STACK_LOAD_LAYER1;
	
LB(11_014);	
	cSEL_ADDRREG(0);																ADDR_REG_STACK_POP;
	
	cHALT;																			NOP;
//*/ // end : prim_mat_mat_mult_dot_square













































































