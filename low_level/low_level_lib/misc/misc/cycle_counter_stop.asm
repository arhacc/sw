

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


