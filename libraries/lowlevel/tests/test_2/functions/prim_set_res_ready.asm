;-------------------------------------------------------------------------------------
;	name:
;		prim_set_res_ready
;	info:
;		calls result ready asm instruction to signal a matrix can be read out via io path
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	labels:
;	    none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_set_res_ready

	resready      	nop

	halt			nop

endfunc

;-------------------------------------------------------------------------------------
