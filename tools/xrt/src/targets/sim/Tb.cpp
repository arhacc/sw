//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/generated/ArchConstants.hpp>
#include <targets/sim/Constants.hpp>
#include <targets/sim/Tb.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <vector>

#include <fmt/core.h>

const std::filesystem::path Tb::cLogFilePath = getXpuHome() / "logs" / "xsim.log";
const std::filesystem::path Tb::cWdbFilePath = getXpuHome() / "logs" / "xsim.wdb";

constexpr unsigned cMaxAttemptsAxiIO = 10000;

//-------------------------------------------------------------------------------------
Tb::Tb(
    const std::string& design_libname,
    const std::string& simkernel_libname,
    float clock_period_ns,
    const std::string& clock_name,
    const std::string& reset_name,
    const Arch& arch)
    : arch(arch), m_xsi{new Xsi::Loader{design_libname, simkernel_libname}} {
    // Load and open the TOP design
    std::cout << "Loading [" << design_libname << "][" << simkernel_libname << "]..."
              << std::endl;
    s_xsi_setup_info info;

    std::cout << "s_xsi_setup_info initalized" << std::endl;

    memset(&info, 0, sizeof(info));

    std::cout << "memset done[" << &info << "]" << std::endl;

    fmt::println(
        "Changing working directory path to {}",
        std::filesystem::path(design_libname).parent_path().parent_path().string());
    std::filesystem::current_path(
        std::filesystem::path(design_libname).parent_path().parent_path());

    logFileNameCStr = new char[std::strlen(cLogFilePath.c_str()) + 1];
    std::strcpy(logFileNameCStr, cLogFilePath.c_str());
    info.logFileName = logFileNameCStr;

    wdbFileNameCStr = new char[std::strlen(cWdbFilePath.c_str()) + 1];
    std::strcpy(wdbFileNameCStr, cWdbFilePath.c_str());
    info.wdbFileName = wdbFileNameCStr;

    m_xsi->open(&info);
    m_xsi->trace_all();

    std::cout << "trace_all done" << std::endl;

    // Get informations about ports
    for (int i = 0; i < m_xsi->get_num_ports(); i++) {
        std::string port_name(m_xsi->get_port_name(i));
        port_parameters params{i, m_xsi->get_port_bits(i), m_xsi->port_is_input(i)};
        m_port_map[port_name] = params;
    }
    // Check if reset and clock exist
    if (m_port_map.find(reset_name) == m_port_map.end())
        throw std::invalid_argument("Reset not found in ports list");

    if (m_port_map[reset_name].port_bits != 1)
        throw std::invalid_argument("Reset is not a scalar");

    if (!m_port_map[reset_name].is_input)
        throw std::invalid_argument("Reset is not an input port");

    m_reset = reset_name;

    if (m_port_map.find(clock_name) == m_port_map.end())
        throw std::invalid_argument("Clock not found in ports list");

    if (m_port_map[clock_name].port_bits != 1)
        throw std::invalid_argument("Clock is not a scalar");

    if (!m_port_map[clock_name].is_input)
        throw std::invalid_argument("Clock is not an input port");

    m_clock = clock_name;

    m_clock_half_period =
        (unsigned int) (clock_period_ns * 10 * pow(10, -9) / m_xsi->get_time_precision() / 2);

    if (m_clock_half_period == 0)
        throw std::invalid_argument("Calculated half period is zero");

    // Results
    std::cout << "Identified " << num_ports() << " top-level ports:" << std::endl;
    // List ports
    list_ports();
    std::cout << "Using " << m_clock << " as clock with half-period of "
              << m_clock_half_period << " simulation steps" << std::endl;
    std::cout << "Using " << m_reset << " as "
              << "reset" << std::endl;
    // At the beginning cycle count is ZERO
    m_cycle_half_count = 0;
    init();
}

Tb::~Tb() {
    // close the simulation
    delete m_xsi;
    delete[] logFileNameCStr;
    delete[] wdbFileNameCStr;
}

