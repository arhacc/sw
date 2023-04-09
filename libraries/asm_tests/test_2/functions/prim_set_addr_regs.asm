;-------------------------------------------------------------------------------------
;	name:
;		prim_set_addr_regs
;	info:
;		set addr reg for both controller and array.
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	2 parameters:
;		array address
;		controller address
;	labels:
;	    none
;-------------------------------------------------------------------------------------

func prim_set_addr_regs

	param			nop
	param			nop
	addrstore		nop
	nop				cload
	nop				addrstore

	halt			nop

endfunc

;-------------------------------------------------------------------------------------
