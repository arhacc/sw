
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