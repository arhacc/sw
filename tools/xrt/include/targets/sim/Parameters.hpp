//
// Created by alex13dumi on 20.12.2022.
//

#ifndef XPU_SIMULATOR_PARAMETERS_HPP
#define XPU_SIMULATOR_PARAMETERS_HPP
#pragma once
#define ARRAY_NR_CELLS 16 //^ must be power of 2, greater or equal to 4

#define ACCELERATOR_ID 42

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// synthesis defines 	
#define NR_PARTIAL_MULTICELLS_PER_MULTICELL 2
#define NR_CELLS_PER_PARTIAL_MULTICELL 2
//^ must be power of 2, greater or equal to 2 ; for dte column_counter+padding_counter generation; should be fixed (and possibly others)
//^ currently, for axi stream integration in io, should be fixed at 2. untested other values at that level
#define NR_CELLS_PER_MULTICELL (NR_CELLS_PER_PARTIAL_MULTICELL * NR_PARTIAL_MULTICELLS_PER_MULTICELL )
#define NR_MULTICELLS (ARRAY_NR_CELLS / NR_CELLS_PER_MULTICELL )
// ! make sure this is an integer; do the math; do not edit these 2 defines
#define DTE_FIFO_SIZE_NR_COMMANDS 8
#define DTE_READY_COUNTER_NR_BITS 4
// ^ 2 counters used to count number of written matrices and result ready(,from programs)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////// memory defines
#define ARRAY_CELL_MEM_SIZE       1024
#define CONTROLLER_MEM_SIZE       1024
#define CONTROLLER_INSTR_MEM_SIZE 1024
// instr (prog) mem max at 1M 
// ^ must be powers of 2
#define CONTROLLER_ADDR_REG_NR_LOCATIONS 4


/////////////////////////////////////////////////////////////////////////////////////////////////////////// parametrizable resources
//!!!!!!!!!!!!!!!!!!!! comment/uncomment as needed !!!!!!!!!!!!!!!!! changing value to 0 has no effect
// please note instructions are not conditionally compiled and compilation errors 
// do not occur if written code uses unavailable hardware resources
// resources only affect the array part. controller is always as complete as can be 

//cell level
//#define RESOURCE_ARRAY_CELL_HAS_ROTATE 1
//#define RESOURCE_ARRAY_CELL_HAS_SHIFT 1
// ^ needs "RESOURCE_CELL_HAS_ROTATE" in order to function
#define RESOURCE_ARRAY_CELL_HAS_STACK  1
#define RESOURCE_ARRAY_CELL_STACK_SIZE 4
#define RESOURCE_CTRL_CELL_STACK_SIZE  4
// minimum of 3 if stack exists.
//#define RESOURCE_ARRAY_CELL_HAS_FLOATING_POINT 1 
//#define RESOURCE_CTRL_HAS_FLOATING_POINT 1
// ^^ requires RESOURCE_ARRAY_CELL_HAS_ROTATE and RESOURCE_ARRAY_CELL_HAS_SHIFT in order to function
// float operations require data sizes of 32b; div in ctrl not implemented
//#define RESOURCE_ARRAY_CELL_HAS_SHIFT_RIGHT_FIXED_AMOUNT 1
#define RESOURCE_ARRAY_HAS_GLOBAL_SHIFT_REG 1

// bw/reduce net 
#define RESOURCE_ARRAY_HAS_BW_SCAN_NET 1
#define RESOURCE_ARRAY_HAS_REDUCE_NET 1
//#define RESOURCE_NET_HAS_ONLY_PERMUTE 1
// overrides all other operations below, requires RESOURCE_ARRAY_HAS_BW_SCAN_NET


#define RESOURCE_NET_HAS_OPERATION_NOP 1
// ^ always leave 1; 
// \/ ALWAYS have at least 1 operation selected. vivado insta crashes if not. 
#define RESOURCE_NET_HAS_OPERATION_ADD 1
//#define RESOURCE_NET_HAS_OPERATION_SUB 1
//#define RESOURCE_NET_HAS_OPERATION_MIN 1
//#define RESOURCE_NET_HAS_OPERATION_MAX 1
//#define RESOURCE_NET_HAS_OPERATION_SPLIT 1
//#define RESOURCE_NET_HAS_OPERATION_PERMUTE 1
//#define RESOURCE_NET_HAS_OPERATION_SUM_PREFIX 1
//#define RESOURCE_NET_HAS_OPERATION_bitwise_AND 1
//#define RESOURCE_NET_HAS_OPERATION_bitwise_OR 1
//#define RESOURCE_NET_HAS_OPERATION_bitwise_XOR 1
//#define RESOURCE_NET_HAS_OPERATION_bitwise_XOR_PREFIX 1




////////////////////////////////////////////////////////////////////////////////////////////////////////////////// io fifos
#define IO_PROG_FIFO_POINTER_NR_BITS 10
//#define IO_PROG_FIFO_USE_PRIMITIVE 1
// ^ comment / uncomment as needed
#define IO_PROG_FIFO_PRIMITIVE_TYPE "distributed"
// ^ active only with IO_DATA_FIFO_USE_PRIMITIVE defined; can be: "auto", "block", "distributed", "ultra"


#define IO_DATA_FIFO_POINTER_NR_BITS 10
// total size is 1<<IO_DATA_FIFO_POINTER_NR_BITS for both in/out fifos
//#define IO_DATA_FIFO_USE_PRIMITIVE 1
// ^ comment / uncomment as needed
#define IO_DATA_FIFO_PRIMITIVE_TYPE "distributed"
// ^ active only with IO_DATA_FIFO_USE_PRIMITIVE defined; can be: "auto", "block", "distributed", "ultra"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////// misc

#define RESOURCE_FEATURE_TESTING 1
// miscelaneous feature testing, in ctrl: clock cycle counter, stack depth counters

#define GLOBAL_SHIFT_REG_LOOP_PIPELINE_STAGES 1
// nr of delay register stages, used for prventing the loop from limitng global clock 
// imposes this number of clock cycles between 2 succesive shift instructions for data validity

#define SHIFT_RIGHT_FIXED_AMOUNT 8
// used for the SHRIGHT_FIXED_AMOUNT assembler instruction. 
// number of bits the acc gets shifted to the right


#define RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION 1
// if commented, select which scan mode using RESOURCE_NETWORK_SINGLE_SCAN_MODE further below 
#define NETWORK_SCAN_MODE_CONTINOUS_MEM_OUT 0
#define NETWORK_SCAN_MODE_CONTINOUS_GLOBAL_SHIFT_REG (NETWORK_SCAN_MODE_CONTINOUS_MEM_OUT+1)
#define NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT (NETWORK_SCAN_MODE_CONTINOUS_GLOBAL_SHIFT_REG+1)
#define NETWORK_SCAN_MODE_ONE_SHOT_SHIFT_REG (NETWORK_SCAN_MODE_ONE_SHOT_MEM_OUT+1)
// ^ modes == how the network gets its addr for bw permutations
//#define RESOURCE_NETWORK_SINGLE_SCAN_MODE (NETWORK_SCAN_MODE_CONTINOUS_MEM_OUT)
// for use with RESOURCE_NETWORK_HAS_SCAN_MODE_SELECTION commented	

#define ARRAY_DECODE_LOCATION_CONTROLLER 1
//#define ARRAY_DECODE_LOCATION_MULTICELL 1
//#define ARRAY_DECODE_LOCATION_PARTIAL_MULTICELL 1
//#define ARRAY_DECODE_LOCATION_ELEM_CELL 1
// ^ pick just one 


#endif //XPU_SIMULATOR_PARAMETERS_HPP
