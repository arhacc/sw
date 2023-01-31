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

//-------------------------------------------------------------------------------------
class XpuTestBench {
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
    void wait_clock_cycles(unsigned int nr_clock_cycles);
    void do_reset(unsigned int nr_active_clock_cycles);
    /*Testbench init*/
    void tb_init();
    void AXI_init();
    void write_addr(uint32_t data);
    void write_data(uint32_t data);
    void wait_high();
    void wait_low();
    void wait();

    void AXI_Lite_write(unsigned int waddr, unsigned int wdata);
    void AXI_Lite_read(unsigned int raddr);
    void AXI_Stream_SendData(unsigned int data);
    void AXI_Stream_SendStop();
    void AXI_Stream_receive_data();
    void AXI_Stream_stop_receive_data();


    const s_xsi_vlog_logicval one_val  = {0X00000001, 0X00000000};
    const s_xsi_vlog_logicval zero_val = {0X00000000, 0X00000000};
    XSI_INT32 kernel_precision;
    XSI_INT32 time_ticks;

private:
    Xsi::Loader xsi;
    std::map<std::string, port_parameters> port_map;
    unsigned int clock_half_period;
    unsigned int cycle_count;
    std::string clock;
    std::string reset;
    std::fstream file;
    int test_done_ok;
};

#endif //XPU_SIMULATOR_TXPU_HPP
//-------------------------------------------------------------------------------------