std::size_t Tb::num_ports() {
    // return number of ports
    return m_port_map.size();
}

bool comparator(
    std::pair<std::string, port_parameters>& x,
    std::pair<std::string, port_parameters>& y) {
    return y.second.port_id > x.second.port_id;
}

std::vector<std::pair<std::string, port_parameters>>
sort(std::map<std::string, port_parameters>& ports) {
    std::vector<std::pair<std::string, port_parameters>> copy_ports;
    for (auto& it : ports) {
        copy_ports.push_back(it);
    }
    std::sort(copy_ports.begin(), copy_ports.end(), comparator);
    return copy_ports;
}

void Tb::list_ports() {
    // list ports from the design
    std::vector<std::pair<std::string, port_parameters>>::iterator it;
    std::vector<std::pair<std::string, port_parameters>> ports = sort(m_port_map);
    for (it = ports.begin(); it != ports.end(); ++it) {
        // fmt::print( "{:<20} ID:{:<2} {:<2} bits {:<3} \n", it->first,
        // it->second.port_id, it->second.port_bits, (it->second.is_input ? "Input" :
        // "Output") );
        std::cout << it->first << "\t" << it->second.port_id << "\t"
                  << it->second.port_bits << "\t"
                  << (it->second.is_input ? "Input" : "Output") << std::endl;
    }
}

void Tb::write(const std::string& port_name, uint32_t value) {
    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (!m_port_map[port_name].is_input)
        throw std::invalid_argument("Write called on output port");

    int nwords = (m_port_map[port_name].port_bits + 31) / 32;
    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    logic_val.at(0) = (s_xsi_vlog_logicval){value, 0};
    for (int i = 1; i < nwords; i++) {
        logic_val.at(i) = (s_xsi_vlog_logicval){0, 0};
    }
    m_xsi->put_value(m_port_map[port_name].port_id, logic_val.data());
    // std::cout << port_name << ":" << read(port_name) << std::endl;
}

void Tb::write64(const std::string& port_name, uint64_t value) {
    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (!m_port_map[port_name].is_input)
        throw std::invalid_argument("Write called on output port");

    if (m_port_map[port_name].port_bits < 32) {
        throw std::invalid_argument(
            fmt::format("Port {} has less than 64 bits", port_name));
    }

    int nwords = (m_port_map[port_name].port_bits + 31) / 32;
    std::vector<s_xsi_vlog_logicval> logic_val(nwords);

    // The most significant bytes come first
    logic_val.at(0) = (s_xsi_vlog_logicval){static_cast<uint32_t>(value >> 32), 0};
    logic_val.at(1) = (s_xsi_vlog_logicval){static_cast<uint32_t>(value), 0};

    for (int i = 2; i < nwords; i++) {
        logic_val.at(i) = (s_xsi_vlog_logicval){0, 0};
    }

    m_xsi->put_value(m_port_map[port_name].port_id, logic_val.data());
    // std::cout << port_name << ":" << read(port_name) << std::endl;
}

uint32_t Tb::read(const std::string& port_name) {
    unsigned int nwords = (m_port_map[port_name].port_bits + 31) / 32;

    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (nwords > 1)
        throw std::invalid_argument(
            port_name
            + " uint = read(string name) applies only to signals of 32b or less");

    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());

    if (logic_val.at(0).bVal != 0) {
        fmt::println(
            "Warning: Reading from port {} which has X or Z bits set: {}",
            port_name,
            formatSimValue(&logic_val.at(0), m_port_map[port_name].port_bits));
    }

    return logic_val.at(0).aVal;
}

