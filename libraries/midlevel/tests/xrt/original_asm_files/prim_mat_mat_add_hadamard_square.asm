
;-------------------------------------------------------------------------------------
;	name:
;		prim_mat_mat_add_hadamard_square
;	description:
;		computes hadamard (element wise) add between 2 square matrices of equal size
;		matrix size == array size
;		res = x + y;
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		3 parameters:
;			1) x location - location from array mem where first matrix is stored
;				stored at ctrl_mem[1023]
;			2) y location - location from array mem where second matrix is stored
;				stored at ctrl_mem[1022]
;			3) res location - location from array mem where result matrix is store
;				stored at ctrl_mem[1021]
;		2 variables:
;			ctrl mem:
;				difference of relative address between matrix 1 and matrix 0
;					stored at ctrl_mem[1020]
;				difference of relative address between matrix result and matrix 1
;					stored at ctrl_mem[1019]
;	labels:
;	    main_loop
;-------------------------------------------------------------------------------------



include "configurations.asm"


func prim_mat_mat_add_hadamard_square
param								addr_reg_stack_duplicate
rstore 1023 						nop		; mem[] = addr_mat0
param								nop
rstore 1022							nop		; mem[] = addr_mat1;
param								nop
rstore 1021							nop		; mem[] = addr_mat_dest;

rload 1022							nop
rsub 1023							nop
rstore 1020							nop		; mem[] = addr_mat1 - addr_mat0

rload 1021							nop
rsub 1022 							nop
rstore 1019							nop		; mem[] = addr_mat_dest - addr_mat1;

vload -1							nop
setdec								nop
vload $ARRAY_NR_CELLS_minus_one		nop
setval 0							nop
vload 0 							nop

main_loop:
	radd 1023							nop
	radd 1020							crload
	radd 1019							cradd
	rsub 1021							crstore
	brcmpnvaldec main_loop,0			nop
nop									addr_reg_stack_pop;

halt								nop

endfunc
















