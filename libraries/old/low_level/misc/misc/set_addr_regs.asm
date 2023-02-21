
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
	
		
LB(10300);
	cPARAM;			NOP;
	cPARAM;			NOP;
	cADDRSTORE;		NOP;	
	cNOP;			CLOAD;
	cNOP;			ADDRSTORE;
	
	cHALT;			NOP;
//*/ end : prim_set_addr_regs