;-------------------------------------------------------------------------------------
;	name:
;		prim_temp_test_dma_hw
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	labels:
;       none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_temp_test_dma_hw

	nop					ixload
	nop					radd 0
	nop					rstore 0
	nop					rstore 2
	nop					rstore 3
	nop					nop
	halt				nop

endfunc

;-------------------------------------------------------------------------------------
