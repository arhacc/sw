
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



