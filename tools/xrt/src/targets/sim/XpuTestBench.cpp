//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "targets/sim/XpuTestBench.h"
#include "targets/sim/defines.h"
#include "targets/sim/psimulation.h"

#define ARRAY_NR_CELLS 16

XpuTestBench::XpuTestBench(const std::string &design_libname, const std::string &simkernel_libname,
        float clock_period_ns, const std::string &clock_name, const std::string &reset_name) :

        xsi(design_libname, simkernel_libname) {
    //Load and open the TOP design
    std::cout << "Created shared object to: xsim.dir/simulator_axi/xsimk.so" << std::endl;
    s_xsi_setup_info info;
    memset(&info, 0, sizeof(info));
    info.logFileName = nullptr;
    char wdbName[] = "test.wdb";
    info.wdbFileName = wdbName;
    xsi.open(&info);
    xsi.trace_all();
    //Get informations about ports
    for (int i = 0; i < xsi.get_num_ports(); i++) {
        std::string port_name(xsi.get_port_name(i));
        port_parameters params{i, xsi.get_port_bits(i), xsi.port_is_input(i)};
        port_map[port_name] = params;
    }
    //Check if reset and clock exist
    if (port_map.find(reset_name) == port_map.end())
        throw std::invalid_argument("Reset not found in ports list");

    if (port_map[reset_name].port_bits != 1)
        throw std::invalid_argument("Reset is not a scalar");

    if (!port_map[reset_name].is_input)
        throw std::invalid_argument("Reset is not an input port");

    reset = reset_name;

    if (port_map.find(clock_name) == port_map.end())
        throw std::invalid_argument("Clock not found in ports list");

    if (port_map[clock_name].port_bits != 1)
        throw std::invalid_argument("Clock is not a scalar");

    if (!port_map[clock_name].is_input)
        throw std::invalid_argument("Clock is not an input port");

    clock = clock_name;

    clock_half_period = (unsigned int) (clock_period_ns * 10 * pow(10, -9) / xsi.get_time_precision() / 2);

    if (clock_half_period == 0)
        throw std::invalid_argument("Calculated half period is zero");

    //Results
    std::cout << "Identified " << num_ports() << " top-level ports:" << std::endl;
    list_ports();
    std::cout << "Using " << clock << " as clock with half-period of " << clock_half_period << " simulation steps"
              << std::endl;
    std::cout << "Using " << reset << " as " << "reset" << std::endl;
    //At the beginning cycle count is ZERO
    cycle_count = 0;
}

XpuTestBench::~XpuTestBench() {
    //close the simulation
    xsi.close();
}

unsigned int XpuTestBench::num_ports() {
    //return number of ports
    return port_map.size();
}

bool comparator(std::pair<std::string, XpuTestBench::port_parameters> &x,
        std::pair<std::string, XpuTestBench::port_parameters> &y) {
    return y.second.port_id > x.second.port_id;
}

std::vector<std::pair<std::string, XpuTestBench::port_parameters>>
sort(const std::map<std::string, XpuTestBench::port_parameters> &ports) {
    std::vector<std::pair<std::string, XpuTestBench::port_parameters>> copy_ports(ports.begin(), ports.end());
    std::sort(copy_ports.begin(), copy_ports.end(), comparator);
    return copy_ports;
}

void XpuTestBench::list_ports() {
    //list ports from the design
    std::vector<std::pair<std::string, port_parameters>>::iterator it;
    std::vector<std::pair<std::string, port_parameters>> ports = sort(port_map);
    for (it = ports.begin(); it != ports.end(); ++it) {
        fmt::print("{:<20} ID:{:<2} {:<2} bits {:<3} \n", it->first, it->second.port_id, it->second.port_bits,
                (it->second.is_input ? "Input" : "Output"));
    }
}

void XpuTestBench::run_ncycles(unsigned int n) {
    for (unsigned i = 0; i < n; i++) {
        //write(clock,0);
        xsi.run(clock_half_period);
        //write(clock,1);
        xsi.run(clock_half_period);
        cycle_count++;
    }
}

void XpuTestBench::write(const std::string &port_name, unsigned int val) {
    if (!port_map[port_name].is_input) {
        throw std::invalid_argument("Write called on output port");
    }
    unsigned int nwords = (port_map[port_name].port_bits + 63) / 64;
    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    logic_val.at(0) = {val, 0};
    for (unsigned i = 1; i < nwords; i++) {
        logic_val.at(i) = {0, 0};
    }
    xsi.put_value(port_map[port_name].port_id, logic_val.data());
    //std::cout << port_name << ":" << read(port_name) << std::endl;
}

