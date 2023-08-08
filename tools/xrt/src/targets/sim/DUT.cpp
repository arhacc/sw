#include "targets/sim/DUT.hpp"

#include <cstring>

DUT::DUT(
    const std::string& design_libname,
    const std::string& simkernel_libname,
    float clock_period_ns,
    const std::string& clock_name,
    const std::string& reset_name)
    : m_xsi{new Xsi::Loader{design_libname, simkernel_libname}} {
    // Load and open the TOP design
    std::cout << "Created shared object to: xsim.dir/simulator_axi/xsimk.so" << std::endl;
    s_xsi_setup_info info;

    std::cout << "s_xsi_setup_info initalized" << std::endl;

    memset(&info, 0, sizeof(info));

    std::cout << "memset done" << std::endl;

    info.logFileName = nullptr;
    char wdbName[]   = "test.wdb";
    info.wdbFileName = wdbName;
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
}

DUT::~DUT() {
    // close the simulation
    delete m_xsi;
}

std::size_t DUT::num_ports() {
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

void DUT::list_ports() {
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

void DUT::write(const std::string& port_name, const std::string_view& value) {
    if (!m_port_map.count(port_name)) {
        throw std::invalid_argument(port_name + " doesn't exist");
    }

    if (!m_port_map[port_name].is_input) {
        throw std::invalid_argument("Write called on output port");
    }

    if (value.size() != 64 && m_port_map[port_name].port_bits != 64) {
        throw std::invalid_argument(
            "Write called on " + std::to_string(m_port_map[port_name].port_bits)
            + " bits! Port " + port_name + " needs to have 64b ");
    }

    std::size_t nwords = (m_port_map[port_name].port_bits + 64) / 64;
    std::vector<s_xsi_vlog_logicval> values(nwords);

    std::bitset<32> LSB{static_cast<std::string>(value.substr(0, 32))};
    std::bitset<32> MSB{static_cast<std::string>(value.substr(32, 64))};

    values.at(0) = (s_xsi_vlog_logicval){static_cast<unsigned int>(MSB.to_ulong()), 0};
    values.at(1) = (s_xsi_vlog_logicval){static_cast<unsigned int>(LSB.to_ulong()), 0};
    m_xsi->put_value(m_port_map[port_name].port_id, values.data());
}

void DUT::write(const std::string& port_name, unsigned int value) {
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

unsigned int DUT::read(const std::string& port_name) {
    unsigned int nwords = (m_port_map[port_name].port_bits + 31) / 32;

    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (nwords > 1)
        throw std::invalid_argument(
            port_name
            + " uint = read(string name) applies only to signals of 32b or less");

    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());
    return logic_val.at(0).aVal;
}

std::map<unsigned int, unsigned int> DUT::get64Value(const std::string port_name) {
    std::vector<s_xsi_vlog_logicval> logic_val(2);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());
    std::map<unsigned int, unsigned int> map;
    map.insert({logic_val.at(1).aVal, logic_val.at(0).aVal});
    return map;
}

void DUT::restart() {
    m_xsi->restart();
}

void DUT::write_addr(uint32_t address) {
    s_xsi_vlog_logicval val = {address, 0x0};
    m_xsi->put_value(m_port_map["s00_axi_awaddr"].port_id, &val);
}

void DUT::write_data(uint32_t data) {
    s_xsi_vlog_logicval val = {data, 0x0};
    m_xsi->put_value(m_port_map["s00_axi_wdata"].port_id, &val);
}

void DUT::_wait_half_period_cycle() {
    // m_xsi->put_value(m_port_map[clock].port_id, &constants::one_val);
    m_xsi->run(m_clock_half_period);
    m_cycle_half_count++;
}

void DUT::_wait_clock_cycle() {
    _wait_clock_cycles(1);
}

void DUT::_wait_clock_cycles(int clock_cycles) {
    for (int i = 0; i < clock_cycles; i++) {
        _wait_half_period_cycle();
        _wait_half_period_cycle();
    }
}

XSI_INT64 DUT::getTime() const {
    return m_xsi->get_time();
}

void DUT::doResetInactive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::one_val);
    // std::cout << "resetn: " << read(m_reset) << std::endl;
}

void DUT::doResetActive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::zero_val);
    // std::cout << "resetn: " << read(m_reset) << std::endl;
}

void DUT::generateClock(unsigned int period) {
    int clock_port = m_port_map[m_clock].port_id;
    m_xsi->generate_clock(clock_port, period, period);
}

unsigned int DUT::getHalfClockPeriod() const {
    return m_clock_half_period;
}

std::vector<unsigned int> DUT::keepAndRemove(const std::vector<unsigned int>& arr) {
    std::vector<unsigned int> result;
    std::size_t n = arr.size();
    std::size_t i = 0;

    while (i < n) {
        result.push_back(arr[i]);
        result.push_back(arr[i + 1]);
        i += 4;

        if (n - i < 2)
            break;
    }

    return result;
}

int DUT::getNoBits(const char* port_name) {
    return m_port_map[port_name].port_bits;
}

void DUT::init() {
    // doResetInactive();
    std::cout << "Finished initilising testbench" << std::endl;
}

void DUT::AXI_init() {
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

void DUT::readAxiSignals() {
    std::cout << "Time: " << std::dec << getTime() << std::endl << std::endl;
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
    // std::cout<< " s00_axis_tdata: " << read("s00_axis_tdata") << std::endl;
    std::cout << " s00_axis_tvalid: " << read("s00_axis_tvalid") << std::endl;
    std::cout << " s00_axis_tready: " << read("s00_axis_tready") << std::endl;
    std::cout << " s00_axis_tlast: " << read("s00_axis_tlast") << std::endl;

    std::cout << " MASTER " << std::endl;
    // std::cout<< " m00_axis_tdata: " << read("m00_axis_tdata") << std::endl <<
    // std::endl;
    std::cout << " m00_axis_tvalid: " << read("m00_axis_tvalid") << std::endl;
    std::cout << " m00_axis_tready: " << read("m00_axis_tready") << std::endl;
    std::cout << " m00_axis_tlast: " << read("m00_axis_tlast") << std::endl << std::endl;
}

int DUT::getStatus() {
    return m_xsi->get_status();
}

const char* DUT::getError() {
    return m_xsi->get_error_info();
}
