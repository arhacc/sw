#pragma once
/*======================================================================================
Instruction's structure for cell/controller:

Instruction's structure for cell/controller: 
instr[15:0]	= {	opCode[15:11]	, // operation code
				operand[10:8]	, // operand select code
				scalar[c-1:0]	} // immediate value or address
		
For c = 8 the pair issued in each cycle is:		
instruction[31:0] = {arrayOpCode[15:11]	, // operation code					ir[31:27]
					 arrayOperand[10:8]	, // operand select code            ir[26:24]
					 arrayScalar[7:0]	, // immediate value or address     ir[23:16]
					 contrOpCode[15:11]	, // operation code					ir[15:11]
					 contrOperand[10:8]	, // operand select code			ir[10:8]
					 contrScalar[7:0]	} // immediate value or address 	ir[7:0]
				
	op: the operand in Controller
	cScalar: the scalar for Controller
	addr: the address register for data memory in Controller

	aScalar: the scalar for any cell
For: i = 0,1,...,(2^x)-1
	op[i]: the operand in cell i 
	addr[i]: the address register for data memory in cell i
	acc[i]: the accumulator of the cell i
	bool[i]: the Boolean variable of the cell i; bool[i] = 1 => cell i is active
======================================================================================*/

//  selects the right operand  
#define	ISA_val		(0)
//  immediate value: 
//	op    = cS = {24{cScalar[c-1]}}, cScalar} 
//	op[i] = aS = {24{arrayScalar[c-1]}}, arrayScalar}

#define ISA_mab 	ISA_val+1
//  absolute data memory: 
//	op    = mem[cS[s/v-1:0]]
//	op[i] = vectMem[aS[s/v-1:0]]

#define ISA_mrl 	ISA_mab+1
//  relative in data memory: 
//	op    = mem[addr + cS[s-1:0]]
//	op[i] = vectMem[addr + aS[v-1:0]]

#define ISA_mri 	ISA_mrl+1
//  relative and increment addr with scalar: 
//	op       = mem[addr + cS[s-1:0]];
//  	addr    <= addr + cS[s-1:0]
//	op[i]    = vectMem[addr + aS[v-1:0]];							  
//  	addr[i] <= addr + aS[v-1:0]

#define ISA_cop		ISA_mri+1
//  co-operand:
//	op = case(cS[1:0])
//		 3'b000: reduction add = add(acc[0],acc[1],...) 
//		 3'b001: reduction min = min(acc[0],acc[1],...)
//		 3'b010: reduction max = max(acc[0],acc[1],...)
//		 3'b011: reduction flag = = or(bool[0],bool[1],...)
//		 3'b100: leftEnd of globalShiftReg
//         3'b101: rEnd of globalShiftReg
//	op[i] = (contrOpCode==send) ? op : acc

#define ISA_stk 	ISA_cop+1
//$ op[i] = stack[i][layer1] (first under accumulator)

#define ISA_mrc		ISA_stk+1
//$ op[i] = vectMem[addr[i] + ((contrOpCode==send) ? op : acc)]

#define ISA_ctl 	7
//  control instructions	



// common instructions between all operands 

#define ISA_global_SR (0)
//$ left/right shift/rotate (and some pushes to the left cell)

#define ISA_addrinc ISA_global_SR+1
//	addr <= addr + value

#define ISA_last_common_instr_operand ISA_addrinc)



// operand = 000, ..., 110
// common instructions
#define ISA_add ISA_last_common_instr_operand+1
//  {cr, acc} <= acc + op;

#define ISA_addc ISA_add+1
//  {cr, acc} <= acc + op + cr;

#define ISA_sub ISA_addc+1
//  {cr, acc} <= acc - op;

#define ISA_rsub ISA_sub+1
//  {cr, acc} <= op - acc;

#define ISA_subc ISA_rsub+1
//  {cr, acc} <= acc - op - cr;

#define ISA_rsubc ISA_subc+1
//  {cr, acc} <= op - acc - cr;

#define ISA_mult ISA_rsubc+1
//  acc <= acc * op;

#define ISA_bwand ISA_mult+1
//  acc <= acc & op;

#define ISA_bwor ISA_bwand+1
//  acc <= acc | op;

#define ISA_bwxor ISA_bwor+1
//  acc <= acc ^ op;

#define ISA_load ISA_bwxor+1
//  acc <= op;

#define ISA_store ISA_load+1
//  op <= acc;

#define ISA_compare ISA_store+1
// cr <= (acc - op)[n]

