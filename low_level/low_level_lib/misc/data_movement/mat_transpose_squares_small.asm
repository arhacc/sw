


/* // initial line + bw bits for permutations (not diagonalized, stored line by line)
		// bw bits and initial lines computed as needed by each possible size

// 16 cells: 

	// 4x4 matrices				0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 1 3 1 3 1 3 1 3 1 3 1 3 1 3 
2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 
3 1 3 1 3 1 3 1 3 1 3 1 3 1 3 1

	// 3x3 matrices				0 1 2 0 1 2 0 1 2 0 1 2 0 1 2 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 2 100 64 6 35 57 14 66 98 2 1 23 2 0 
67 1 3 101 70 64 121 35 15 67 66 98 87 1 3 65 

	// 5x5 matrices				0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 1 7 4 66 64 90 64 12 1 37 21 33 4 0 
2 2 71 3 5 67 26 56 76 78 36 6 87 7 39 119 
3 39 2 2 6 122 90 2 44 14 7 5 6 36 22 20 
103 1 3 1 7 123 66 98 78 64 69 35 7 1 3 85
	
	// 6x6 matrices				0 1 2 3 4 5 0 1 2 3 4 5 0 1 2 3
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 1 7 1 5 1 57 1 33 1 13 1 23 1 3 
2 2 6 6 4 4 56 56 32 32 12 12 22 22 2 2 
3 39 7 3 5 7 57 59 33 45 13 33 23 21 3 1 
38 38 2 2 6 6 58 58 44 44 32 32 20 20 0 0 
39 1 3 1 7 1 59 1 45 1 33 1 21 3 1 3 
	
	// 8x8 matrices				0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 1 7 1 3 1 7 1 3 1 7 1 3 1 7 
2 2 6 6 2 2 6 6 2 2 6 6 2 2 6 6 
3 5 7 5 3 5 7 5 3 5 7 5 3 5 7 5 
4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 
5 7 5 3 5 7 5 3 5 7 5 3 5 7 5 3 
6 6 2 2 6 6 2 2 6 6 2 2 6 6 2 2 
7 1 3 1 7 1 3 1 7 1 3 1 7 1 3 1 
	
	// 11x11 matrices			0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 	
1 3 1 7 1 3 1 15 1 3 10 100 64 96 64 4 
2 2 6 6 2 2 14 14 67 9 11 101 32 2 68 66 
3 5 7 5 3 13 15 13 8 8 8 68 66 6 2 2 
4 4 4 4 46 12 77 7 9 11 9 103 100 64 96 64 
5 7 5 31 47 15 6 6 10 10 14 34 20 0 0 0 
6 6 30 30 79 5 7 5 11 13 15 37 21 53 1 69 
7 29 31 29 4 4 4 4 12 12 12 36 22 52 5 23 
62 28 93 3 5 7 5 3 13 15 13 39 23 101 52 22 
63 31 2 2 6 6 2 2 14 14 2 38 37 1 53 21 
95 1 3 1 7 1 3 1 15 1 3 65 0 0 0 20  
	
	// 16x16 matrices			0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 3 1 7 1 3 1 15 1 3 1 7 1 3 1 15 
2 2 6 6 2 2 14 14 2 2 6 6 2 2 14 14 
3 5 7 5 3 13 15 13 3 5 7 5 3 13 15 13 
4 4 4 4 12 12 12 12 4 4 4 4 12 12 12 12 
5 7 5 11 13 15 13 11 5 7 5 11 13 15 13 11 
6 6 10 10 14 14 10 10 6 6 10 10 14 14 10 10 
7 9 11 9 15 9 11 9 7 9 11 9 15 9 11 9 
8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 
9 11 9 15 9 11 9 7 9 11 9 15 9 11 9 7 
10 10 14 14 10 10 6 6 10 10 14 14 10 10 6 6 
11 13 15 13 11 5 7 5 11 13 15 13 11 5 7 5 
12 12 12 12 4 4 4 4 12 12 12 12 4 4 4 4 
13 15 13 3 5 7 5 3 13 15 13 3 5 7 5 3 
14 14 2 2 6 6 2 2 14 14 2 2 6 6 2 2 
15 1 3 1 7 1 3 1 15 1 3 1 7 1 3 1




// 64 cells:




// 128 cells:




// 256 cells:













// multiple ARRAY_NR_CELLS sizes


	//size 16 cells 
/* start : prim_mat_transpose_squares_small
	name:
		prim_mat_transpose_squares_small
	info:
		Transposes square matrices of any size, smaller than ARRAY_NR_CELLS.
		Algorithm goes through the matrixes, one diagonal at a time.
		Line stored at "CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION" must be of the following values: ixload % small_matrix_size
			For example, for size 5: 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 5 0
		If ARRAY_NR_CELLSdoes not divide evenly by the small matrix size, data from last cells will be garbage.
	requiremens hardware:
		`define ARRAY_NR_CELLS (16)
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		`define RESOURCE_ARRAY_HAS_BW_SCAN_NET (1)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
			`define RESOURCE_NET_HAS_OPERATION_PERMUTE (1)
		`define RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION (1) 
			or
		`define RESOURCE_NETWORK_SINGLE_SCAN_MODE (`NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT)
	requirements variables/parameters locations:
		4 parameters, stored at :
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION : stored in ctrl memory : location from array mem where initial line (ixlode % rectangle_size) is found
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE : stored in ctrl memory : location of square matrices size in number of cells occupied 
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION : stored in ctrl memory : location from array mem where initial matrix is found (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION : stored in ctrl memory : location from array mem where result is stored (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION : stored in ctrl memory : location from array mem where bw permutation bits are found (offset)
		requires 2 matrices :
			bw permutation bits
			data to transpose 
	uses:
	    label 10_229 - function start;
	    label 10_230 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION -1) 
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION -1)


LB(10_229);
	
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cNOP;																		CRLOAD;
	cSRSTORE;																	NOP;
	cVLOAD(-1);																	ADDR_REG_STACK_DUPLICATE;
	cSETDEC;																	NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);	NOP;
	cVSUB(1);																	NOP;
	cSETCompVal;																NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);			NOP;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cVLOAD(0);																	VLOAD(9995);
	
LB(10_230);
	cSTACK_DUPLICATE;															ADDR_REG_STACK_POP;
	cRADD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);				ADDR_REG_STACK_DUPLICATE;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															NOP;
	cRSUB(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		SCANPERMUTE(2);
	cNOP;																		ADDR_REG_STACK_POP;
	cVRVSUB(0);																	ADDR_REG_STACK_DUPLICATE;
	cNOP;																		SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;																		WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															RSTACK_PUSH_LOAD(0);	// permute network start 
	cNOP;     																	ADDR_REG_STACK_POP;
	cNOP;    																	ADDR_REG_STACK_DUPLICATE;
	cNOP;           															SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;                                          								WHERECARRY;
	cNOP;																			CADD;
	cNOP;                                           							ENDWHERE;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);			ADDRINC_ACC;		
	cNOP;                                           							SCANLOAD;
	cSTACK_POP;																	CADDRINC;
	cBRNValDEC(10_230);															RSTORE(0); 
	
	cNOP;																	ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);														NOP;
	
	cHALT;																	NOP;
//*/ // end : prim_mat_transpose_squares_small





			// 32 cells
