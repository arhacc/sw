#pragma once
#include "ClockConsumerThread.hpp"

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
 *Signal description 	                                                          Write Address channel 	          Read Address channel
 ***************************************************************************************************************************************************
 *Address ID, to identify multiple streams over a single channel 	                    AWID 	                            ARID
 *Address of the first beat of the burst 	                                            AWADDR 	                            ARADDR
 *Number of beats inside the burst 	                                                    AWLEN 	                            ARLEN
 *Size of each beat 	                                                                AWSIZE 	                            ARSIZE
 *Type of the burst 	                                                                AWBURST 	                        ARBURST
 *Lock type, to provide atomic operations 	                                            AWLOCK 	                            ARLOCK
 *Memory type, how the transaction has to progress through the system 	                AWCACHE 	                        ARCACHE
 *Protection type: privilege, security level and data/instruction access 	            AWPROT 	                            ARPROT
 *Quality of service of the transaction 	                                            AWQOS 	                            ARQOS
 *Region identifier, to access multiple logical interfaces from a single physical one 	AWREGION 	                        ARREGION
 *User-defined data 	                                                                AWUSER 	                            ARUSER
 *xVALID handshake signal 	                                                            AWVALID 	                        ARVALID
 *xREADY handshake signal 	                                                            AWREADY 	                        ARREADY

 *
 * Signals of the Read and Write Data channels
 *
 ***************************************************************************************************************************************************
 *Signal description 	                                                           Write Data channel 	               Read Data channel
 ***************************************************************************************************************************************************
 *Data ID, to identify multiple streams over a single channel 	                         WID 	                            RID
 *Read/Write data 	                                                                     WDATA 	                            RDATA
 *Read response, to specify the status of the current RDATA signal 		                   -                                RRESP
 *Byte strobe, to indicate which bytes of the WDATA signal are valid 	                 WSTRB                                -
 *Last beat identifier                                                                   WLAST                           	RLAST
 *User-defined data 	                                                                 WUSER 	                            RUSER
 *xVALID handshake signal 	                                                             WVALID 	                        RVALID
 *xREADY handshake signal 	                                                             WREADY 	                        RREADY

 *
 * Signals of the Write Response channel
 *
 ***************************************************************************************************************************************************
 *Signal description 	                                                          Write Response channel
 ***************************************************************************************************************************************************
 *Write response ID, to identify multiple streams over a single channel 	              BID
 *Write response, to specify the status of the burst 	                                  BRESP
 *User-defined data 	                                                                  BUSER
 *xVALID handshake signal 	                                                              BVALID
 *xREADY handshake signal 	                                                              BREADY
 *
 */

class AxiLite: public ClockConsumerThread
{
public:
    AxiLite(Sync& sync, DUT* pdut, std::string axi_lite_thread);
    /**
    * Extracts 'k' bits from position 'p'
    * @return unsigned int     New number extracted
    */
    unsigned int extractBits(unsigned long number, int k, int p);
    unsigned int extractAxiBits(unsigned int number, int k, int p);

    /**
    * Writes wData from wAddr to Subordinate. Signals on Write channel have 'w' as prefix
    * @return void
    */
    void AxiStreamWrite(uint32_t wAddr, uint32_t wData);
    /**
    * Writes wData from wAddr to Master. Signals on Read channel have 'r' as prefix
    * @return void
    */
    unsigned int AxiStreamRead(uint32_t rAddr);
};