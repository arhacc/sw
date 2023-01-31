/*

	info:
		
		1) all programs are sutructured as: 
			mem is split in 2 halves - one for computing; one for io 
				each half is split in 4: operand 0; operand1; result; misc info/ram;
		2) all testing done on 2 cells_per_partial + 2 partials_per_multi setup;
		3) io testing not properly done at this point
		4) all programs follow the following flow:
			a) array - data io transfers in 
			b) cADDRLD + ADDRL 
					cVLOAD(0);				VLOAD(0)
					cADDRLD;				ADDRLD;
					cNOP;					NOP;
				unde in loc de 0,0 tre sa fie o valoare calculata in arm. 
			c) controller - parameters transfers in  
			d) jump ca sa se dea startul
				!) aici pot incepe load pt urmatorul 
			e) run efectiv - primitivele - function calls after jump address
			
			f) data io transfer out 
		5) all primitives end in cHALT 
		
	
		
		
	
	
//*/ 