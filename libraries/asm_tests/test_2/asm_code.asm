

/***************************************************************************************
THE PROGRAM: normal testing from example file
***************************************************************************************/
// THE PROGRAM USED TO INITIALIZE THE ACCELERATOR
//*


	LB(0);
			cPLOAD(0);			NOP;			// line not counted ( not saved in controller mem)
			cHALT;				NOP;			// line 0

	LB(1);
			cJMP(10_350);		NOP; // prim_initialize									// line 1
			cJMP(10_300);		NOP; // prim_set_addr_regs								// line 2
			cJMP(10_310);		NOP; // prim_set_res_ready								// line 3
			cJMP(10_320);		NOP; // prim_set_interrupt								// line 4
			cJMP(10_330);		NOP; // prim_wait_matrices								// line 5
			cJMP(10_360);		NOP; // prim_cycle_counter_start_W_halt					// line 6
			cJMP(10_361);		NOP; // prim_cycle_counter_start_WO_halt				// line 7
			cJMP(10_362);		NOP; // prim_cycle_counter_stop							// line 8
			cJMP(10_363);		NOP; // prim_cycle_counter_reset						// line 9
			cJMP(10104);		NOP; // prim_delay_5									// line 10
			cJMP(9000);			NOP; // prim_temp_test_dma_hw							// line 11
			cJMP(9200);			NOP; // prim_temp_fast_testing							// line 12
			cJMP(9001);			NOP; // prim_temp_test3									// line 13












/* start : prim_initialize
	name:
		prim_initialize
	info:
		Initial configurations for xpu accelerator.
	requirements hardware:
		none
	requirements variables/parameters locations:
		none

	uses:
	    label 10_350 - function start;
//*/
//*	// code

LB(10_350);
	cNOP;				ACTIVATE;
	cSEL_ADDRREG(0); 	NOP;
	cNOP;				SCANNOP;
	cHALT;				NOP;
//*/ end : prim_initialize










					//mandatory addr_reg setup
//cNOP;				NOP;
//cVLOAD(0);		VLOAD(0);
//cADDRSTORE;		ADDRSTORE;



/* start : prim_set_addr_regs
	name:
		prim_set_addr_regs
	info:
		set addr reg for both controller and array.
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
	2 parameters:
		array address
		controller address
	uses:
	    label 10_300 - function start;
//*/
//*	// code


LB(10_300);
	cPARAM;			NOP;
	cPARAM;			NOP;
	cADDRSTORE;		NOP;
	cNOP;			CLOAD;
	cNOP;			ADDRSTORE;

	cHALT;			NOP;
//*/ end : prim_set_addr_regs









/* start : prim_set_interrupt
	name:
		prim_set_interrupt
	info:
		Calls set interrupt asm instruction to generate an interrupt towards the mid level
	requirements hardware:
		none
	requirements variables/parameters locations:
		none

	uses:
	    label 10_320 - function start;
//*/
//*	// code


LB(10320);
	cSETINT;    NOP;

	cHALT;		NOP;
//*/ end : prim_set_interrupt







/* start : prim_set_res_ready
	name:
		prim_set_res_ready
	info:
		Calls result ready asm instruction to signal a matrix can be read out via IO path
	requirements hardware:
		none
	requirements variables/parameters locations:
		none

	uses:
	    label 10_310 - function start;
//*/
//*	// code


LB(10310);
	cRESREADY;      NOP;

	cHALT;			NOP;
//*/ end : prim_set_res_ready












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
	cSETVAL(0);																		NOP;
	cVLOAD(-1);																		NOP;
	cSETDEC;																		NOP;
	cVLOAD(0);																		NOP;

	LB(10_331);
		cWAITMATW(1);																	NOP;
		cBRCMPNValDEC(10_331,0);														NOP;

	cHALT;																			NOP;
//*/ // end : prim_wait_matrices











/* start : prim_cycle_counter_start_W_halt
	name:
		prim_cycle_counter_start_W_halt
	info:
		Starts the clock cycle counter for time measurements.
		Does not keep counting when in halt state.
	requiremens hardware:
		`define RESOURCE_FEATURE_TESTING (1)
	requirements variables/parameters locations:
		0 parameters:
		requires 0 matrices :
	uses:
	    label 10_360 - function start;
//*/
//*	// code

LB(10_360);
	cCC_START_W_HALT;																NOP;

	cHALT;																			NOP;
//*/ // end : prim_cycle_counter_start_W_halt





/* start : prim_cycle_counter_start_WO_halt
	name:
		prim_cycle_counter_start_WO_halt
	info:
		Starts the clock cycle counter for time measurements.
		Keeps counting even when in halt state.
	requiremens hardware:
		`define RESOURCE_FEATURE_TESTING (1)
	requirements variables/parameters locations:
		0 parameters:
		requires 0 matrices :
	uses:
	    label 10_361 - function start;
//*/
//*	// code