/* start : prim_mat_transpose_squares_small
	name:
		prim_mat_transpose_squares_small
	info:
		Transposes square matrices of any size, smaller than ARRAY_NR_CELLS.
		Algorithm goes through the matrixes, one diagonal at a time.
		Line stored at "CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION" must be of the following values: ixload % small_matrix_size
			For example, for size 5: 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 5 0
		If ARRAY_NR_CELLSdoes not divide evenly by the small matrix size, data from last cells will be garbage.
	requiremens hardware:
		`define ARRAY_NR_CELLS (32)
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		`define RESOURCE_ARRAY_HAS_BW_SCAN_NET (1)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
			`define RESOURCE_NET_HAS_OPERATION_PERMUTE (1)
		`define RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION (1) 
			or
		`define RESOURCE_NETWORK_SINGLE_SCAN_MODE (`NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT)
	requirements variables/parameters locations:
		4 parameters, stored at :
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION : stored in ctrl memory : location from array mem where initial line (ixlode % rectangle_size) is found
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE : stored in ctrl memory : location of square matrices size in number of cells occupied 
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION : stored in ctrl memory : location from array mem where initial matrix is found (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION : stored in ctrl memory : location from array mem where result is stored (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION : stored in ctrl memory : location from array mem where bw permutation bits are found (offset)
		requires 2 matrices :
			bw permutation bits
			data to transpose 
	uses:
	    label 10_229 - function start;
	    label 10_230 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION -1) 
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION -1)


LB(10_229);
	
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cNOP;																		CRLOAD;
	cSRSTORE;																	NOP;
	cVLOAD(-1);																	ADDR_REG_STACK_DUPLICATE;
	cSETDEC;																	NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);	NOP;
	cVSUB(1);																	NOP;
	cSETCompVal;																NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);			NOP;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cVLOAD(0);																	VLOAD(9995);
	
LB(10_230);
	cSTACK_DUPLICATE;															ADDR_REG_STACK_POP;
	cRADD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);				ADDR_REG_STACK_DUPLICATE;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															NOP;
	cRSUB(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		SCANPERMUTE(2);
	cNOP;																		ADDR_REG_STACK_POP;
	cVRVSUB(0);																	ADDR_REG_STACK_DUPLICATE;
	cNOP;																		SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;																		WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															RSTACK_PUSH_LOAD(0);	// permute network start 
	cNOP;     																	ADDR_REG_STACK_POP;
	cNOP;    																	ADDR_REG_STACK_DUPLICATE;
	cNOP;           															SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;                                          								WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);           ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cNOP;																		CADDRINC;
	cNOP;																		NOP;
	cSTACK_POP;                                           						SCANLOAD;
	cBRNValDEC(10_230);															RSTORE(0); 
															
	
	cNOP;																	ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);														NOP;
	
	cHALT;																	NOP;
//*/ // end : prim_mat_transpose_squares_small








		// 64 cells
