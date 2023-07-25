;-------------------------------------------------------------------------------------
;	name:
;		prim_fpga_test4
;	info:
;		test 4 for fpga with new architecture bitstream
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

func prim_fpga_test4

	nop				ixload
	nop				radd 0
	nop				rstore 0
	nop				ixload
	nop				radd 1
	nop				rstore 1
	nop				ixload
	nop				radd 2
	nop				rstore 2
	nop				ixload
	nop				radd 3
	nop				rstore 3

	halt			nop
endfunc

;-------------------------------------------------------------------------------------
