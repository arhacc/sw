

;-------------------------------------------------------------------------------------

;	name:
;		prim_cycle_counter_start_wo_halt
;	info:
;		starts the clock cycle counter for time measurements.
;		keeps counting even when in halt state.
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	labels:
;	    none

;-------------------------------------------------------------------------------------

func prim_cycle_counter_start_wo_halt

	cc_start_wo_halt																nop

	halt																			nop

endfunc

;-------------------------------------------------------------------------------------
