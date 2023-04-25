;-------------------------------------------------------------------------------------
;	name:
;		prim_wait_matrices
;	info:
;		used to call wait_matrix (cwaitmatw) asm instruction in order to make accelerator wait for io data
;	requiremens hardware:
;		none
;	requirements variables/parameters locations:
;		1 parameters, stored at :
;			code_prim_wait_matrices_nr_matrices_location : stored in ctrl memory : location for parameter storage
;		1 ram data, stored at :
;			code_prim_wait_matrices_loop_counter_location : stored in ctrl memory : main loop counter
;		requires 0 matrices :
;	labels:
;	    label 1 - main loop
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_wait_matrices

	param											nop
	rstore 0										nop	;// mem[] = addr_mat0

	vsub 1 											nop
	setval 0 										nop
	vload -1 										nop
	setdec											nop
	vload 0 										nop

	label1:
		waitmatw 1 									nop
		brcmpnvaldec label1,0 						nop

	halt											nop

endfunc
;-------------------------------------------------------------------------------------
