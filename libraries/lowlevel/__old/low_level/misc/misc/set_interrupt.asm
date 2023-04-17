
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
	cSETINT;      	NOP;

	cHALT;			NOP;
//*/ end : prim_set_interrupt