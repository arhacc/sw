;-------------------------------------------------------------------------------------
;	name:
;		prim_mat_mat_add_hadamard_square
;	info:
;		Computes Hadamard (element wise) addition between 2 matrices that both take up the whole array.
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		3 parameters, stored at :
;			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT0_LOCATION : stored in ctrl memory : location from array mem where first matrix is stored
;			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT1_LOCATION : stored in ctrl memory : location from array mem where second matrix is stored
;			CODE_PRIM_MAT_MAT_ADD_HADAMARD_SQUARE_MAT_RES_LOCATION : stored in ctrl memory : location from array mem where result matrix is store
;		requires 2 matrices :
;			mat data in 0
;			mat data in 1
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_mat_mat_add_hadamard_square

	param					addr_reg_stack_duplicate
	rstore 0				nop;	 mem[] = addr_mat0
	param					nop
	rstore 1				nop;	// mem[] = addr_mat1;
	param					nop
	rstore 2				nop;	// mem[] = addr_mat_dest;

	rload 1					nop
	rsub 0					nop
	rstore 3				nop;	// mem[] = addr_mat1 - addr_mat0

	rload 2					nop
	rsub 1					nop
	rstore 4				nop;	// mem[] = addr_mat_dest - addr_mat1;

	vload -1				nop
	setdec					nop
	vload 15 				nop
	setval 0				nop
	vload 0					nop

label1:
		radd 0						nop
		radd 3						crload
		radd 4						cradd
		rsub 2						crstore
		brcmpnvaldec label1,0		nop

	nop								addr_reg_stack_pop

	halt							nop

endfunc

;-------------------------------------------------------------------------------------
