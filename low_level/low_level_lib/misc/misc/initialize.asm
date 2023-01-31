
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
	cNOP;				SCANNOP(0);
	cHALT;				NOP;
//*/ end : prim_initialize