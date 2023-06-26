;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;minor test implementation
; binary form at: 
;	xpo\iio_simulation_data\program_file\program_file_16_2_2_temp_test1.txt
;
;vector:
;array mem[0] = [ 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1  ]
;
;matrix:
;array mem[1] = [ 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15  ]
;array mem[2] = [ 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  ]
;array mem[3] = [ 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17  ]
;array mem[4] = [ 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18  ]
;array mem[5] = [ 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19  ]
;array mem[6] = [ 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20  ]
;array mem[7] = [ 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21  ]
;array mem[8] = [ 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22  ]
;array mem[9] = [ 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  ]
;array mem[10] = [ 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  ]
;array mem[11] = [ 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25  ]
;array mem[12] = [ 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26  ]
;array mem[13] = [ 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27  ]
;array mem[14] = [ 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  ]
;array mem[15] = [ 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29  ]
;array mem[16] = [ 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30  ]
;
; result:
; ACC = [ 120 136 152 168 184 200 216 232 248 264 280 296 312 328 344 360  ]
;
; cc = 39 = 2x16 + 7
;-------------------------------------------------------------------------------------

func testForMatrixVectorMultiply
label0:
				nop				activate
				sel_addrreg 0	scannop
				vload 0 		vload 0
				addrstore		addrstore	
				
		;generateSquareMatrix
				vload 16		ixload
		label1:	vsub 1			ristore 1
				brnz label1		vadd 1
				
		;loadVeector
				nop				vload 1
				nop				store 0
				
				cc_start_w_halt	nop
				
		;matrixVectorMultiply
				nop				scanadd
				nop				riload 0
				vload 16		mult 0
		label2:	left_redins     riload -1
				brnzdec label2	mult 0 
				left_redins		nop
				nop				srload
				
				cc_stop			nop
				
				setint			nop
				halt			nop
endfunc

;-------------------------------------------------------------------------------------



