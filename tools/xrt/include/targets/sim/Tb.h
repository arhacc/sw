//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
/**
 ***************************************************************************************************************************************************
 *  AMBA® AXI Protocol Specification
 ***************************************************************************************************************************************************

 * AXI Protocol supports high-performance, high-frequency communication between a Manager and Subordinate. A Manager might be a Core or DMA. A Subordinate can be a Peripheral(I2C, SPI) or Memory(SRAM, DRAM).
 * Key features:
 * High-frequency operations without using complex bridges.
 * Low cost DMA because of separate write and read data channels
 * Suitable for high-bandwidth and low-latency designs.

 ***************************************************************************************************************************************************
 *  AXI Arhitecture
 ***************************************************************************************************************************************************

 * AXI Protocol is transactions based and has 5 different channels:
  --------------------------------
  | Channel             | Prefix |
  --------------------------------
 * AW: write request    | 'A'
 * W:  write data       |  -
 * B:  write response   |  -
 * AR: read request     | 'A'
 * R:  read data        |  -

 * A request channel carries control information about that data will be transfered. The data is transferred between a Manager and a Subordinate:
 * - A write data channel from Master to Subordinate to transfer data. The Subordinate uses the write response channel to inform the Manager that
 * the write has been sucessful or not(it signals the completion of the data transfer)
 * - A read data channel to transfer data from Subordinate to Master
 * Observation: Master uses 3 channels to transfer data to a Subordinate, besides 2 channels to transfer data to Master.
 * !!!!!!!!!!!! FIGURE A1.1 to be put in THESIS and A1.2

 ***************************************************************************************************************************************************
 *  Channel definition
 ***************************************************************************************************************************************************

 * Each of the five independent channels have a VALID and READY signals that provide a two way handshake.
 * VALID: used by the information source to show when valid data, address or control information is available
 * READY: used by the destination source to show when it accepts information
 * Both read and write include a LAST signal to indicate the transfer of the last(final) data item

 ***************************************************************************************************************************************************
 *  Write and read request channels
 ***************************************************************************************************************************************************

 * Separate read and write request channels that carry address and control information for a transaction

 ***************************************************************************************************************************************************
 *  Write data channel
 ***************************************************************************************************************************************************

 * This channel carries the write data from Manager to Subordinate. It has the following properties:
 * - data signal can be 8, 16, 32, 64, 128, 256, 512, 1024 bits wide. Width is given by DATA_WIDTH property.
 * - a byte lane strobe signal for 1B(Byte) that indicates which bytes of a data signal are valid.
 * The Manager can perform write transactions without Subordinate acknowledgment of previous write transactions, write data channel information
 * is always treated as buffer.

 ***************************************************************************************************************************************************
 *  Write response channel
 ***************************************************************************************************************************************************

 * This channel is used by a Subordinate that responds to a write channel. Completion signal is requeired on the write response channel by all
 * write transactions. Completion is signaled only for a complete transaction, not for each data transfer.

 ***************************************************************************************************************************************************
 *  Read data channel
 ***************************************************************************************************************************************************

 * This channel carries read data and read response from a Subordinate to a Master. It has the following properties:
 * - data signal can be 8, 16, 32, 64, 128, 256, 512, 1024 bits wide. Width is given by DATA_WIDTH property.
 * - read response signal indicating the status of the read transaction

 ***************************************************************************************************************************************************
 *  Interface and interconnect
 ***************************************************************************************************************************************************

 * A system consists of a number of Managers and Subordinates devices that are connected using a form of interconnect (Splitters and Arbiters ??)
 * !!!!!!! Figure A1.3: Interface and interconnect

 ***************************************************************************************************************************************************
 *  Signal list
 ***************************************************************************************************************************************************

 *
 * Signals of the Read and Write Address channels
 *
 ***************************************************************************************************************************************************
 *Signal description                                                              Write Address channel               Read Address channel
 ***************************************************************************************************************************************************
 *Address ID, to identify multiple streams over a single channel                        AWID                                ARID
 *Address of the first beat of the burst                                                AWADDR                              ARADDR
 *Number of beats inside the burst                                                      AWLEN                               ARLEN
 *Size of each beat                                                                     AWSIZE                              ARSIZE
 *Type of the burst                                                                     AWBURST                             ARBURST
 *Lock type, to provide atomic operations                                               AWLOCK                              ARLOCK
 *Memory type, how the transaction has to progress through the system                   AWCACHE                             ARCACHE
 *Protection type: privilege, security level and data/instruction access                AWPROT                              ARPROT
 *Quality of service of the transaction                                                 AWQOS                               ARQOS
 *Region identifier, to access multiple logical interfaces from a single physical one   AWREGION                            ARREGION
 *User-defined data                                                                     AWUSER                              ARUSER
 *xVALID handshake signal                                                               AWVALID                             ARVALID
 *xREADY handshake signal                                                               AWREADY                             ARREADY

 *
 * Signals of the Read and Write Data channels
 *
 ***************************************************************************************************************************************************
 *Signal description                                                               Write Data channel                  Read Data channel
 ***************************************************************************************************************************************************
 *Data ID, to identify multiple streams over a single channel                            WID                                RID
 *Read/Write data                                                                        WDATA                              RDATA
 *Read response, to specify the status of the current RDATA signal                         -                                RRESP
 *Byte strobe, to indicate which bytes of the WDATA signal are valid                     WSTRB                                -
 *Last beat identifier                                                                   WLAST                              RLAST
 *User-defined data                                                                      WUSER                              RUSER
 *xVALID handshake signal                                                                WVALID                             RVALID
 *xREADY handshake signal                                                                WREADY                             RREADY

 *
 * Signals of the Write Response channel
 *
 ***************************************************************************************************************************************************
 *Signal description                                                              Write Response channel
 ***************************************************************************************************************************************************
 *Write response ID, to identify multiple streams over a single channel                   BID
 *Write response, to specify the status of the burst                                      BRESP
 *User-defined data                                                                       BUSER
 *xVALID handshake signal                                                                 BVALID
 *xREADY handshake signal                                                                 BREADY
 *
 */

