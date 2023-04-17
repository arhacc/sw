
/* start : prim_delay_prim_delay_medium
	name:
		prim_delay_medium 
	info:
		up to 10_000_000
	requiremens hardware:
		has controller stack
			controller stack 2 levels 
	requirements variables/parameters locations:
		param0: amount of delay stored at: CODE_PRIM_DELAY_AMOUNT_LOCATION
	uses:
	    label 10_007 - function start;
		label 10_008 - delay loop;
//*/ 
/*	// code
	`define CODE_PRIM_DELAY_AMOUNT_LOCATION (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	
LB(10007);
	cVSTACK_PUSH_LOAD(1);                                           NOP;
	cSETDEC;                                                        NOP;
	cPARAM;                                                         NOP;
	cRSTORE(`CODE_PRIM_DELAY_AMOUNT_LOCATION);                      NOP;
	cVSUB(6);														NOP;
	LB(10008);
        cBRNZDEC(10008);                                            NOP;
	cSTACK_POP;                                                     NOP;
//*/ end : prim_delay_prim_delay_medium