uint64_t Tb::read64(const std::string& port_name) {
    unsigned int nwords = (m_port_map[port_name].port_bits + 31) / 32;

    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (nwords > 2)
        throw std::invalid_argument(
            port_name
            + " uint = read64(string name) applies only to signals of 64b or less");

    std::vector<s_xsi_vlog_logicval> logic_val(2);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());

    if (logic_val.at(0).bVal != 0 || logic_val.at(1).bVal != 0) {
        fmt::println(
            "Warning: Reading from port {} which has X or Z bits set: {}{}",
            port_name,
            formatSimValue(&logic_val.at(0)),
            formatSimValue(&logic_val.at(1)));
    }

    return static_cast<uint64_t>(logic_val.at(0).aVal) << 32
           | static_cast<uint64_t>(logic_val.at(1).aVal);
}

void Tb::restart() {
    m_xsi->restart();
}

void Tb::write_addr(uint32_t address) {
    s_xsi_vlog_logicval val = {address, 0x0};
    m_xsi->put_value(m_port_map["s00_axi_awaddr"].port_id, &val);
}

void Tb::write_data(uint32_t data) {
    s_xsi_vlog_logicval val = {data, 0x0};
    m_xsi->put_value(m_port_map["s00_axi_wdata"].port_id, &val);
}

void Tb::wait_clock_cycle(int _numberOfCycles) {
    m_xsi->run(_numberOfCycles * 2 * m_clock_half_period);
}

XSI_INT64 Tb::getTime() const {
    return m_xsi->get_time();
}

void Tb::doResetInactive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::one_val);
    std::cout << "resetn: " << read(m_reset) << std::endl;
}

void Tb::doResetActive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::zero_val);
    std::cout << "resetn: " << read(m_reset) << std::endl;
}

void Tb::generateClock(unsigned int period) {
    int clock_port = m_port_map[m_clock].port_id;
    m_xsi->generate_clock(clock_port, period, period);
}

void Tb::init() {
    // doResetInactive();

    generateClock(m_clock_half_period);

    AXI_init();
    std::cout << "Finished initialising testbench" << std::endl;
}

void Tb::AXI_init() {
    std::cout << "AXI STREAM INIT" << std::endl;
    std::regex regex_axi("^(s|m){1}[_00_axi_|_00_axis_]*[a-z]*");
    for (int i = 0; i < m_xsi->get_num_ports(); i++) {
        if (m_port_map[static_cast<std::string>(m_xsi->get_port_name(i))].is_input
            && std::regex_match(m_xsi->get_port_name(i), regex_axi)) {
            write(m_xsi->get_port_name(i), 0);
            std::cout << i << " " << m_xsi->get_port_name(i)
                      << std::endl; // << " value: " << read(m_xsi->get_port_name(i)) <<
                                    // std::endl;
        }
    }
    std::cout << "FINISHED..." << std::endl;
}

