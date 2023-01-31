
/* start : prim_cycle_counter_start_W_halt
	name:
		prim_cycle_counter_start_W_halt
	info:
		Starts the clock cycle counter for time measurements.
		Does not keep counting even when in halt state.
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