unsigned int XpuTestBench::read(const std::string &port_name) {
    /*instead of 64 put 32, right method to do 31 instead of 63*/
    unsigned int nwords = (port_map[port_name].port_bits + 63) / 64;
    if (nwords > 1) {
        throw std::invalid_argument(port_name + " uint = read(string name) applies only to signals of 32b or less");
    }
    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    xsi.get_value(port_map[port_name].port_id, logic_val.data());
    return logic_val.at(0).aVal;
}

void XpuTestBench::restart() {
    xsi.restart();
}

void XpuTestBench::do_reset(unsigned int nr_active_clock_cycles) {
    AXI_init();
    write("resetn", 0);
    run_ncycles(nr_active_clock_cycles - 1);
    //write("clock", 0);
    xsi.run(clock_half_period);
    write("resetn", 1);
    //write("clock", 1);
    xsi.run(clock_half_period);
}

void XpuTestBench::write_addr(uint32_t data) {
    s_xsi_vlog_logicval val = {data, 0x0};
    std::cout << "write_addr(0x" << std::hex << data << ")" << std::endl;
    xsi.put_value(port_map["s00_axi_awaddr"].port_id, &val);
}

void XpuTestBench::write_data(uint32_t data) {
    s_xsi_vlog_logicval val = {data, 0x0};
    std::cout << "write_data(0x" << std::hex << data << ")" << std::endl;
    xsi.put_value(port_map["s00_axi_wdata"].port_id, &val);
}

void XpuTestBench::wait_high_clock() {
    //xsi.put_value(port_map[clock].port_id, &constants::one_val);
    xsi.run(clock_half_period);
}

void XpuTestBench::wait_low_clock() {
    //xsi.put_value(port_map[clock].port_id, &constants::zero_val);
    xsi.run(clock_half_period);
}

void XpuTestBench::wait_clock() {
    wait_low_clock();
    wait_high_clock();
}

bool XpuTestBench::axi_bvalid_ready() {
    unsigned int bvalid = read("s00_axi_bvalid");
    std::cout << "time:" << std::dec << xsi.get_time() << " bvalid value: " << bvalid << std::endl;
    return (bvalid & 0x1) != 0;
}

void XpuTestBench::wait_clock_cycles(unsigned int nr_clock_cycles) {
    for (unsigned i = 0; i < nr_clock_cycles - 1; i++) {
        wait_clock();
    }
    wait_low_clock();
    write("s00_axis_tvalid", 1);
    write("s00_axis_tlast", 1);
    write("m00_axis_tready", 1);
    wait_high_clock();
    wait_low_clock();
}

/*
 void XpuTestBench::wait_clock_cycles(unsigned int nr_clock_cycles)
{
    while(nr_clock_cycles)
    {
        nr_clock_cycles--;
    }
}
 */

XSI_INT64 XpuTestBench::get_time() {
    return xsi.get_time();
}

void XpuTestBench::generate_clock(int clock_half_period) {
    xsi.generate_clock(0, clock_half_period, clock_half_period);
}

void XpuTestBench::tb_init() {
    std::string simulation_accelerator_setup(
            std::to_string(ARRAY_NR_CELLS) + '_' + std::to_string(NR_PARTIAL_MULTICELLS_PER_MULTICELL) + '_' +
            std::to_string(NR_CELLS_PER_PARTIAL_MULTICELL));

    std::string axi_lite_program_file_path(
            SIMULATION_FILE_PATH_BACKSTEPS + "io_simulation_data/program_file/program_file_" +
            simulation_accelerator_setup + "_" + SIMULATION_FUNCTION_PERFORMED + "_" + SIMULATION_TEST_NAME + ".txt");

    std::string axi_lite_program_file_combined_path(
            SIMULATION_FILE_PATH_BACKSTEPS + "io_simulation_data/program_file/program_file_combined_" +
            simulation_accelerator_setup + "_" + SIMULATION_FUNCTION_PERFORMED + "_" + SIMULATION_TEST_NAME + ".txt");

    std::string axi_lite_function_arguments_file_path(
            SIMULATION_FILE_PATH_BACKSTEPS + "io_simulation_data/function_arguments/function_arguments_" +
            simulation_accelerator_setup + "_" + SIMULATION_FUNCTION_PERFORMED + "_" + SIMULATION_TEST_NAME + ".txt");

    std::string axi_stream_in_file_path(SIMULATION_FILE_PATH_BACKSTEPS + "io_simulation_data/input/axi_stream_input_" +
                                        simulation_accelerator_setup + "_" + SIMULATION_FUNCTION_PERFORMED + "_" +
                                        SIMULATION_TEST_NAME + ".txt");

    std::string axi_stream_out_file_path(
            SIMULATION_FILE_PATH_BACKSTEPS + "io_simulation_data/output/axi_stream_output_" +
            simulation_accelerator_setup + "_" + SIMULATION_FUNCTION_PERFORMED + "_" + SIMULATION_TEST_NAME + ".txt");

#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    std::string golden_model_file_path(SIMULATION_FILE_PATH_BACKSTEPS +
                                       "io_simulation_data/golden_model_results/golden_model_results_" +
                                       simulation_accelerator_setup +
                                       "_" +
                                        SIMULATION_FUNCTION_PERFORMED +
                                        "_" +
                                        SIMULATION_TEST_NAME +
                                        ".txt");