//-------------------------------------------------------------------------------------
void Tb::axiWrite(uint32_t wAddr, uint32_t wData) {
    // std::cout << std::dec << "[AXI_LITE_WRITE] TIME: " << std::dec  << tb->getTime() <<
    // " wdata " << std::hex << wdata << std::endl;

    // posedge clock
    write_addr(wAddr);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 1);
    write_data(wData);
    write(
        "s00_axi_wstrb",
        (1 << (arch.get(ArchConstant::IO_INTF_PROG_AXILITE_DATA_SIZE) / 8)) - 1);
    write("s00_axi_bready", 1);
    write("s00_axi_wvalid", 1);
    write("s00_axi_araddr", 0);
    write("s00_axi_arprot", 0);
    write("s00_axi_arvalid", 0);
    write("s00_axi_rready", 0);
    wait_clock_cycle(1);

    for (unsigned attempts = 0; read("s00_axi_bvalid") == 0; attempts++) {
        wait_clock_cycle(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    for (unsigned attempts = 0; read("s00_axi_bvalid") == 1; attempts++) {
        wait_clock_cycle(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    // negedge axilite_bvalid
    write("s00_axi_awaddr", 0);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 0);
    write("s00_axi_wdata", 0);
    write("s00_axi_wstrb", 0);
    write("s00_axi_wvalid", 0);
    write("s00_axi_bready", 0);

    wait_clock_cycle(1);
}

//-------------------------------------------------------------------------------------
unsigned int Tb::axiRead(uint32_t rAddr) {
    write("s00_axi_awaddr", 0);
    write("s00_axi_awprot", 0);
    write("s00_axi_awvalid", 0);
    write("s00_axi_wdata", 0);
    write("s00_axi_wstrb", 0);
    write("s00_axi_wvalid", 0);
    write("s00_axi_bready", 0);
    write("s00_axi_araddr", rAddr);
    write("s00_axi_arprot", 0);
    write("s00_axi_arvalid", 1);
    write("s00_axi_rready", 1);
    wait_clock_cycle(1);

    for (unsigned attempts = 0; read("s00_axi_arready") == 0; attempts++) {
        wait_clock_cycle(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    for (unsigned attempts = 0; read("s00_axi_arready") == 1; attempts++) {
        wait_clock_cycle(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    write("s00_axi_arvalid", 0);

    //    std::cout << "araddr: " << tb->read("s00_axi_araddr") << std::endl;
    //
    //    if(tb->read("s00_axi_rvalid"))
    //        std::cout << "AXI_Lite Read:"  << std::hex << tb->read("s00_axi_rdata") <<
    //        std::endl;

    wait_clock_cycle(2);

    write("s00_axi_araddr", 0);
    write("s00_axi_rready", 0);

    wait_clock_cycle(1);

    return read("s00_axi_rdata");
}

//-------------------------------------------------------------------------------------
void Tb::axiStreamWrite(std::span<const uint64_t> data) {
    assert(data.size() > 1);

    for (std::size_t i = 0; i < data.size(); i++) {
        write64("s00_axis_tdata", data[i]);
        write("s00_axis_tvalid", 1);
        write("s00_axis_tlast", (i == data.size() - 1) ? 1 : 0);

        unsigned attempts = 0;

        do {
            wait_clock_cycle(1);

            if (attempts++ >= cMaxAttemptsAxiIO) {
                throw std::runtime_error("Simulator AXI Stream Write timed out");
            }
        } while (read("s00_axis_tready") == 0);
    }

    write("s00_axis_tdata", 0);
    write("s00_axis_tvalid", 0);
    write("s00_axis_tlast", 0);
}

//-------------------------------------------------------------------------------------
std::vector<uint64_t> Tb::axiStreamRead(std::size_t nvalues) {
    std::vector<uint64_t> data(nvalues);

    write("m00_axis_tready", 1);
    wait_clock_cycle(1); // Maybe not??

    for (std::size_t i = 0; i < nvalues; ++i) {
        unsigned attempts = 0;

        while (read("m00_axis_tvalid") == 0) {
            wait_clock_cycle(1);

            if (attempts++ >= cMaxAttemptsAxiIO) {
                throw std::runtime_error("Simulator AXI Stream Read timed out");
            }
        }

        data.at(i) = read64("m00_axis_tdata");

        // tlast signal is optional; and not set correctly set by the accelerator
#if 0
        if (read("m00_axis_tvalid") != ((i == nvalues - 1) ? 1 : 0)) {
            fmt::println(
                "Warning: Unexpected or missing tlast in transfer at {}/{}",
                i + 1,
                nvalues);
        }
#endif

        wait_clock_cycle(1);
    }

    write("m00_axis_tready", 0);

    return data;
}

//-------------------------------------------------------------------------------------
std::string Tb::formatSimValue(p_xsi_vlog_logicval val, std::uint8_t bits) {
    std::string s;

    assert(bits <= 32 && bits > 0);

    for (std::int8_t i = bits - 1; i >= 0; i--) {
        std::uint8_t bitA = (val->aVal >> i) & 1;
        std::uint8_t bitB = (val->bVal >> i) & 1;

        switch (bitA << 1 | bitB) {
            case 0b00:
                s += "0";
                break;
            case 0b10:
                s += "1";
                break;
            case 0b11:
                s += "X";
                break;
            case 0b01:
                s += "Z";
                break;
        }
    }

    return s;
}
