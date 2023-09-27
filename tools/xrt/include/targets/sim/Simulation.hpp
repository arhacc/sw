//
// Created by alex13dumi on 20.12.2022.
//

#ifndef XPU_SIMULATOR_PARAMETERS_SIMULATION_HPP
#define XPU_SIMULATOR_PARAMETERS_SIMULATION_HPP
#pragma once
#include "Defines.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// tb/simulator defines

#define SIMULATION_TYPE_BEHAVIOURAL 1
//#define SIMULATION_TYPE_POST_SYNTH_FUNCTIONAL 1
// ^ pick one 

#define SIMULATION_FUNCTION_PERFORMED "fpga_test"
#define SIMULATION_TEST_NAME "test3"
// ALWAYS check nr of lines in function arguments file matches nr of prun commands sent 



#define SMALL_RECTANGLES_SIZE 1
// for use with functions that work on rectangles/squares (horizontal small rectangle)
// use ARRAY_NR_CELLS if working on vertical / full horizontal functions
// measured in nr array cells

#define SIMULATION_GENERATE_NEW_PROGRAM_FILE 1
#define SIMULATION_HAS_DATA_INPUT_FILE 1
#define SIMULATION_HAS_FUNCTION_ARGUMENTS_FILE 1
#define SIMULATION_GOLDEN_MODEL_CHECKS 0
// ^ comment/uncomment if data checking is desired  (also needs file)


#define SIMULATION_NR_LINES_DATA_OUT_READ 1
// for IO; blocks tb natural $stop() if not set properly;
#define SIMULATION_NR_DATA_OUT_READ 1024
// ^ comment if not known or desired 
// it is usually: (SIMULATION_NR_LINES_DATA_OUT_READ * ARRAY_NR_CELLS)


#define SIMULATION_DATA_OUT_FILE_ADD_NEW_LINE_AFTER_NR_LINES_READ 16
// ^ comment if this is not desired // for easy read 

#define SIMULATION_TIMESCALE_TIME_UNIT 1ns
#define SIMULATION_TIMESCALE_PRECISION 1ps
#define SIMULATION_CLOCK_PERIOD 10
// ^ use even number; 10=>100MHz
#define SIMULATION_DRAIN_TIME_CLOCK_CYCLES 300
// ^ wait amount after set interrupt; disabled if "SIMULATION_NR_DATA_OUT_READ is not known 
#define SIMULATION_DRAIN_TIME_DATA_OUT_CLOCK_CYCLES (32 * ARRAY_NR_CELLS * 2)
// ^ wait amount after set interrupt. in order to read data out when "SIMULATION_NR_DATA_OUT_READ" is not known 
#define SIMULATION_END_TIME_CLOCK_CYCLES 10000
// ^ emergency simulation stop in case it gets stuck in fork 




#define SIMULATION_PRINTING 0
// ^ comment/uncomment; use when in depth debug is needed, do not use with post synth simulation

#define SIMULATION_PRINT_CLOCK_CYCLES_PAUSE 0
// ^ used in order to print less frequently; if printing is not required on each cc
// for printing on each cc, set to 1;

#define TB_PRINT_NR_NUMBERS_BEFORE_BAR (8)
//final printing
#define PRINT_FINAL_DO_FINAL_PRINTING 0

// continous printing
#define PRINT_CONTINOUS 0
// ^ overrides the rest 
#define PRINT_CONTINOUS_ARRAY_GENERAL_INFO 0
#define PRINT_CONTINOUS_ARRAY_SCAN_NET_INFO 0
#define PRINT_CONTINOUS_INSTRUCTION_INFO 0
#define PRINT_CONTINOUS_FP_INFO_PART1 0
#define PRINT_CONTINOUS_FP_INFO_PART2 0
#define PRINT_CONTINOUS_XPU_IO_FIFOS 0
#define PRINT_CONTINOUS_XPU_IO_PROGRAM_FIFO 0
#define PRINT_CONTINOUS_XPU_IO_DATA_IN_FIFO 0
#define PRINT_CONTINOUS_XPU_IO_DATA_OUT_FIFO 0
#define PRINT_CONTINOUS_ARRAY_IO_MONITORING 0
#define PRINT_CONTINOUS_ARRAY_IO_MONITORING_PARTIAL_MULTICELL_LEVEL 0
#define PRINT_CONTINOUS_ARRAY_IO_MONITORING_ELEMCELL_LEVEL 0
#define PRINT_CONTINOUS_ARRAY_IO_MONITORING_ELEMCELL_LEVEL_IO_COMMAND 0
#define PRINT_CONTINOUS_CONTROLLER_MEMORY_DATA 0
#define PRINT_CONTINOUS_ARRAY_MEMORY_DATA 0
#define PRINT_CONTINOUS_ARRAY_STACK 0
#define PRINT_CONTINOUS_CTRL_STACK 0
#define PRINT_CONTINOUS_CUSTOM_PRINT_CONTROLLER 0
#define PRINT_CONTINOUS_CUSTOM_PRINT_ARRAY 0
// ^: 1 or 0
// all for printing controls are independent 
// if printing speicfic locations form memory is needed, modify custom printing array (top of file) in simulator

#define PRINT_CONTROLLER_MEMORY_START_CONTINOUS 0
#define PRINT_CONTROLLER_MEMORY_STOP_CONTINOUS 4

#define PRINT_ARRAY_MEMORY_START_CONTINOUS 53
#define PRINT_ARRAY_MEMORY_STOP_CONTINOUS 56
// ^ includes margins 

/////////////////////////////////////////////////////////////////////////////////////////////////////// USER : do not edit below this point 

#ifdef SIMULATION_TYPE_BEHAVIOURAL
#define SIMULATION_FILE_BASEPATH std::string("/home/alex13dumi/Desktop/xpu_simulator/")
#endif
#ifdef SIMULATION_TYPE_POST_SYNTH_FUNCTIONAL
#define SIMULATION_FILE_PATH_BACKSTEPS std::string("../../../../../")
#endif

#endif //XPU_SIMULATOR_PARAMETERS_SIMULATION_HPP
