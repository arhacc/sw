//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/sim/AxiLite.h>

//-------------------------------------------------------------------------------------
AxiLite::AxiLite(Dut* _dut) {
    dut = _dut;
}


void AxiLite::write(uint32_t wAddr, uint32_t wData)
{
    //std::cout << std::dec << "[AXI_LITE_WRITE] TIME: " << std::dec  << dut->getTime() << " wdata " << std::hex << wdata << std::endl;

    //posedge clock
    dut->write_addr(wAddr);
    dut->write("s00_axi_awprot", 0);
    dut->write("s00_axi_awvalid", 1);
    dut->write_data(wData);
    dut->write("s00_axi_wstrb", (1<<(IO_INTF_PROG_AXILITE_DATA_SIZE/8))-1 );
    dut->write("s00_axi_bready", 1);
    dut->write("s00_axi_wvalid", 1);
    dut->write("s00_axi_araddr", 0);
    dut->write("s00_axi_arprot", 0);
    dut->write("s00_axi_arvalid", 0);
    dut->write("s00_axi_rready", 0);
    wait_clock_cycle();

    while(dut->read("s00_axi_bvalid") == 0)
    {
        wait_clock_cycle();
    }

    while(dut->read("s00_axi_bvalid") == 1)
    {
        wait_clock_cycle();
    }

    //negedge axilite_bvalid
    dut->write("s00_axi_awaddr", 0);
    dut->write("s00_axi_awprot", 0);
    dut->write("s00_axi_awvalid", 0);
    dut->write("s00_axi_wdata", 0);
    dut->write("s00_axi_wstrb", 0);
    dut->write("s00_axi_wvalid", 0);
    dut->write("s00_axi_bready", 0);

    wait_clock_cycle();
}

unsigned int AxiLite::read(uint32_t rAddr)
{
    dut->write("s00_axi_awaddr", 0);
    dut->write("s00_axi_awprot", 0);
    dut->write("s00_axi_awvalid", 0);
    dut->write("s00_axi_wdata", 0);
    dut->write("s00_axi_wstrb", 0);
    dut->write("s00_axi_wvalid", 0);
    dut->write("s00_axi_bready", 0);
    dut->write("s00_axi_araddr", rAddr);
    dut->write("s00_axi_arprot", 0);
    dut->write("s00_axi_arvalid", 1);
    dut->write("s00_axi_rready", 1);
    wait_clock_cycle();

    while(dut->read("s00_axi_arready") == 0)
    {
        wait_clock_cycle();
    }

    while(dut->read("s00_axi_arready") == 1)
    {
        wait_clock_cycle();
    }

    dut->write("s00_axi_arvalid", 0);

//    std::cout << "araddr: " << dut->read("s00_axi_araddr") << std::endl;
//
//    if(dut->read("s00_axi_rvalid"))
//        std::cout << "AXI_Lite Read:"  << std::hex << dut->read("s00_axi_rdata") << std::endl;

    wait_clock_cycles(2);

    dut->write("s00_axi_araddr", 0);
    dut->write("s00_axi_rready", 0);

    wait_clock_cycles(1);

    return dut->read("s00_axi_rdata");
}

//-------------------------------------------------------------------------------------
