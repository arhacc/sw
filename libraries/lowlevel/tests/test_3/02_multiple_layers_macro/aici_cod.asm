;-------------------------------------------------------------------------------------
;XPU kernel low level library
;
;minor test implementation
; binary form at: 
;	xpo\iio_simulation_data\program_file\program_file_16_2_2_temp_test1.txt
;-------------------------------------------------------------------------------------
architectureId xpu_46FA4B1F360DA9A43C262193294DC4CD
define XPU_CONST log2($ARRAY_NR_CELLS + 10) * $NR_MULTICELLS + 100
;include "macros.asm"
macro generateSquareMatrixM1(par1M1, par2M1)
				vload log2(par1M1*2)	ixload
		label1:	vsub par1M1			ristore par2M1 / 10
				brnz label1		vadd ($XPU_CONST/2)*3 + 1 + $ARRAY_NR_CELLS
endmacro

;-------------------------------------------------------------------------------------
macro generateSquareMatrixM2(par1M2, par2M2)
	generateSquareMatrixM1(par1M2 * par2M2, par2M2  )
	generateSquareMatrixM1(par1M2		, par2M2/2)
endmacro

;-------------------------------------------------------------------------------------
macro generateSquareMatrixM3(par1M3, par2M3)
	generateSquareMatrixM2(par1M3 * par2M3, par2M3  )
	generateSquareMatrixM2(par1M3		, par2M3/2)
endmacro

func test
label0:
				nop							activate
				sel_addrreg 0				scannop
				vload 10+2 					vload 1
				addrstore					addrstore	
				
				generateSquareMatrixM3($ARRAY_NR_CELLS * $ARRAY_NR_CELLS * 100, $NR_MULTICELLS *($ARRAY_NR_CELLS + 1))
				
				halt						nop
endfunc

;-------------------------------------------------------------------------------------
