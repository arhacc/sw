;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;minor test implementation
; binary form at: 
;	xpo\iio_simulation_data\program_file\program_file_16_2_2_temp_test1.txt
;-------------------------------------------------------------------------------------
architectureId xpu_46FA4B1F360DA9A43C262193294DC4CD
define XPU_CONST log2($ARRAY_NR_CELLS + 10) * $NR_MULTICELLS + 100

macro generateSquareMatrix(par1, par2)
				vload log2(par1*2)	ixload
		label1:	vsub par1			ristore par2 / 10
				brnz label1			vadd ($XPU_CONST/2)*3 + 1 + $ARRAY_NR_CELLS
endmacro

func test
label0:
				nop							activate
				sel_addrreg 0				scannop
				vload 10+2 					vload 1
				addrstore					addrstore	
				
				generateSquareMatrix($ARRAY_NR_CELLS * $ARRAY_NR_CELLS * 100, $NR_MULTICELLS *($ARRAY_NR_CELLS + 1))
				halt						nop
endfunc

;-------------------------------------------------------------------------------------