#endif
    axi_stream_out_file.open(axi_stream_out_file_path, std::ios::out);
    axi_stream_in_file.open(axi_stream_in_file_path, std::ios::in);
#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    golden_model_out_file.open(golden_model_file_path, std::ios::in);
#endif
    axi_lite_program_file_combined.open(axi_lite_program_file_combined_path, std::ios::out);
    axi_lite_program_file.open(axi_lite_program_file_path, std::ios::in);
    axi_lite_function_arguments_file.open(axi_lite_function_arguments_file_path, std::ios::in);

    test_done_ok = 0;
    xsi.put_value(port_map["resetn"].port_id, &constants::one_val);
    std::cout << "resetn: " << read("resetn") << std::endl;
    //AXI_init();
    std::cout << "Finished initilising testbench" << std::endl;
}

void XpuTestBench::AXI_init() {
    std::cout << "AXI STREAM INIT" << std::endl;
    std::regex regex_axi("^(s|m){1}[_00_axi_|_00_axis_]*[a-z]*");
    for (int i = 0; i < xsi.get_num_ports(); i++) {
        if (port_map[static_cast<std::string>(xsi.get_port_name(i))].is_input &&
            std::regex_match(xsi.get_port_name(i), regex_axi)) {
            write(xsi.get_port_name(i), 0);
            std::cout << i << " " << xsi.get_port_name(i) << " value: " << read(xsi.get_port_name(i)) << std::endl;
        }
    }
    std::cout << "FINISHED..." << std::endl;
}

void XpuTestBench::readAxiSignals() {
    std::cout << "Time: " << std::dec << get_time() << std::endl << std::endl;
    std::cout << " clock: " << read("clock") << std::endl;
    std::cout << " resetn: " << read("resetn") << std::endl << std::endl;

    std::cout << " SLAVE " << std::endl;
    std::cout << " s00_axi_awaddr: " << read("s00_axi_awaddr") << std::endl;
    std::cout << " s00_axi_awprot: " << read("s00_axi_awprot") << std::endl;
    std::cout << " s00_axi_awvalid: " << read("s00_axi_awvalid") << std::endl;
    std::cout << " s00_axi_awready: " << read("s00_axi_awready") << std::endl;
    std::cout << " s00_axi_wdata: " << read("s00_axi_wdata") << std::endl;
    std::cout << " s00_axi_wstrb: " << read("s00_axi_wstrb") << std::endl;
    std::cout << " s00_axi_wvalid: " << read("s00_axi_wvalid") << std::endl;
    std::cout << " s00_axi_wready: " << read("s00_axi_wready") << std::endl;
    std::cout << " s00_axi_bresp: " << read("s00_axi_bresp") << std::endl;
    std::cout << " s00_axi_bvalid: " << read("s00_axi_bvalid") << std::endl;
    std::cout << " s00_axi_bready: " << read("s00_axi_bready") << std::endl;
    std::cout << " s00_axi_araddr: " << read("s00_axi_araddr") << std::endl;
    std::cout << " s00_axi_arprot: " << read("s00_axi_arprot") << std::endl;
    std::cout << " s00_axi_arvalid: " << read("s00_axi_arvalid") << std::endl;
    std::cout << " s00_axi_arready: " << read("s00_axi_arready") << std::endl;
    std::cout << " s00_axi_rdata: " << read("s00_axi_rdata") << std::endl;
    std::cout << " s00_axi_rresp: " << read("s00_axi_rresp") << std::endl;
    std::cout << " s00_axi_rvalid: " << read("s00_axi_rvalid") << std::endl;
    std::cout << " s00_axi_rready: " << read("s00_axi_rready") << std::endl;
    std::cout << " s00_axis_tdata: " << read("s00_axis_tdata") << std::endl;
    std::cout << " s00_axis_tvalid: " << read("s00_axis_tvalid") << std::endl;
    std::cout << " s00_axis_tready: " << read("s00_axis_tready") << std::endl;
    std::cout << " s00_axis_tlast: " << read("s00_axis_tlast") << std::endl;

    std::cout << " MASTER " << std::endl;
    //std::cout<< " m00_axis_tdata: " << read("m00_axis_tdata") << std::endl << std::endl;
    std::cout << " m00_axis_tvalid: " << read("m00_axis_tvalid") << std::endl;
    std::cout << " m00_axis_tready: " << read("m00_axis_tready") << std::endl;
    std::cout << " m00_axis_tlast: " << read("m00_axis_tlast") << std::endl << std::endl;
}