#define ISA_stack_store_pop	ISA_compare+1
//  op <= acc (for save in local mem); pop stack 		

#define ISA_stack_push_load ISA_stack_store_pop+1
//  acc <= op (for save in local mem); push stack 

#define ISA_swap_memacc	ISA_stack_push_load+1
// acc <= mem[mem_addr_reg + instr_scalar];
// mem[mem_addr_reg+instr_scalar] <= acc;

#define ISA_fadd ISA_swap_memacc+1
// float add; inital instruction

#define ISA_fsub ISA_fadd+1
// float sub; inital instruction

#define ISA_fmult ISA_fsub+1
// float mult; inital instruction

#define ISA_fdiv ISA_fmult+1
// float div; inital instruction

#define ISA_fmult_first ISA_fdiv+1
// mantReg <= acc * {1'b1, memFrac[22:17]}

#define ISA_fmult_second ISA_fmult_first+1
// acc <= {8'b0, ((acc * {1'b1, memFrac[16:0]}) + mantReg << 17)[47:24]}

#define ISA_fdiv_loop1 ISA_fmult_second+1
// acc <= mantAcc >= mantMem ? mantAcc - mantMem : acc; efOp[0] = mantAcc >= mantMem

#define ISA_fdiv_loop2 ISA_fdiv_loop1+1
// acc <= acc << 1; mantReg <= {mantReg[30:0], efOp[0]}
// end common instructions
#define ISA_last_common_instr_val ISA_fdiv_loop2
// array
#define ISA_srcall ISA_last_common_instr_val+1
//$ search in all cells

#define ISA_search ISA_srcall+1
//$ search in selected cells

#define ISA_csearch ISA_search+1
//$ conditioned search

#define ISA_insert ISA_csearch+1
//$ insert op at first

//controller

#define ISA_send ISA_last_common_instr_val+1
//# send op as coOperand to array (NF2)	


// operand = 111; ctl
// common instructions
#define ISA_stack_operations ISA_last_common_instr_operand+1
// based on value in instruction: 0=>pop; 1=>duplicate; 2=>over; 3=>swap 
// pop layer 0 of stack (acc = stack layer1
// push layer 0 of stack (L0 L1 L2 => L0 L0 L1 L2)
// push stack layer1 ( L0 L1 => L1 L0 L1 ) 
// swap first 2 layers of stack ( L0 L1 L2=> L1 L0 L2 ) 

#define ISA_misc_store_load ISA_stack_operations+1
// miscelaneous load/store between, acc, global shift reg, addr_reg, w/ ctl operand

#define ISA_rotate_local ISA_misc_store_load+1
// acc = acc >> scalar_value; // msb rotates	; ctl for array, val+rest for ctrl

#define ISA_insval ISA_rotate_local+1
// acc <= {acc[23:0], scalar}

#define ISA_float ISA_insval+1
// float instruction defined by value from 1 to 9// val only

#define ISA_right_fixed_shifting ISA_float+1
// shifting right by 1 or by fixed amount. normal, w/ carry, arithmetic	

// end common instructions
#define ISA_last_common_instr_CTL ISA_right_fixed_shifting
// array
#define ISA_spatial_select ISA_last_common_instr_CTL+1

#define ISA_setscan ISA_spatial_select+1
//$ set scan function to value[2:0]

#define ISA_brshift	ISA_setscan+1
//$ Boolean vector right shift; for read from array 

#define ISA_delete ISA_brshift+1
//$ delete first	

// controller
#define ISA_jmp ISA_last_common_instr_CTL+1
//# pc <= pc + scalar;

#define ISA_sendint ISA_jmp+1
//# send interrupt

#define ISA_misc_testing ISA_sendint+1
// # for start and stop instructions (clock cycle counters)

#define ISA_param ISA_misc_testing+1
//# acc <= prog

#define ISA_prun 12
//# program run: 	cPRUN(startAddr) => {16'b0, 8'b1111_0111, startAddr[7:0]}

#define ISA_pload ISA_prun+1
//# program load: 	cPLOAD => {16'b0, 8'b1111_1111, 7'b0}

#define ISA_tinrun ISA_pload+1
// transfer in run; processed by dma 

#define ISA_toutrun ISA_tinrun+1
// transfer out run; processed by dma 

#define ISA_waitmatw ISA_toutrun+1
// wait matrix to be written (used by controller to wait for dma)

#define ISA_resready ISA_waitmatw+1
// inform dma that result is ready to be read. (generated by controller)


// #: controller only
// $: array only
// &: TBD
//=====================================================================

	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 