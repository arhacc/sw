;-------------------------------------------------------------------------------------
;	name:
;		prim_temp_test_3
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	labels:
;	    label 1 - main for loop;
;-------------------------------------------------------------------------------------

include "configurations.asm"

func

	vload 995 									vload 995

	vload 1 									nop
	setdec										nop
	vload 1           							scanadd
	addrstore         							nop
	vload 15        	  						nop
	store 17           							nop
	vload $SCAN_NET_DEPTH						vload 1

label1:
	brnzdec 1         						nop

	vload 42          							nop
	rstore           							nop

	vload 994 									vload 994

	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	nop											nop
	halt										nop

endfunc

;-------------------------------------------------------------------------------------
