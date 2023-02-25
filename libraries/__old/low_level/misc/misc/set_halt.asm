
/* start : prim_set_halt
	name:
		prim_set_halt
	info:
		Calls the halt asm instruction. To be used when needing to block program fifo for io sync.
	requirements hardware:
		none
	requirements variables/parameters locations:
		none
		
	uses:
	    label 10_340 - function start;
//*/
//*	// code
	
LB(10_340);
	cHALT;			NOP;
//*/ end : prim_set_halt