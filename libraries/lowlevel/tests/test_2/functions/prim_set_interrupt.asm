;-------------------------------------------------------------------------------------
;	name:
;		prim_set_interrupt
;	info:
;		calls set interrupt asm instruction to generate an interrupt towards the mid level
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	labels:
;	    none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_set_interrupt

	setint    	nop

	halt		nop

endfunc


;-------------------------------------------------------------------------------------
