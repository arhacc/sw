

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





