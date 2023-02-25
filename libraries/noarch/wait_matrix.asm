;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;wait_matrix implementation
;
;-------------------------------------------------------------------------------------

func wait_matrix
label0:
				param        					nop
				rstore 16         				nop
				setval 0          				nop
				vload -1        				nop
				setdec       					nop
				vload      						nop
label1:
				waitmatw 1     					nop
				brcmpnvaldec label1, 0       	nop

				halt         					nop
endfunc

;-------------------------------------------------------------------------------------