int axi_write_count;

void XpuTestBench::AXI_Lite_write(uint32_t waddr, uint32_t wdata) {
    //posedge clock
    //while(0 == read("s00_axi_bvalid")); //wait for 1

    //readAxiSignals();

    write_addr(waddr);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 1);
    write_data(wdata);
    write("s00_axi_wstrb", (1 << (IO_INTF_PROG_AXILITE_DATA_SIZE / 8)) - 1);
    write("s00_axi_bready", 1);
    write("s00_axi_wvalid", 1);
    write("s00_axi_araddr", 0);
    write("s00_axi_arprot", 0);
    write("s00_axi_arvalid", 0);
    write("s00_axi_rready", 0);
    wait_high_clock();
    wait_low_clock();

    //readAxiSignals();

    //while(1 == read("s00_axi_bvalid")); //wait for 0
    while (read("s00_axi_bvalid") == 0) {
        wait_high_clock();
        wait_low_clock();
    }

    //readAxiSignals();

    while (read("s00_axi_bvalid") == 1) {
        wait_high_clock();
        wait_low_clock();
    }

    //negedge axilite_bvalid
    write("s00_axi_awaddr", 0);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 0);
    write("s00_axi_wdata", 0);
    write("s00_axi_wstrb", 0);
    write("s00_axi_wvalid", 0);
    write("s00_axi_bready", 0);

    if (axi_write_count == 1) {
        write("s00_axis_tvalid", 0);
        write("s00_axis_tlast", 0);
    }

    wait_high_clock();
    wait_low_clock();

    ++axi_write_count;
    std::cout << "written at: " << std::hex << "(0x" << waddr << ") data: " << std::hex << "(0x" << wdata << ")"
              << std::endl;
}

void XpuTestBench::AXI_Lite_read(uint32_t raddr) {
    write("s00_axi_awaddr", 0);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 0);
    write("s00_axi_wdata", 0);
    write("s00_axi_wstrb", 0);
    write("s00_axi_wvalid", 0);
    write("s00_axi_bready", 0);
    write("s00_axi_araddr", raddr);
    write("s00_axi_arprot", 0);
    write("s00_axi_arvalid", 1);
    write("s00_axi_rready", 1);
    wait_high_clock();
    wait_low_clock();

    while (read("s00_axi_arready") == 0) {
        wait_high_clock();
        wait_low_clock();
    }

    while (read("s00_axi_arready") == 1) {
        wait_high_clock();
        wait_low_clock();
    }

    write("s00_axi_arvalid", 0);

    std::cout << "araddr: " << read("s00_axi_araddr") << std::endl;

    if (read("s00_axi_rvalid"))
        std::cout << "AXI_Lite Read:" << std::hex << read("s00_axi_rdata") << std::endl;

    wait_high_clock();
    wait_low_clock();

    wait_high_clock();
    wait_low_clock();

    write("s00_axi_araddr", 0);
    write("s00_axi_rready", 0);

    wait_high_clock();
    wait_low_clock();

    wait_high_clock();
    wait_low_clock();
}

void XpuTestBench::AXI_Stream_SendStop() {
    write("m00_axis_tdata", 0);
    write("m00_axis_tvalid", 0);
    write("m00_axis_tlast", 0);
}

void XpuTestBench::AXI_Stream_SendData(unsigned int data) {

}

void XpuTestBench::AXI_Stream_receive_data() {

}

void XpuTestBench::AXI_Stream_stop_receive_data() {

}

/*
 * Extracts k bits from position p
 * rsh number by (p-1)
 * create a number with k set bits ((1<<k)-1)
 * AND between these operations
 * */

int extractBits(int number, int k, int p) {
    return (((1 << k) - 1) & (number >> (p - 1)));
}