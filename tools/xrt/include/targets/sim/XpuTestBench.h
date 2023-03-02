//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once
#ifndef XPU_SIMULATOR_TXPU_HPP
#define XPU_SIMULATOR_TXPU_HPP
#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <fmt/format.h>
#include <charconv>
#include "constants.hpp"

typedef struct{
    int port_id;
    int port_bits;
    bool is_input;
} port_parameters;

class XpuTestBench
{
public:
    XpuTestBench(const std::string& design_libname, const std::string& simkernel_libname,
                 float clock_period_ns, const std::string& clock_name, const std::string& reset_name);
    ~XpuTestBench();

    unsigned int num_ports();
    void list_ports();
    void run_ncycles(unsigned int n);
    void write(const std::string &port_name, unsigned int val);
    unsigned int read(const std::string &port_name);
    void restart();
    void do_reset(unsigned int nr_active_clock_cycles);
    void write_addr(uint32_t data);
    void write_data(uint32_t data);
    void wait_high_clock();
    void wait_low_clock();
    void wait_clock();
    bool axi_bvalid_ready();
    void wait_clock_cycles(unsigned int nr_clock_cycles);
    XSI_INT64 get_time();
    void generate_clock(int clock_half_period);

    /*Testbench init*/
    void tb_init();
    void AXI_init();
    void readAxiSignals();
    void AXI_Lite_write(uint32_t waddr, uint32_t wdata);
    void AXI_Lite_read(uint32_t raddr);
    void AXI_Stream_SendData(unsigned int data);
    void AXI_Stream_SendStop();
    void AXI_Stream_receive_data();
    void AXI_Stream_stop_receive_data();

    XSI_INT32 kernel_precision;
    XSI_INT32 time_ticks;

    std::fstream axi_stream_out_file;
    std::fstream axi_stream_in_file;
#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    std::fstream golden_model_out_file;
#endif
    std::fstream axi_lite_program_file_combined;
    std::fstream axi_lite_program_file;
    std::fstream axi_lite_function_arguments_file;

    unsigned int clock_half_period;
    Xsi::Loader xsi;

private:
    std::map<std::string, port_parameters> port_map;
    unsigned int cycle_count;
    std::string clock;
    std::string reset;
    int test_done_ok;
};

#endif //XPU_SIMULATOR_TXPU_HPP
//-------------------------------------------------------------------------------------
