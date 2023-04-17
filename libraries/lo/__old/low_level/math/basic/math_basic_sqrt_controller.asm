/* start : prim_ctrl_sqrt_int2fixed_point16dot16
	name:
		prim_ctrl_sqrt_int2fixed_point16dot16
	info:
		Computes the square root of 32b integer from acc into 16.16 fixed point.
	requiremens hardware:
		min `define RESOURCE_CTRL_CELL_STACK_SIZE (3)
		`define DATA_SIZE (32) 
	requirements variables/parameters locations:
		no parameters
		3 ram variaables, stored at: 
			CODE_MATH_SQRT_CTRL_RAM_LOC_a
			CODE_MATH_SQRT_CTRL_RAM_LOC_b
			CODE_MATH_SQRT_CTRL_RAM_LOC_c
	uses:
	    label 10_500 - function start;
	    label 10_501 - if not 0 - result !=0, do computations
	    label 10_502 - function end
	    label 10_503 - main for loop
	    label 10_504 - final if
	    label 10_505 - if within loop
	    label 10_506 - additional if condition , if result == c go to add +1
//*/ 
//*	// code			

	`define CODE_MATH_SQRT_CTRL_RAM_LOC_a (`CONTROLLER_INSTR_MEM_SIZE/2 -1)
	`define CODE_MATH_SQRT_CTRL_RAM_LOC_b (`CODE_MATH_SQRT_CTRL_RAM_LOC_a -1)
	`define CODE_MATH_SQRT_CTRL_RAM_LOC_c (`CODE_MATH_SQRT_CTRL_RAM_LOC_b -1) 

LB(10_500);
	cVADD(0);																NOP;
	cBRNZ(10_501);															NOP;
	cJMP(10_502);															NOP;
LB(10_501);																	
	cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
	cVLOAD(1);																NOP;
	cSHIFT_LEFT(30);														NOP;
	cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_b);								NOP;
	cVLOAD(0);																NOP;
	cSTACK_DUPLICATE;														NOP;
	
	LB(10_503);
		cSTACK_POP;																NOP;
		cSTACK_DUPLICATE;														NOP;
		cSTACK_DUPLICATE;														NOP;
		cRLOAD(`CODE_MATH_SQRT_CTRL_RAM_LOC_b);									NOP;
		cSADD;																	NOP;
		cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_a);								NOP;
		cRRVSUB(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
		cBRSGN(10_505);															NOP;
		cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
		cRLOAD(`CODE_MATH_SQRT_CTRL_RAM_LOC_a);									NOP;
		cRADD(`CODE_MATH_SQRT_CTRL_RAM_LOC_b);									NOP;
		cVSTACK_PUSH_LOAD(0);													NOP;
	LB(10_505);
		cRLOAD(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);									NOP;
		cSHIFT_LEFT(1);															NOP;
		cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
		cRLOAD(`CODE_MATH_SQRT_CTRL_RAM_LOC_b);									NOP;
		cSHIFT_RIGHT(1);														NOP;
		cRSTORE(`CODE_MATH_SQRT_CTRL_RAM_LOC_b);								NOP;
		cBRZ(10_504);															NOP;
		cBRSGN(10_504);															NOP;
		cJMP(10_503);															NOP;
		
LB(10_504);	
	cSTACK_POP;																NOP;
	cRCOMPARE(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
	cBRZ(10_506);															NOP;
	cRCOMPARE(`CODE_MATH_SQRT_CTRL_RAM_LOC_c);								NOP;
	cBRNSGN(10_502);														NOP;
LB(10_506);	
	cVADD(1);																NOP;
LB(10_502);
	
	cNOP;																	NOP;
	cSEL_ADDRREG(0);														NOP;
	cHALT;																	NOP;
//*/ // end : prim_ctrl_sqrt_int2fixed_point16dot16


































