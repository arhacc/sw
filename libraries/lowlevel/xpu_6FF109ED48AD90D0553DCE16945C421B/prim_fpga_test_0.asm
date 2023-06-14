;-------------------------------------------------------------------------------------
;	name:
;		prim_fpga_test0
;	info:
;		test 0 for fpga with new architecture bitstream
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	parameters:
;		none
;	labels:
;	    none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_fpga_test0

	vload 52		nop
	nop				nop

	halt			nop

endfunc

;-------------------------------------------------------------------------------------
