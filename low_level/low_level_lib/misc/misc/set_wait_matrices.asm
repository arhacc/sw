/* start : prim_wait_matrices
	name:
		prim_wait_matrices
	info:
		used to call wait_matrix (cWAITMATW) asm instruction in order to make accelerator wait for IO data 
	requiremens hardware:
		none
	requirements variables/parameters locations:
		1 parameters, stored at :
			CODE_PRIM_WAIT_MATRICES_NR_MATRICES_LOCATION : stored in ctrl memory : location for parameter storage
		1 ram data, stored at :
			CODE_PRIM_WAIT_MATRICES_LOOP_COUNTER_LOCATION : stored in ctrl memory : main loop counter
		requires 0 matrices :
	uses:
	    label 10_330 - function start;
	    label 10_331 - main loop
//*/ 
//*	// code

	`define CODE_PRIM_WAIT_MATRICES_NR_MATRICES_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_PRIM_WAIT_MATRICES_LOOP_COUNTER_LOCATION (`CODE_PRIM_WAIT_MATRICES_NR_MATRICES_LOCATION -1)
	
LB(10_330);
	cPARAM;																			NOP;	
	cRSTORE(`CODE_PRIM_WAIT_MATRICES_NR_MATRICES_LOCATION);							NOP;	// mem[] = addr_mat0
	
	cVSUB(1);																		NOP;
	cSETCompVal;																	NOP;
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cVLOAD(0);																		NOP;
	
	LB(10_331);
		cWAITMATW(1);																	NOP;
		cBRNValDEC(10_331);																NOP;
	
	cHALT;																			NOP;
//*/ // end : prim_wait_matrices