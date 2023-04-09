

;/***************************************************************************************
;the program: normal testing from example file
;***************************************************************************************/
;;// the program used to initialize the accelerator
;;//*

func complete

	label0:
			pload 0				nop			;;// line not counted ( not saved in controller mem)
			halt				nop			;;// line 0

	label1:
			jmp label10_350 			nop ;// prim_initialize									;// line 1
			jmp label10_300 			nop ;// prim_set_addr_regs								;// line 2
			jmp label10_310 			nop; ;// prim_set_res_ready								;// line 3
			jmp label10_320 			nop; ;// prim_set_interrupt								;// line 4
			jmp label10_330 			nop; ;// prim_wait_matrices								;// line 5
			jmp label10_360 			nop; ;// prim_cycle_counter_start_w_halt				;// line 6
			jmp label10_361 			nop; ;// prim_cycle_counter_start_wo_halt				;// line 7
			jmp label10_362 			nop; ;// prim_cycle_counter_stop						;// line 8
			jmp label10_363 			nop; ;// prim_cycle_counter_reset						;// line 9
			jmp label10_104 			nop; ;// prim_delay_5									;// line 10
			jmp label9000 				nop; ;// prim_temp_test_dma_hw							;// line 11
			jmp label9200 				nop; ;// prim_temp_fast_testing							;// line 12
			jmp label9001 				nop; ;// prim_temp_test3								;// line 13












;/* start : prim_initialize
;	name:
;		prim_initialize
;	info:
;		initial configurations for xpu accelerator.
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;
;	uses:
;	    label 10_350 - function start;
;//*/
;//*	;// code

label10_350:
	nop					activate
	sel_addrreg 0  		nop
	nop					scannop
	halt				nop
;//*/ end : prim_initialize







;/* start : prim_set_addr_regs
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
;	uses:
;	    label 10_300 - function start;
;//*/
;//*	;// code


label10_300:
	param			nop
	param			nop
	addrstore		nop
	nop				cload
	nop				addrstore

	halt			nop
;//*/ end : prim_set_addr_regs









;/* start : prim_set_interrupt
;	name:
;		prim_set_interrupt
;	info:
;		calls set interrupt asm instruction to generate an interrupt towards the mid level
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;
;	uses:
;	    label 10_320 - function start;
;//*/
;//*	;// code


label10320:
	setint    	nop

	halt		nop
;//*/ end : prim_set_interrupt







;/* start : prim_set_res_ready
;	name:
;		prim_set_res_ready
;	info:
;		calls result ready asm instruction to signal a matrix can be read out via io path
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;
;	uses:
;	    label 10_310 - function start;
;//*/
;//*	;// code


label10310:
	resready      	nop

	halt			nop
;//*/ end : prim_set_res_ready












;/* start : prim_wait_matrices
;	name:
;		prim_wait_matrices
;	info:
;		used to call wait_matrix (cwaitmatw) asm instruction in order to make accelerator wait for io data
;	requiremens hardware:
;		none
;	requirements variables/parameters locations:
;		1 parameters, stored at :
;			code_prim_wait_matrices_nr_matrices_location : stored in ctrl memory : location for parameter storage
;		1 ram data, stored at :
;			code_prim_wait_matrices_loop_counter_location : stored in ctrl memory : main loop counter
;		requires 0 matrices :
;	uses:
;	    label 10_330 - function start;
;	    label 10_331 - main loop
;//*/
;//*	;// code

label10_330:
	param																			nop
	rstore 0											 							nop	;// mem[] = addr_mat0

	vsub 1 																			nop
	setval 0 																		nop
	vload -1 																		nop
	setdec																			nop
	vload 0 																		nop

	label10_331:
		waitmatw 1 																	nop
		brcmpnvaldec label10_331 0 													nop

	halt																			nop
;//*/ ;// end : prim_wait_matrices











;/* start : prim_cycle_counter_start_w_halt
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
;	uses:
;	    label 10_360 - function start;
;//*/
;//*	;// code

label10_360:
	cc_start_w_halt																	nop

	halt																			nop
;//*/ ;// end : prim_cycle_counter_start_w_halt





;/* start : prim_cycle_counter_start_wo_halt
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
;	uses:
;	    label 10_361 - function start;
;//*/
;//*	;// code

label10_361:
	cc_start_wo_halt																nop

	halt																			nop
;//*/ ;// end : prim_cycle_counter_start_wo_halt







;/* start : prim_cycle_counter_stop
;	name:
;		prim_cycle_counter_stop
;	info:
;		stops the clock cycle counter for time measurements.
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	uses:
;	    label 10_362 - function start;
;//*/
;//*	;// code

label10_362:
	cc_stop																			nop

	halt																			nop
;//*/ ;// end : prim_cycle_counter_stop






;/* start : prim_cycle_counter_reset
;		prim_cycle_counter_reset
;	info:
;		resets the clock cycle counter for time measurements to value "0".
;	requiremens hardware:
;		`define resource_feature_testing (1)
;	requirements variables/parameters locations:
;		0 parameters:
;		requires 0 matrices :
;	uses:
;	    label 10_363 - function start;
;//*/
;//*	;// code

label10_363:
	cc_reset																		nop

	halt																			nop
;//*/ ;// end : prim_cycle_counter_reset






;/* start : prim_delay_5
;	name:
;		prim_delay_5
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	uses:
;	    label 10_104 - function start;
;//*/
;//*	;// code
label10_104:
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	halt				nop
;//*/ end : prim_delay_5






;/* start : prim_temp_test_dma_hw
;	name:
;		prim_temp_test_dma_hw
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	uses:
;	    label 9000 - function start;
;//*/
;//*	;// code
label9000:
	nop					ixload
	nop					radd 0
	nop					rstore 0
	nop					rstore 2
	nop					rstore 3
	nop					nop
	halt				nop
;//*/ end : prim_temp_test_dma_hw






;/* start : prim_temp_test_3
;	name:
;		prim_temp_test_3
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	uses:
;	    label 9000 - function start;
;//*/
;//*	;// code
label9001:

	vload 995 									vload 995

	vload 1 									nop
	setdec										nop
	vload 1           							scanadd
	addrstore         							nop
	vload 15        	  						nop
	store 17           							nop
	vload 2*$clog2(`array_nr_cells)-1     		vload 1

label9002:
	brnzdec 9002         						nop

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
;//*/ end : prim_temp_test_3






;/* start : prim_temp_fast_testing
;	name:
;		prin_temp_fast_testing
;	requirements hardware:
;		none
;	requirements variables/parameters locations:
;		none
;	uses:
;	    label 9200 - function start;
;//*/
;//*	;// code
label9200:
	vload 998 			vload 998
	nop					nop
	nop					nop

	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop
	nop					nop

	vload 999 			vload 999

	nop					nop
	nop					nop
	halt				nop
;//*/ end : prin_temp_fast_testing








label9;

	prun 0				nop;	// pload/halt
	prun 1				nop;	// initial stuff
	prun 2 0 0			nop;	// prim_set_addr_regs

	send_matrix_array 0 1 16
	prun 5 1			nop;	// prim_wait_matrices

	prun 11				nop;	// prim_temp_test_dma_hw

	prun 3				nop;	// res ready
	get_matrix_array 0 1 16 1	 ;// aici 1 ca sa am res ready mai deasupra

	prun 4				nop;	// prim_set_interrupt







endfunc










