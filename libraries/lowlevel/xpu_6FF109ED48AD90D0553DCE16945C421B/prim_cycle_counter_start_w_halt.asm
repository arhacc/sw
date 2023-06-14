;-------------------------------------------------------------------------------------
;	name:
;		prim_cycle_counter_start_w_halt
;	info:
;		starts the clock cycle counter for time measurements.
;		does not keep counting when in halt state.
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	labels:
;	    none
;-------------------------------------------------------------------------------------

include "configurations.asm"

func prim_cycle_counter_start_w_halt

	cc_start_w_halt																	nop

	halt																			nop

endfunc

;-------------------------------------------------------------------------------------
