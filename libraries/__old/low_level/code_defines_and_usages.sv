

/*
	IMPORTANT information regarding misc usage between primitives


	
	word meanings:
		vertical	
			data is stored such as all operations are done in one cell 
			(all data for 1 computation stored in array.cell[i].mem[0:1024])
		horizontal
			data is stored such that they take one mem location in each cell
			(all data for 1 computation stored in array.cell[0:n].mem[0])
		rectangle 
			data is stored such that they take multiple mem locations in each cell
				(all data for 1 computation stored in array.cell[0:n].mem[0:x])
				
				
	labels: (used in jumps)
		16b label in "compiler" => max label 64k 

	used in:
	
		misc:
			set_addr_regs.asm
				label 10_300 - prim_set_addr_regs function start;
			set_res_ready.asm
				label 10_310 - prim_set_res_ready function start;
			set_interrupt.asm
				label 10_320 - prim_set_interrupt function start;
			set_wait_matrices.asm
				label 10_330 - prim_wait_matrices function start;
			set_halt.asm
				label 10_340 - prim_set_halt function start;
			initialize.asm
				label 10_350 - prim_initialize function start;
			cycle_counter_start_W_halt.asm
				label 10_360 - prim_cycle_counter_start_W_halt function start;
			cycle_counter_start_WO_halt.asm
				label 10_361 - prim_cycle_counter_start_WO_halt function start;
			cycle_counter_stop.asm
				label 10_362 - prim_cycle_counter_stop function start;
			cycle_counter_reset.asm
				label 10_363 - prim_cycle_counter_reset function start;
				
	
		delay:
			delay_small.asm
				label 10_100 - prim_delay_1 function_start
				label 10_101 - prim_delay_2 function_start
				label 10_102 - prim_delay_3 function_start
				label 10_103 - prim_delay_4 function_start
				label 10_104 - prim_delay_5 function_start
				label 10_105 - prim_delay_6 function_start
				label 10_106 - prim_delay_7 function_start
			delay_medium.asm
				label 10_107 - prim_delay_medium function_start
					label 10_108 - prim_delay_medium delay loop
		
		
		
		
		data_movement
			ctrl_acc_ctrl_mem.asm
				label 10_200 - prim_copy_ctrl_acc_ctrl_mem function start;	
			ctrl_mem_ctrl_acc.asm
				label 10_201 - prim_copy_ctrl_mem_ctrl_acc function start;
			ctrl_acc_array_acc.asm
				label 10_202 - prim_copy_ctrl_acc_array_acc function start;
			ctrl_acc_array_mem.asm
				label 10_203 - prim_copy_ctrl_acc_array_mem function start;
			ctrl_mem_array_acc_v1.asm
				label 10_204 - prim_copy_ctrl_mem_array_acc_v1 function start;
			ctrl_mem_array_acc_v2.asm
				label 10_205 - prim_copy_ctrl_mem_array_acc_v2 function start;
				label 10_206 - prim_copy_ctrl_mem_array_acc_v2; add data to global shift register loop;
			ctrl_mem_array_mem_v1.asm
				label 10_207 - prim_copy_ctrl_mem_array_mem_v1 function start;
			ctrl_mem_array_mem_v2.asm
				label 10_208 - prim_copy_ctrl_mem_array_mem_v2 function start;
				label 10_209 - prim_copy_ctrl_mem_array_mem_v2 global data shift register load loop.
			prim_reverse_ctrl_mem_data.asm
				label 10_210 - prim_reverse_ctrl_mem_data function start;
				label 10_211 - prim_reverse_ctrl_mem_data data movement loop.
			prim_reverse_array_mem_data.asm
				label 10_212 - prim_reverse_array_mem_data function start;
				label 10_213 - prim_reverse_array_mem_data data movement loop.
			prim_copy_array_mem_array_mem
				label 10_214 - prim_copy_array_mem_array_mem function start;
				label 10_215 - prim_copy_array_mem_array_mem main loop
			prim_copy_ctrl_mem_ctrl_mem
				label 10_216 - prim_copy_ctrl_mem_ctrl_mem function start;
				label 10_217 - prim_copy_ctrl_mem_ctrl_mem main loop
		
			prim_vec_transpose_horiz2vert
				label 10_218 - prim_vec_transpose_horiz2vert function start;
				label 10_219 - prim_vec_transpose_horiz2vert for loop - right shift (lower part of vector) 
				label 10_220 - prim_vec_transpose_horiz2vert for loop - left shift (upper part of vector)
				label 10_221 - prim_vec_transpose_horiz2vert skip for loop if at cell is at one end;
		
			prim_vec_transpose_vert2horiz
				label 10_222 - prim_vec_transpose_vert2horiz function start;
				label 10_223 - prim_vec_transpose_vert2horiz for loop - left shift 
				label 10_224 - prim_vec_transpose_vert2horiz for loop - right shift 
				label 10_225 - prim_vec_transpose_vert2horiz skip for loop if at cell is at one end
				label 10_226 - prim_vec_transpose_vert2horiz skip for loop if at cell is at one end
				
			prim_mat_transpose_square
				label 10_227 - prim_mat_transpose_square function start;
				label 10_228 - prim_mat_transpose_square main loop;
				
			prim_mat_transpose_squares_small	
				label 10_229 - prim_mat_transpose_square function start;
				label 10_230 - prim_mat_transpose_square main loop 
		
		
		
		
				
		math
			basic
				prim_math_basic_sqrt_controller
					label 10_500 - prim_math_basic_sqrt_controller function start
					label 10_501 - prim_math_basic_sqrt_controller if not 0 - result !=0, do computations
					label 10_502 - prim_math_basic_sqrt_controller function end
					label 10_503 - prim_math_basic_sqrt_controller main for loop
					label 10_504 - prim_math_basic_sqrt_controller final if
					label 10_505 - prim_math_basic_sqrt_controller if within loop
					label 10_506 - prim_math_basic_sqrt_controller additional if condition , if result == c go to add +1
										
			dense_linear_algebra	
				mat_mat
					prim_mat_mat_add_hadamard_square
						label 11_000 - prim_mat_mat_add_hadamard_square function start
						label 11_001 - prim_mat_mat_add_hadamard_square main loop
					prim_mat_mat_mult_hadamard_square
						label 11_002 - prim_mat_mat_mult_hadamard_square function start
						label 11_003 - prim_mat_mat_mult_hadamard_square main loop
					prim_mat_mat_sub_hadamard_square
						label 11_004 - prim_mat_mat_sub_hadamard_square function start
						label 11_005 - prim_mat_mat_sub_hadamard_square main loop
					
					prim_mat_mat_add_hadamard_rectangle
						label 11_006 - prim_mat_mat_add_hadamard_rectangle function start
						label 11_007 - prim_mat_mat_add_hadamard_rectangle main loop
					prim_mat_mat_mult_hadamard_rectangle
						label 11_008 - prim_mat_mat_mult_hadamard_rectangle function start
						label 11_009 - prim_mat_mat_mult_hadamard_rectangle main loop
					prim_mat_mat_sub_hadamard_rectangle
						label 11_010 - prim_mat_mat_sub_hadamard_rectangle function start
						label 11_011 - prim_mat_mat_sub_hadamard_rectangle main loop
					
					
					prim_mat_mat_mult_dot_square
						label 11_012 - prim_mat_mat_mult_dot_square function start;
						label 11_013 - prim_mat_mat_mult_dot_square main loop 
						label 11_014 - prim_mat_mat_mult_dot_square function end
					
					prim_mat_mat_mult_dot_square
						label 11_015 - prim_mat_mat_mult_dot_accumulate_square function start;
						label 11_016 - prim_mat_mat_mult_dot_accumulate_square main loop 
						label 11_017 - prim_mat_mat_mult_dot_accumulate_square function end
					
















				
					
	// deprecated below line ::::::::::::::::::::::::::::::				
					
					
		vec-vec operations		
				label 11_000 - prim_vec_vec_add_vertical_16cells function start
				label 11_000 - prim_vec_vec_add_vertical_32cells function start
				label 11_000 - prim_vec_vec_add_vertical_64cells function start
				label 11_000 - prim_vec_vec_add_vertical_128cells function start
				label 11_000 - prim_vec_vec_add_vertical_256cells function start
				label 11_001 - prim_vec_vec_add_vertical_16cells for loop 
				label 11_001 - prim_vec_vec_add_vertical_32cells for loop 
				label 11_001 - prim_vec_vec_add_vertical_64cells for loop 
				label 11_001 - prim_vec_vec_add_vertical_128cells for loop 
				label 11_001 - prim_vec_vec_add_vertical_256cells for loop 
			     
			    label 11_100 - prim_vec_vec_add_horizontal_16cells function start
			    label 11_100 - prim_vec_vec_add_horizontal_32cells function start
			    label 11_100 - prim_vec_vec_add_horizontal_64cells function start
			    label 11_100 - prim_vec_vec_add_horizontal_128cells function start
			    label 11_100 - prim_vec_vec_add_horizontal_256cells function start

                label 11_200 - prim_vec_vec_add_rectangle_16cells function start
                label 11_200 - prim_vec_vec_add_rectangle_32cells function start
                label 11_200 - prim_vec_vec_add_rectangle_64cells function start
                label 11_200 - prim_vec_vec_add_rectangle_128cells function start
                label 11_200 - prim_vec_vec_add_rectangle_256cells function start
                label 11_201 - prim_vec_vec_add_rectangle_16cells main for loop
                label 11_201 - prim_vec_vec_add_rectangle_32cells main for loop
                label 11_201 - prim_vec_vec_add_rectangle_64cells main for loop
                label 11_201 - prim_vec_vec_add_rectangle_128cells main for loop
                label 11_201 - prim_vec_vec_add_rectangle_256cells main for loop
                
                label 11_300 - prim_vec_vec_mult_scalar_vertical_16cells function start;
                label 11_300 - prim_vec_vec_mult_scalar_vertical_32cells function start;
                label 11_300 - prim_vec_vec_mult_scalar_vertical_64cells function start;
                label 11_300 - prim_vec_vec_mult_scalar_vertical_128cells function start;
                label 11_300 - prim_vec_vec_mult_scalar_vertical_256cells function start;
		        label 11_301 - prim_vec_vec_mult_scalar_vertical_16cells main for loop;
		        label 11_301 - prim_vec_vec_mult_scalar_vertical_32cells main for loop;
		        label 11_301 - prim_vec_vec_mult_scalar_vertical_64cells main for loop;
		        label 11_301 - prim_vec_vec_mult_scalar_vertical_128cells main for loop;
		        label 11_301 - prim_vec_vec_mult_scalar_vertical_256cells main for loop;
                
                label 11_400 - prim_vec_vec_mult_scalar_horizontal_16cells function start;
                label 11_400 - prim_vec_vec_mult_scalar_horizontal_32cells function start;
                label 11_400 - prim_vec_vec_mult_scalar_horizontal_64cells function start;
                label 11_400 - prim_vec_vec_mult_scalar_horizontal_128cells function start;
                label 11_400 - prim_vec_vec_mult_scalar_horizontal_256cells function start;
                
                label 11_500 - prim_vec_vec_mult_scalar_rectangle_16cells function start;
                label 11_500 - prim_vec_vec_mult_scalar_rectangle_32cells function start;
                label 11_500 - prim_vec_vec_mult_scalar_rectangle_64cells function start;
                label 11_500 - prim_vec_vec_mult_scalar_rectangle_128cells function start;
                label 11_500 - prim_vec_vec_mult_scalar_rectangle_256cells function start;
                label 11_501 - prim_vec_vec_mult_scalar_rectangle_16cells main for loop;
                label 11_501 - prim_vec_vec_mult_scalar_rectangle_32cells main for loop;
                label 11_501 - prim_vec_vec_mult_scalar_rectangle_64cells main for loop;
                label 11_501 - prim_vec_vec_mult_scalar_rectangle_128cells main for loop;
                label 11_501 - prim_vec_vec_mult_scalar_rectangle_256cells main for loop;
                
                label 11_600 - prim_vec_reduce_add_vertical_16cells (_v1 / _v2) function start;
                label 11_600 - prim_vec_reduce_add_vertical_32cells (_v1 / _v2) function start;
                label 11_600 - prim_vec_reduce_add_vertical_64cells (_v1 / _v2) function start;
                label 11_600 - prim_vec_reduce_add_vertical_128cells (_v1 / _v2) function start;
                label 11_600 - prim_vec_reduce_add_vertical_256cells (_v1 / _v2) function start;
                label 11_601 - prim_vec_reduce_add_vertical_16cells (_v1 / _v2) main for loop;
                label 11_601 - prim_vec_reduce_add_vertical_32cells (_v1 / _v2) main for loop;
                label 11_601 - prim_vec_reduce_add_vertical_64cells (_v1 / _v2) main for loop;
                label 11_601 - prim_vec_reduce_add_vertical_128cells (_v1 / _v2) main for loop;
                label 11_601 - prim_vec_reduce_add_vertical_256cells (_v1 / _v2) main for loop;
                
                label 11_700 - prim_vec_reduce_add_vertical_16cells (_v1 / _v2) function start;
				label 11_700 - prim_vec_reduce_add_vertical_32cells (_v1 / _v2) function start;
				label 11_700 - prim_vec_reduce_add_vertical_64cells (_v1 / _v2) function start;
				label 11_700 - prim_vec_reduce_add_vertical_128cells (_v1 / _v2) function start;
				label 11_700 - prim_vec_reduce_add_vertical_256cells (_v1 / _v2) function start;
                
                label 11_800 - prim_vec_reduce_add_horizontal_manysmall_16cells function start 
                label 11_800 - prim_vec_reduce_add_horizontal_manysmall_32cells function start 
                label 11_800 - prim_vec_reduce_add_horizontal_manysmall_64cells function start 
                label 11_800 - prim_vec_reduce_add_horizontal_manysmall_128cells function start 
                label 11_800 - prim_vec_reduce_add_horizontal_manysmall_256cells function start 
                label 11_801 - prim_vec_reduce_add_horizontal_manysmall_16cells main for loop; global data shifting;
                label 11_801 - prim_vec_reduce_add_horizontal_manysmall_32cells main for loop; global data shifting;
                label 11_801 - prim_vec_reduce_add_horizontal_manysmall_64cells main for loop; global data shifting;
                label 11_801 - prim_vec_reduce_add_horizontal_manysmall_128cells main for loop; global data shifting;
                label 11_801 - prim_vec_reduce_add_horizontal_manysmall_256cells main for loop; global data shifting;
                
                label 11_900 - prim_vec_reduce_mult_vertical_16cells_ (_v1 / _v2) function start;
				label 11_900 - prim_vec_reduce_mult_vertical_32cells_ (_v1 / _v2) function start;
				label 11_900 - prim_vec_reduce_mult_vertical_64cells_ (_v1 / _v2) function start;
				label 11_900 - prim_vec_reduce_mult_vertical_128cells_ (_v1 / _v2) function start;
				label 11_900 - prim_vec_reduce_mult_vertical_256cells_ (_v1 / _v2) function start;
				label 11_901 - prim_vec_reduce_mult_vertical_16cells_ (_v1 / _v2) main for loop;
				label 11_901 - prim_vec_reduce_mult_vertical_32cells_ (_v1 / _v2) main for loop;
				label 11_901 - prim_vec_reduce_mult_vertical_64cells_ (_v1 / _v2) main for loop;
				label 11_901 - prim_vec_reduce_mult_vertical_128cells_ (_v1 / _v2) main for loop;
				label 11_901 - prim_vec_reduce_mult_vertical_256cells_ (_v1 / _v2) main for loop;
				
				label 12_000 - prim_vec_scalar_add_ctrl_val_vertical_16cells_v1 function start;
				label 12_000 - prim_vec_scalar_add_ctrl_val_vertical_32cells_v1 function start;
				label 12_000 - prim_vec_scalar_add_ctrl_val_vertical_64cells_v1 function start;
				label 12_000 - prim_vec_scalar_add_ctrl_val_vertical_128cells_v1 function start;
				label 12_000 - prim_vec_scalar_add_ctrl_val_vertical_256cells_v1 function start;
				label 12_001 - prim_vec_scalar_add_ctrl_val_vertical_16cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_add_ctrl_val_vertical_32cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_add_ctrl_val_vertical_64cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_add_ctrl_val_vertical_128cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_add_ctrl_val_vertical_256cells_v1 main for loop;
				
				label 12_002 - prim_vec_scalar_add_ctrl_val_vertical_16cells_v2 function start;
				label 12_002 - prim_vec_scalar_add_ctrl_val_vertical_32cells_v2 function start;
				label 12_002 - prim_vec_scalar_add_ctrl_val_vertical_64cells_v2 function start;
				label 12_002 - prim_vec_scalar_add_ctrl_val_vertical_128cells_v2 function start;
				label 12_002 - prim_vec_scalar_add_ctrl_val_vertical_256cells_v2 function start;
				label 12_003 - prim_vec_scalar_add_ctrl_val_vertical_16cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_add_ctrl_val_vertical_32cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_add_ctrl_val_vertical_64cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_add_ctrl_val_vertical_128cells_v2 main for loop; 
				label 12_003 - prim_vec_scalar_add_ctrl_val_vertical_256cells_v2 main for loop; 
				
				label 12_000 - prim_vec_scalar_mult_ctrl_val_vertical_16cells_v1 function start;
				label 12_000 - prim_vec_scalar_mult_ctrl_val_vertical_32cells_v1 function start;
				label 12_000 - prim_vec_scalar_mult_ctrl_val_vertical_64cells_v1 function start;
				label 12_000 - prim_vec_scalar_mult_ctrl_val_vertical_128cells_v1 function start;
				label 12_000 - prim_vec_scalar_mult_ctrl_val_vertical_256cells_v1 function start;
				label 12_001 - prim_vec_scalar_mult_ctrl_val_vertical_16cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_mult_ctrl_val_vertical_32cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_mult_ctrl_val_vertical_64cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_mult_ctrl_val_vertical_128cells_v1 main for loop;
				label 12_001 - prim_vec_scalar_mult_ctrl_val_vertical_256cells_v1 main for loop;
				
				label 12_002 - prim_vec_scalar_mult_ctrl_val_vertical_16cells_v2 function start;
				label 12_002 - prim_vec_scalar_mult_ctrl_val_vertical_32cells_v2 function start;
				label 12_002 - prim_vec_scalar_mult_ctrl_val_vertical_64cells_v2 function start;
				label 12_002 - prim_vec_scalar_mult_ctrl_val_vertical_128cells_v2 function start;
				label 12_002 - prim_vec_scalar_mult_ctrl_val_vertical_256cells_v2 function start;
				label 12_003 - prim_vec_scalar_mult_ctrl_val_vertical_16cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_mult_ctrl_val_vertical_32cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_mult_ctrl_val_vertical_64cells_v2 main for loop;
				label 12_003 - prim_vec_scalar_mult_ctrl_val_vertical_128cells_v2 main for loop; 
				label 12_003 - prim_vec_scalar_mult_ctrl_val_vertical_256cells_v2 main for loop; 
				
				label 12_100 - prim_vec_scalar_add_array_val_vertical_16cells_v1 function start;
				label 12_100 - prim_vec_scalar_add_array_val_vertical_32cells_v1 function start;
				label 12_100 - prim_vec_scalar_add_array_val_vertical_64cells_v1 function start;
				label 12_100 - prim_vec_scalar_add_array_val_vertical_128cells_v1 function start;
				label 12_100 - prim_vec_scalar_add_array_val_vertical_256cells_v1 function start;
				label 12_101 - prim_vec_scalar_add_array_val_vertical_16cells_v1 main for loop;
				label 12_101 - prim_vec_scalar_add_array_val_vertical_32cells_v1 main for loop;
				label 12_101 - prim_vec_scalar_add_array_val_vertical_64cells_v1 main for loop;
				label 12_101 - prim_vec_scalar_add_array_val_vertical_128cells_v1 main for loop;
				label 12_101 - prim_vec_scalar_add_array_val_vertical_256cells_v1 main for loop;
				
				label 12_102 - prim_vec_scalar_add_array_val_vertical_16cells_v2 function start;
				label 12_102 - prim_vec_scalar_add_array_val_vertical_32cells_v2 function start;
				label 12_102 - prim_vec_scalar_add_array_val_vertical_64cells_v2 function start;
				label 12_102 - prim_vec_scalar_add_array_val_vertical_128cells_v2 function start;
				label 12_102 - prim_vec_scalar_add_array_val_vertical_256cells_v2 function start;
				label 12_103 - prim_vec_scalar_add_array_val_vertical_16cells_v2main for loop;
				label 12_103 - prim_vec_scalar_add_array_val_vertical_32cells_v2main for loop;
				label 12_103 - prim_vec_scalar_add_array_val_vertical_64cells_v2main for loop;
				label 12_103 - prim_vec_scalar_add_array_val_vertical_128cells_v2main for loop;
				label 12_103 - prim_vec_scalar_add_array_val_vertical_256cells_v2main for loop;
		
				label 12_200 - prim_vec_scalar_mult_array_val_vertical_16cells_v1 function start;
				label 12_200 - prim_vec_scalar_mult_array_val_vertical_32cells_v1 function start;
				label 12_200 - prim_vec_scalar_mult_array_val_vertical_64cells_v1 function start;
				label 12_200 - prim_vec_scalar_mult_array_val_vertical_128cells_v1 function start;
				label 12_200 - prim_vec_scalar_mult_array_val_vertical_256cells_v1 function start;
				label 12_201 - prim_vec_scalar_mult_array_val_vertical_16cells_v1 main for loop;
				label 12_201 - prim_vec_scalar_mult_array_val_vertical_32cells_v1 main for loop;
				label 12_201 - prim_vec_scalar_mult_array_val_vertical_64cells_v1 main for loop;
				label 12_201 - prim_vec_scalar_mult_array_val_vertical_128cells_v1 main for loop;
				label 12_201 - prim_vec_scalar_mult_array_val_vertical_256cells_v1 main for loop;
				
				label 12_202 - prim_vec_scalar_mult_array_val_vertical_16cells_v2 function start;
				label 12_202 - prim_vec_scalar_mult_array_val_vertical_32cells_v2 function start;
				label 12_202 - prim_vec_scalar_mult_array_val_vertical_64cells_v2 function start;
				label 12_202 - prim_vec_scalar_mult_array_val_vertical_128cells_v2 function start;
				label 12_202 - prim_vec_scalar_mult_array_val_vertical_256cells_v2 function start;
				label 12_203 - prim_vec_scalar_mult_array_val_vertical_16cells_v2 main for loop;
				label 12_203 - prim_vec_scalar_mult_array_val_vertical_32cells_v2 main for loop;
				label 12_203 - prim_vec_scalar_mult_array_val_vertical_64cells_v2 main for loop;
				label 12_203 - prim_vec_scalar_mult_array_val_vertical_128cells_v2 main for loop;
				label 12_203 - prim_vec_scalar_mult_array_val_vertical_256cells_v2 main for loop;
				
				label 12_300 - prim_vec_transpose_horizontal_2_vertical_16cells function start;
				label 12_301 - prim_vec_transpose_horizontal_2_vertical_16cells for loop - right shift (lower part of vector) 
				label 12_302 - prim_vec_transpose_horizontal_2_vertical_16cells for loop - left shift (upper part of vector)
				label 12_304 - prim_vec_transpose_horizontal_2_vertical_16cells skip for loop if at cell is at one end
				
				label 12_310 - prim_vec_transpose_vertical_2_horizontal_16cells function start;
				label 12_311 - prim_vec_transpose_vertical_2_horizontal_32cells for loop - left shift 
				label 12_312 - prim_vec_transpose_vertical_2_horizontal_64cells for loop - right shift 
				label 12_313 - prim_vec_transpose_vertical_2_horizontal_128cells skip for loop if at cell is at one end
				label 12_314 - prim_vec_transpose_vertical_2_horizontal_256cells skip for loop if at cell is at one end
		
		
				
				
//*/



















