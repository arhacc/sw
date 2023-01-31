;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;XpuProject4 asm implementation
;
;-------------------------------------------------------------------------------------

func get_matrix
				param        		nop
				nop          		nop
				nop          		cload
				param        		nop
				vsub 1       		vsub 1
				store 0      		addrld
label0:			
				vload 8      		nop
				getv          		nop
				load 0        		ioload
				brzdec label1		ristore 1
				store 0      		nop
				jmp label0   		nop
label1:			
				halt         		nop
endfunc

;-------------------------------------------------------------------------------------


