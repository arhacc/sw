macro	generateFullSquareMatrix(where, fitstLine) ; "where" is the previous addess
				nop						vload where
				nop						addrstore
				vload $ARRAY_NR_CELLS	vload fitstLine
		label1:	vsub 1					ristore 1
				brnz label1				vadd 1
endmacro