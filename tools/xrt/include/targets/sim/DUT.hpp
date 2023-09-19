#ifndef XPU_SIMULATOR_TXPU_HPP
#define XPU_SIMULATOR_TXPU_HPP
#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <cassert>
#include <charconv>
#include <string_view>
#include <thread>
#include <iomanip>

#include "Defines.hpp"
#include "Simulation.hpp"
#include "Parameters.hpp"
#include "Constants.hpp"

typedef struct{
    int port_id;
    int port_bits;
    bool is_input;
} port_parameters;

class DUT
{
private:
    /*Pointer to Loader that instantiates the design*/
    Xsi::Loader* m_xsi{};
    /*A map that contains information about ports from design*/
    std::map<std::string, port_parameters> m_port_map{};
    /*Clock port*/
    std::string m_clock{};
    /*Reset port*/
    std::string m_reset{};
    unsigned int m_test_done_ok{};
    /*Number of cycles that were run*/
    unsigned int m_cycle_half_count{};
    /*Half period of clock*/
    unsigned int m_clock_half_period{};

public:
    /**
    * Construct an DUT object that talks to Vivado Simulator
    *
    * @param design_libname         Name of compiled library that is exported to xsim.dir
    * @param simkernel_libname      API that is used for calling functions from Vivado: librdi_simulator_kernel.so
    * @param clock_period_ns        Period of clock in ns
    * @param clock_name             Name of clock port from design
    * @param reset_name             Name of reset port from design
    * @return DUT*           DUT object
    */
    DUT(const std::string& design_libname, const std::string& simkernel_libname,
        float clock_period_ns, const std::string& clock_name, const std::string& reset_name);
    /**
    * Destroys an DUT object. Deletes *xsi.
    */
    ~DUT();
    /**
    * Returns the size of m_port_map.
    * @return unsigned int size
    */
    std::size_t num_ports();
    /**
    * Lists the ports from design to OUT stream.
    */
    void list_ports();
    /**
    * Writes 32b value to design port
    *
    * @param port_name          Name of design port 
    * @param value              Value to write to port_name
    * @return T                 A value that was given
    */
    void write(const std::string& port_name, const std::string_view& value);
    /**
    * Writes 64b value to design port
    *
    * @param port_name          Name of design port
    * @param value              Value to write to port_name
    * @return T                 A value that was given
    */
    void write(const std::string& port_name, unsigned int value);
    /**
    * Reads 32 bits value from an OUT port
    *
    * @param port_name          Name of design port 
    * @return unsigned int      Value that was read from design port
    */
    unsigned int read(const std::string& port_name);

    std::map<unsigned int, unsigned int> get64Value(const std::string port_name);

    /**
    * Resets simulation
    */
    void restart();
    /**
    * Resets the accelerator
    *
    * @param clock_cycles       Number of clock cycles that
    */
    void do_reset(int clock_cycles);
    /**
    * Writes address to s00_axi_awaddr port
    *
    * @param address            Write address
    */
    void write_addr(uint32_t address);
    /**
    * Writes data to s00_axi_wdata port
    *
    * @param address            Write data
    */
    void write_data(uint32_t data);
    /**
    * Wait for m_clock_half_period
    */
    void _wait_half_period_cycle();

    /**
    * Wait for clock_period
    */
    void _wait_clock_cycle();
    /**
    * Wait number of clock_cycles
    *
    * @param clock_cycles       Number of clocks to wait for
    */
    void _wait_clock_cycles(int clock_cycles);
    /**
    * Gets current simulation time
    *
    * @return XSI_INT64         Current simulation time
    */
    XSI_INT64 getTime() const;

    // set resetn to 1
    void doResetInactive();

    // set resetn to 0
    void doResetActive();
    /**
    * Generates clock with period (us,ns,ps)
    *
    * @param period             Number of us,ns,ps you want the clock to run
    */
    void generateClock(unsigned int period);
    /**
    * Getter for m_clock_half_period
    *
    * @return unsigned int      m_clock_half_period
    */
    unsigned int getHalfClockPeriod() const;

    std::vector<unsigned int> keepAndRemove(const std::vector<unsigned int>& arr);

    int getNoBits(const char* port_name);
    /*Set initial signals for testbench*/
    void init();
    /*AXI_Lite set initial signals*/
    void AXI_init();
    void readAxiSignals();
    int getStatus();
    const char* getError();

#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    std::fstream golden_model_out_file;
#endif
};

#endif //XPU_SIMULATOR_TXPU_HPP
