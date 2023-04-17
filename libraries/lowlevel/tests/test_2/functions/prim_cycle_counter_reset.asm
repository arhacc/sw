


;-------------------------------------------------------------------------------------

;		prim_cycle_counter_reset
;	info:
;		resets the clock cycle counter for time measurements to value "0".
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	labels:
;	    none

;-------------------------------------------------------------------------------------

func prim_cycle_counter_reset

	cc_reset																		nop

	halt																			nop

endfunc

;-------------------------------------------------------------------------------------
