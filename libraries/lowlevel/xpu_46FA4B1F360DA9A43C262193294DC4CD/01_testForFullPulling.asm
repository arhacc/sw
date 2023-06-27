;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;array mem[0] = [ 0 1 2 3 4 5 6 7 | 8 9 10 11 12 13 14 15  ]
;array mem[1] = [ 1 2 3 4 5 6 7 8 | 9 10 11 12 13 14 15 16  ]
;array mem[2] = [ 2 3 4 5 6 7 8 9 | 10 11 12 13 14 15 16 17  ]
;array mem[3] = [ 3 4 5 6 7 8 9 10 | 11 12 13 14 15 16 17 18  ]
;array mem[4] = [ 4 5 6 7 8 9 10 11 | 12 13 14 15 16 17 18 19  ]
;array mem[5] = [ 5 6 7 8 9 10 11 12 | 13 14 15 16 17 18 19 20  ]
;array mem[6] = [ 6 7 8 9 10 11 12 13 | 14 15 16 17 18 19 20 21  ]
;array mem[7] = [ 7 8 9 10 11 12 13 14 | 15 16 17 18 19 20 21 22  ]
;array mem[8] = [ 8 9 10 11 12 13 14 15 | 16 17 18 19 20 21 22 23  ]
;array mem[9] = [ 9 10 11 12 13 14 15 16 | 17 18 19 20 21 22 23 24  ]
;array mem[10] = [ 10 11 12 13 14 15 16 17 | 18 19 20 21 22 23 24 25  ]
;array mem[11] = [ 11 12 13 14 15 16 17 18 | 19 20 21 22 23 24 25 26  ]
;array mem[12] = [ 12 13 14 15 16 17 18 19 | 20 21 22 23 24 25 26 27  ]
;array mem[13] = [ 13 14 15 16 17 18 19 20 | 21 22 23 24 25 26 27 28  ]
;array mem[14] = [ 14 15 16 17 18 19 20 21 | 22 23 24 25 26 27 28 29  ]
;array mem[15] = [ 15 16 17 18 19 20 21 22 | 23 24 25 26 27 28 29 30  ]
;0---4---8---c---
;----------------
;----------------
;----------------
;4---8---c---f---
;----------------
;----------------
;----------------
;8---c---f---20---
;----------------
;----------------
;----------------
;c---f---20---24---
;----------------
;----------------
;----------------
;
;compactare pe verticala =>
;0---4---8---c---
;4---8---c---f---
;8---c---f---20---
;c---f---20---24---
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;tanspozitie =>
;048c------------
;----------------
;----------------
;----------------
;48cf------------
;----------------
;----------------
;----------------
;8cf20------------
;----------------
;----------------
;----------------
;cf2024------------
;----------------
;----------------
;----------------
;
;compactare =>
;048c------------
;48cf------------
;8cf20------------
;cf2024------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;=>
;048c------------
;48cf------------
;8cf20------------
;cf2024------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
;----------------
; result:
;array mem[0] = [ 0 4 8 12 - - - - - - - - - - - -  ]
;array mem[1] = [ 4 8 12 16 - - - - - - - - - - - -  ]
;array mem[2] = [ 8 12 16 20 - - - - - - - - - - - -  ]
;array mem[3] = [ 12 16 20 24 - - - - - - - - - - - -  ]
;-------------------------------------------------------------------------------------
func testForFullPulling
label0:
				nop				activate
				sel_addrreg 0	scannop
				
				generateFullSquareMatrixIX(-1)

				;cc_start_w_halt	nop
				
;remove lines
				nop				load 4
				nop				store 1
				nop				load 8
				nop				store 2
				nop				load 12
				nop				store 3
				
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
				
;remove lines
				nop				load 20
				nop				store 17
				nop				load 24
				nop				store 18
				nop				load 28
				nop				store 19
				
;matrixTranspose 
				nop				ixload
				nop				addrstore
				nop				store 33 ; read address: 2*$ARRAY_NR_CELLS
				nop				vadd 16
				vload -1		store 32 ; write address: 1+2*$ARRAY_NR_CELLS
				store 1     	rload 16
				vload 14		rstore 0
	;read
		label18:	store 0 		load 32		
				srstore			nop	
				nop				nop
				rotateright		nop
				nop				srload
				load 1			addrstore
				vadd 1			store 32
				store 1			rload 0
	;rotate		
				srstore         nop
				nop             nop
		label19:	rotateleft      nop
				brnzdec label19  nop
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
				
				brnzdec label18	nop
				;cc_stop			nop
				
				setint			nop
				halt			nop
endfunc