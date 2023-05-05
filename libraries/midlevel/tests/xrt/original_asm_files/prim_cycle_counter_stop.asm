;-------------------------------------------------------------------------------------
;	name:
;		prim_cycle_counter_stop
;	info:
;		stops the clock cycle counter for time measurements.
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	labels:
;	    none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_cycle_counter_stop

	cc_stop																			nop

	halt																			nop

endfunc

;-------------------------------------------------------------------------------------