LB(10_361);
	cCC_START_WO_HALT;																NOP;

	cHALT;																			NOP;
//*/ // end : prim_cycle_counter_start_WO_halt







/* start : prim_cycle_counter_stop
	name:
		prim_cycle_counter_stop
	info:
		Stops the clock cycle counter for time measurements.
	requiremens hardware:
		`define RESOURCE_FEATURE_TESTING (1)
	requirements variables/parameters locations:
		0 parameters:
		requires 0 matrices :
	uses:
	    label 10_362 - function start;
//*/
//*	// code

LB(10_362);
	cCC_STOP;																		NOP;

	cHALT;																			NOP;
//*/ // end : prim_cycle_counter_stop






/* start : prim_cycle_counter_reset
		prim_cycle_counter_reset
	info:
		Resets the clock cycle counter for time measurements to value "0".
	requiremens hardware:
		`define RESOURCE_FEATURE_TESTING (1)
	requirements variables/parameters locations:
		0 parameters:
		requires 0 matrices :
	uses:
	    label 10_363 - function start;
//*/
//*	// code

LB(10_363);
	cCC_RESET;																		NOP;

	cHALT;																			NOP;
//*/ // end : prim_cycle_counter_reset






/* start : prim_delay_5
	name:
		prim_delay_5
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
	uses:
	    label 10_104 - function start;
//*/
//*	// code
LB(10104);
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cHALT;				NOP;
//*/ end : prim_delay_5




/* start : prim_temp_test_dma_hw
	name:
		prim_temp_test_dma_hw
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
	uses:
	    label 9000 - function start;
//*/
//*	// code
LB(9000);
	cNOP;				IXLOAD;
	cNOP;				RADD(0);
	cNOP;				RSTORE(0);
	cNOP;				RSTORE(2);
	cNOP;				RSTORE(3);
	cNOP;				NOP;
	cHALT;				NOP;
//*/ end : prim_temp_test_dma_hw


/* start : prim_temp_test_3
	name:
		prim_temp_test_3
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
	uses:
	    label 9000 - function start;
//*/
//*	// code
LB(9001);

	cVLOAD(995);		VLOAD(995);

	cVLOAD(1);									NOP;
	cSETDEC;									NOP;
	cVLOAD(1);          						SCANADD;
	cADDRSTORE;         						NOP;
	cVLOAD(15);         						NOP;
	cSTORE(17);          						NOP;
	cVLOAD(2*$clog2(`ARRAY_NR_CELLS)-1);    	VLOAD(1);

LB(9002);
	cBRNZDEC(9002);        						NOP;

	cVLOAD(42);         						NOP;
	cCRSTORE;           						NOP;



	cVLOAD(994);		VLOAD(994);

	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cHALT;				NOP;
//*/ end : prim_temp_test_3






/* start : prim_temp_fast_testing
	name:
		prin_temp_fast_testing
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
	uses:
	    label 9200 - function start;
//*/
//*	// code
LB(9200);
	cVLOAD(998);		VLOAD(998);
	cNOP;				NOP;
	cNOP;				NOP;


	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;

	cVLOAD(999);		VLOAD(999);

	cNOP;				NOP;
	cNOP;				NOP;
	cHALT;				NOP;
//*/ end : prin_temp_fast_testing




/*

	cNOP;				RLOAD(1);
	cNOP;				WHEREZERO;
	cNOP;				IXLOAD;
	cNOP;				ELSEWHERE;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cVLOAD(997);		VLOAD(997);
	cNOP;				SCANSPLIT(0);
	cNOP;				IXLOAD;
	cNOP;				VLOAD(0);
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				NOP;
	cNOP;				ENDWHERE;
	cNOP;				NOP;
	cNOP;				SCANLOAD;
	cNOP;				RSTORE(2);

	cNOP;				NOP;
	cNOP;				NOP;

//*/








































			cHALT;			NOP;




//* // for temp_test4
	LB(9);

			cPRUN(0);		NOP;	// pload/halt
			cPRUN(1);		NOP;	// initial stuff
			cPRUN(2);		NOP;	// prim_set_addr_regs

			SEND_MATRIX_ARRAY(0, 1, 16);
			cPRUN(5);		NOP;	// prim_wait_matrices

			cPRUN(11);		NOP;	// prim_temp_test_dma_hw

			cPRUN(3);		NOP;	// res ready
			GET_MATRIX_ARRAY(0, 1, 16, 1); // aici 1 ca sa am res ready mai deasupra

			cPRUN(4);		NOP;	// prim_set_interrupt

//*/


/* // for temp_test1

	LB(9);

			cPRUN(0);		NOP;	// halt. stops pload
			cPRUN(1);		NOP;	// initial stuff
			cPRUN(2);		NOP;	// prim_set_addr_regs

			cPRUN(12);		NOP;	// prim_temp_fast_testing


			cPRUN(4);		NOP;	// prim_set_interrupt

//*/




