;-------------------------------------------------------------------------------------
;	name:
;		prim_initialize
;	info:
;		initial configurations for xpu accelerator.
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	labels:
;	    none
;-------------------------------------------------------------------------------------

func prim_initialize

	nop					activate
	sel_addrreg 0  		nop
	nop					scannop
	halt				nop

endfunc

;-------------------------------------------------------------------------------------