/* start : prim_mat_transpose_squares_small
	name:
		prim_mat_transpose_squares_small
	info:
		Transposes square matrices of any size, smaller than ARRAY_NR_CELLS.
		Algorithm goes through the matrixes, one diagonal at a time.
		Line stored at "CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION" must be of the following values: ixload % small_matrix_size
			For example, for size 5: 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 5 0
		If ARRAY_NR_CELLSdoes not divide evenly by the small matrix size, data from last cells will be garbage.
	requiremens hardware:
		`define ARRAY_NR_CELLS (64)
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		`define RESOURCE_ARRAY_HAS_BW_SCAN_NET (1)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
			`define RESOURCE_NET_HAS_OPERATION_PERMUTE (1)
		`define RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION (1) 
			or
		`define RESOURCE_NETWORK_SINGLE_SCAN_MODE (`NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT)
	requirements variables/parameters locations:
		4 parameters, stored at :
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION : stored in ctrl memory : location from array mem where initial line (ixlode % rectangle_size) is found
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE : stored in ctrl memory : location of square matrices size in number of cells occupied 
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION : stored in ctrl memory : location from array mem where initial matrix is found (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION : stored in ctrl memory : location from array mem where result is stored (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION : stored in ctrl memory : location from array mem where bw permutation bits are found (offset)
		requires 2 matrices :
			bw permutation bits
			data to transpose 
	uses:
	    label 10_229 - function start;
	    label 10_230 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION -1) 
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION -1)


LB(10_229);
	
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cNOP;																		CRLOAD;
	cSRSTORE;																	NOP;
	cVLOAD(-1);																	ADDR_REG_STACK_DUPLICATE;
	cSETDEC;																	NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);	NOP;
	cVSUB(1);																	NOP;
	cSETCompVal;																NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);			NOP;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cVLOAD(0);																	VLOAD(9995);
	
LB(10_230);
	cSTACK_DUPLICATE;															ADDR_REG_STACK_POP;
	cRADD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);				ADDR_REG_STACK_DUPLICATE;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															NOP;
	cRSUB(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		SCANPERMUTE(2);
	cNOP;																		ADDR_REG_STACK_POP;
	cVRVSUB(0);																	ADDR_REG_STACK_DUPLICATE;
	cNOP;																		SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;																		WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															RSTACK_PUSH_LOAD(0);	// permute network start 
	cNOP;     																	ADDR_REG_STACK_POP;
	cNOP;    																	ADDR_REG_STACK_DUPLICATE;
	cNOP;           															SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;                                          								WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);           ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cNOP;																		CADDRINC;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cSTACK_POP;                                           						SCANLOAD;
	cBRNValDEC(10_230);															RSTORE(0); 
															
	
	cNOP;																	ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);														NOP;
	
	cHALT;																	NOP;
//*/ // end : prim_mat_transpose_squares_small








		// 128 cells 
