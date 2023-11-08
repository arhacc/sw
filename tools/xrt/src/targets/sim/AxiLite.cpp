//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/sim/AxiLite.h>
#define IO_INTF_PROG_AXILITE_DATA_SIZE 32 //???????

//-------------------------------------------------------------------------------------
AxiLite::AxiLite(Tb* _dut) {
    tb = _dut;
}

//-------------------------------------------------------------------------------------
void AxiLite::write(uint32_t wAddr, uint32_t wData)
{
    //std::cout << std::dec << "[AXI_LITE_WRITE] TIME: " << std::dec  << tb->getTime() << " wdata " << std::hex << wdata << std::endl;

    //posedge clock
    tb->write_addr(wAddr);
    tb->write("s00_axi_awprot", 0);
    tb->write("s00_axi_awvalid", 1);
    tb->write_data(wData);
    tb->write("s00_axi_wstrb", (1<<(IO_INTF_PROG_AXILITE_DATA_SIZE/8))-1 );
    tb->write("s00_axi_bready", 1);
    tb->write("s00_axi_wvalid", 1);
    tb->write("s00_axi_araddr", 0);
    tb->write("s00_axi_arprot", 0);
    tb->write("s00_axi_arvalid", 0);
    tb->write("s00_axi_rready", 0);
    wait_clock_cycle(1);

    while(tb->read("s00_axi_bvalid") == 0)
    {
        wait_clock_cycle(1);
    }

    while(tb->read("s00_axi_bvalid") == 1)
    {
        wait_clock_cycle(1);
    }

    //negedge axilite_bvalid
    tb->write("s00_axi_awaddr", 0);
    tb->write("s00_axi_awprot", 0);
    tb->write("s00_axi_awvalid", 0);
    tb->write("s00_axi_wdata", 0);
    tb->write("s00_axi_wstrb", 0);
    tb->write("s00_axi_wvalid", 0);
    tb->write("s00_axi_bready", 0);

    wait_clock_cycle(1);
}

//-------------------------------------------------------------------------------------
unsigned int AxiLite::read(uint32_t rAddr)
{
    tb->write("s00_axi_awaddr", 0);
    tb->write("s00_axi_awprot", 0);
    tb->write("s00_axi_awvalid", 0);
    tb->write("s00_axi_wdata", 0);
    tb->write("s00_axi_wstrb", 0);
    tb->write("s00_axi_wvalid", 0);
    tb->write("s00_axi_bready", 0);
    tb->write("s00_axi_araddr", rAddr);
    tb->write("s00_axi_arprot", 0);
    tb->write("s00_axi_arvalid", 1);
    tb->write("s00_axi_rready", 1);
    wait_clock_cycle(1);

    while(tb->read("s00_axi_arready") == 0)
    {
        wait_clock_cycle(1);
    }

    while(tb->read("s00_axi_arready") == 1)
    {
        wait_clock_cycle(1);
    }

    tb->write("s00_axi_arvalid", 0);

//    std::cout << "araddr: " << tb->read("s00_axi_araddr") << std::endl;
//
//    if(tb->read("s00_axi_rvalid"))
//        std::cout << "AXI_Lite Read:"  << std::hex << tb->read("s00_axi_rdata") << std::endl;

    wait_clock_cycle(2);

    tb->write("s00_axi_araddr", 0);
    tb->write("s00_axi_rready", 0);

    wait_clock_cycle(1);

    return tb->read("s00_axi_rdata");
}

//-------------------------------------------------------------------------------------
void AxiLite::wait_clock_cycle(int _numberOfCycles) {
    tb->wait_clock_cycle(_numberOfCycles);
}

//-------------------------------------------------------------------------------------
