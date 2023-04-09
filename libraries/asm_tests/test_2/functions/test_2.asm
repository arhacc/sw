




;-------------------------------------------------------------------------------------







;-------------------------------------------------------------------------------------
func test_2

	;load_tot(); intai se apeleaza acel load, apoi test_2

	prun prim_initial_load					nop;	// pload/halt
	prun prim_initialize					nop;	// initial stuff
	prun prim_set_addr_regs 0 0				nop;	// prim_set_addr_regs

	send_matrix_array 0 1 16
	prun prim_wait_matrices 1				nop;	// prim_wait_matrices

	prun prim_temp_test_dma_hw				nop;	// prim_temp_test_dma_hw

	prun prim_set_res_ready					nop;	// res ready
	get_matrix_array 0 1 16 1	 ;// aici 1 ca sa am res ready mai deasupra

	prun prim_set_interrupt				    nop;	// prim_set_interrupt

endfunc

;-------------------------------------------------------------------------------------