/* start : prim_mat_transpose_squares_small
	name:
		prim_mat_transpose_squares_small
	info:
		Transposes square matrices of any size, smaller than ARRAY_NR_CELLS.
		Algorithm goes through the matrixes, one diagonal at a time.
		Line stored at "CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION" must be of the following values: ixload % small_matrix_size
			For example, for size 5: 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 5 0
		If ARRAY_NR_CELLSdoes not divide evenly by the small matrix size, data from last cells will be garbage.
	requiremens hardware:
		`define ARRAY_NR_CELLS (128)
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		`define RESOURCE_ARRAY_HAS_BW_SCAN_NET (1)
		`define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG (1)
			`define RESOURCE_NET_HAS_OPERATION_PERMUTE (1)
		`define RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION (1) 
			or
		`define RESOURCE_NETWORK_SINGLE_SCAN_MODE (`NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT)
	requirements variables/parameters locations:
		4 parameters, stored at :
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION : stored in ctrl memory : location from array mem where initial line (ixlode % rectangle_size) is found
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE : stored in ctrl memory : location of square matrices size in number of cells occupied 
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION : stored in ctrl memory : location from array mem where initial matrix is found (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION : stored in ctrl memory : location from array mem where result is stored (offset)
			CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION : stored in ctrl memory : location from array mem where bw permutation bits are found (offset)
		requires 2 matrices :
			bw permutation bits
			data to transpose 
	uses:
	    label 10_229 - function start;
	    label 10_230 - main loop 
//*/ 
//*	// code			

	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION -1) 
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION -1)
	`define CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION (`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION -1)


LB(10_229);
	
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cNOP;																		CRLOAD;
	cSRSTORE;																	NOP;
	cVLOAD(-1);																	ADDR_REG_STACK_DUPLICATE;
	cSETDEC;																	NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);	NOP;
	cVSUB(1);																	NOP;
	cSETCompVal;																NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);			NOP;
	cPARAM;																		NOP;
	cRSTORE(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);			NOP;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_INITIAL_LINE_LOCATION);		NOP;
	cVLOAD(0);																	VLOAD(9995);
	
LB(10_230);
	cSTACK_DUPLICATE;															ADDR_REG_STACK_POP;
	cRADD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_BWBITS_LOCATION);				ADDR_REG_STACK_DUPLICATE;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															NOP;
	cRSUB(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		SCANPERMUTE(2);
	cNOP;																		ADDR_REG_STACK_POP;
	cVRVSUB(0);																	ADDR_REG_STACK_DUPLICATE;
	cNOP;																		SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;																		WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_IN_LOCATION);			ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cSTACK_POP;																	CADDRINC;
	cSTACK_DUPLICATE;															RSTACK_PUSH_LOAD(0);	// permute network start 
	cNOP;     																	ADDR_REG_STACK_POP;
	cNOP;    																	ADDR_REG_STACK_DUPLICATE;
	cNOP;           															SRLOAD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_RECTANGLE_SIZE_LOCATION);		CSUB;
	cNOP;                                          								WHERECARRY;
	cNOP;																			CADD;
	cRLOAD(`CODE_PRIM_MAT_TRANSPOSE_SQUARES_SMALL_DATA_OUT_LOCATION);           ENDWHERE;
	cNOP;																		ADDRINC_ACC;
	cNOP;																		CADDRINC;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cNOP;																		NOP;
	cSTACK_POP;                                           						SCANLOAD;
	cBRNValDEC(10_230);															RSTORE(0); 
															
	
	cNOP;																	ADDR_REG_STACK_POP;
	cSEL_ADDRREG(0);														NOP;
	
	cHALT;																	NOP;
//*/ // end : prim_mat_transpose_squares_small














