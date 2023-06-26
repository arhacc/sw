;-------------------------------------------------------------------------------------
;XPU kernel low level library
;	
;	initial:
;array mem[0] = [ 2 2 2 2 2 2 2 2 | 2 2 2 2 2 2 2 2  ]
;array mem[1] = [ 3 3 3 3 3 3 3 3 | 3 3 3 3 3 3 3 3  ]
;array mem[2] = [ 4 4 4 4 4 4 4 4 | 4 4 4 4 4 4 4 4  ]
;array mem[3] = [ 5 5 5 5 5 5 5 5 | 5 5 5 5 5 5 5 5  ]
;array mem[4] = [ 6 6 6 6 6 6 6 6 | 6 6 6 6 6 6 6 6  ]
;array mem[5] = [ 7 7 7 7 7 7 7 7 | 7 7 7 7 7 7 7 7  ]
;array mem[6] = [ 8 8 8 8 8 8 8 8 | 8 8 8 8 8 8 8 8  ]
;array mem[7] = [ 9 9 9 9 9 9 9 9 | 9 9 9 9 9 9 9 9  ]
;array mem[8] = [ 10 10 10 10 10 10 10 10 | 10 10 10 10 10 10 10 10  ]
;array mem[9] = [ 11 11 11 11 11 11 11 11 | 11 11 11 11 11 11 11 11  ]
;array mem[10] = [ 12 12 12 12 12 12 12 12 | 12 12 12 12 12 12 12 12  ]
;array mem[11] = [ 13 13 13 13 13 13 13 13 | 13 13 13 13 13 13 13 13  ]
;array mem[12] = [ 14 14 14 14 14 14 14 14 | 14 14 14 14 14 14 14 14  ]
;array mem[13] = [ 15 15 15 15 15 15 15 15 | 15 15 15 15 15 15 15 15  ]
;array mem[14] = [ 16 16 16 16 16 16 16 16 | 16 16 16 16 16 16 16 16  ]
;array mem[15] = [ 17 17 17 17 17 17 17 17 | 17 17 17 17 17 17 17 17  ]
;	result:
;array mem[16] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[17] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[18] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[19] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[20] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[21] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[22] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[23] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[24] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[25] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[26] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[27] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[28] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[29] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[30] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[31] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;-------------------------------------------------------------------------------------

func testForFullSquareMatrixTranspose
label0:
				nop				activate
				sel_addrreg 0	scannop
				
				generateFullSquareMatrix(-1, 2)
		
				cc_start_w_halt	nop
								
;matrixTranspose 
				nop				ixload
				nop				addrstore
				nop				store 32 ; read address: 2*$ARRAY_NR_CELLS
				nop				vadd 16
				vload -1		store 33 ; write address: 1+2*$ARRAY_NR_CELLS
				store 1     	rload 0
				vload 14		rstore 16
	;read
		label8:	store 0 		load 32		
				nop				vsub 1			;srstore		nop	
				nop				wherenegative 	;nop			nop
				nop				vadd 16 		;rotateright	nop
				nop				endwhere 		;nop			srload
				load 1			addrstore
				vadd 1			store 32
				store 1			rload 0
	;rotate		
				srstore         nop
				nop             nop
		label9:	rotateleft      nop
				brnzdec label9  nop
	;store
				nop				srload 
				nop				store 34
				nop				load 33
				srstore			nop
				nop				nop
				rotateleft		nop
				nop				srload
				nop				addrstore
				nop				store 33
				nop				load 34
				load 0			rstore 0
				
				brnzdec label8	nop
				
				
;end matrixTransppose cc = 577 = n^2 + 21n -15

				cc_stop			nop
				
				setint			nop
				halt			nop
endfunc

;-------------------------------------------------------------------------------------