//-------------------------------------------------------------------------------------
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
#include "XSILoader.h"

//-------------------------------------------------------------------------------------
typedef struct{
    int port_id;
    int port_bits;
    bool is_input;
} port_parameters;

class Tb {
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
    * Construct an Tb object that talks to Vivado Simulator
    *
    * @param design_libname         Name of compiled library that is exported to xsim.dir
    * @param simkernel_libname      API that is used for calling functions from Vivado: librdi_simulator_kernel.so
    * @param clock_period_ns        Period of clock in ns
    * @param clock_name             Name of clock port from design
    * @param reset_name             Name of reset port from design
    * @return Tb*           Tb object
    */
    Tb(const std::string& design_libname, const std::string& simkernel_libname,
        float clock_period_ns, const std::string& clock_name, const std::string& reset_name);
    /**
    * Destroys an Tb object. Deletes *xsi.
    */
    ~Tb();
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
    * Wait for clock_period
    */
    void wait_clock_cycle(int _numberOfCycles);
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

    int getNoBits(const char* port_name);
    /*Set initial signals for testbench*/
    void init();
    /*AXI_Lite set initial signals*/
    void AXI_init();
    void readAxiSignals();
    int getStatus();
    const char* getError();


    /**
    * Writes wData from wAddr to Subordinate. Signals on Write channel have 'w' as prefix
    * @return void
    */
    void axiWrite(uint32_t wAddr, uint32_t wData);
    /**
    * Writes wData from wAddr to Master. Signals on Read channel have 'r' as prefix
    * @return void
    */
    unsigned int axiRead(uint32_t rAddr);


#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    std::fstream golden_model_out_file;
#endif
};
//-------------------------------------------------------------------------------------
