//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <targets/sim/Constants.hpp>
#include <targets/sim/Tb.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <vector>

#include <fmt/core.h>

std::filesystem::path Tb::cLogFilePath            = getXpuHome() / "logs" / "xsim";
std::filesystem::path Tb::cWdbFilePath            = getXpuHome() / "logs" / "xsim";
const std::filesystem::path Tb::cSimulationLogDir = getXpuHome() / "logs" / "simulation_files";

[[maybe_unused]] constexpr unsigned cMaxAttemptsAxiIO = 10000;
constexpr unsigned cDrainTime                         = 100; // clock cycles

//-------------------------------------------------------------------------------------
Tb::Tb(
    const std::string& design_libname,
    const std::string& simkernel_libname,
    const std::string& clock_name,
    const std::string& reset_name,
    const Arch& arch,
    bool enableWdb,
    bool debugFilePrint,
    std::string_view log_suffix)
    : arch(arch), m_xsi{new Xsi::Loader{design_libname, simkernel_libname}} {
    createDirIfNotExists(cLogFilePath.parent_path());
    createDirIfNotExists(cWdbFilePath.parent_path());
    createDirIfNotExists(cSimulationLogDir);

    if (log_suffix != "") {
        cLogFilePath += ".";
        cLogFilePath += log_suffix;
        cWdbFilePath += ".";
        cWdbFilePath += log_suffix;
    }

    cLogFilePath += ".log";
    cWdbFilePath += ".wdb";
    // Load and open the TOP design
    logInit.print(fmt::format("Loading [{}][{}]...\n", design_libname, simkernel_libname));

    s_xsi_setup_info info;
    logFileNameCStr = new char[std::strlen(cLogFilePath.c_str()) + 1];
    std::strcpy(logFileNameCStr, cLogFilePath.c_str());
    info.logFileName = logFileNameCStr;
    logInit.print(fmt::format("Logfile: [{}]\n", info.logFileName));

    wdbFileNameCStr = new char[std::strlen(cWdbFilePath.c_str()) + 1];
    std::strcpy(wdbFileNameCStr, cWdbFilePath.c_str());
    info.wdbFileName = wdbFileNameCStr;
    logInit.print(fmt::format("Wdbfile: [{}]\n", info.wdbFileName));

    logInit.print(fmt::format("DebugFilePrint: [{}]\n", debugFilePrint));

    m_xsi->open(&info);

    if (enableWdb) {
        m_xsi->trace_all();

        logInit.print("trace_all done\n");
    }

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

    uint32_t clock_period_ns = read("clock_period");

    logInit.print(fmt::format("Clock period is {}\n", clock_period_ns));

    m_clock_half_period =
        (unsigned int) ((double) clock_period_ns * 10 * pow(10, -9) / m_xsi->get_time_precision() / 2);

    if (m_clock_half_period == 0)
        throw std::invalid_argument("Calculated half period is zero");

    // Results
    logInit.print(fmt::format("Identified {} top-level ports:\n", num_ports()));
    // List ports
    list_ports();
    logInit.print(
        fmt::format("Using {} as clock with half-period of {} simulation steps\n", m_clock, m_clock_half_period));
    logInit.print(fmt::format("Using {} as reset\n", m_reset));
    // At the beginning cycle count is ZERO
    m_cycle_half_count = 0;
    init(debugFilePrint);
}

Tb::~Tb() {
    runClockCycles(cDrainTime);
    write("is_simulation_final_clock_cycle", 1);
    runClockCycles(2);

    // close the simulation
    delete m_xsi;
    delete[] logFileNameCStr;
    delete[] wdbFileNameCStr;
}

std::size_t Tb::num_ports() {
    // return number of ports
    return m_port_map.size();
}

void Tb::reset() {
    doResetActive();
    runClockCycles(5);
    doResetInactive();
    runClockCycles(5);
}

bool comparator(std::pair<std::string, port_parameters>& x, std::pair<std::string, port_parameters>& y) {
    return y.second.port_id > x.second.port_id;
}

std::vector<std::pair<std::string, port_parameters>> sort(std::map<std::string, port_parameters>& ports) {
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
        logInit.print(fmt::format(
            "{}\t{}\t{}\t{}\n",
            it->first,
            it->second.port_id,
            it->second.port_bits,
            (it->second.is_input ? "Input" : "Output")));
    }
}

void Tb::doWrite(const std::string& port_name, uint32_t value) {
    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (!m_port_map[port_name].is_input)
        throw std::invalid_argument("Write called on output port");

    int nwords = (m_port_map[port_name].port_bits + 31) / 32;
    std::vector<s_xsi_vlog_logicval> logic_val(nwords + 1);
    logic_val.at(0) = s_xsi_vlog_logicval{value, 0};
    for (int i = 1; i < nwords + 1; i++) {
        logic_val.at(i) = s_xsi_vlog_logicval{0, 0};
    }
    m_xsi->put_value(m_port_map[port_name].port_id, logic_val.data());
}

void Tb::doWrite64(const std::string& port_name, uint64_t value) {
    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (!m_port_map[port_name].is_input)
        throw std::invalid_argument("Write called on output port");

    if (m_port_map[port_name].port_bits < 32) {
        throw std::invalid_argument(fmt::format("Port {} has less than 64 bits", port_name));
    }

    int nwords = (m_port_map[port_name].port_bits + 31) / 32;
    std::vector<s_xsi_vlog_logicval> logic_val(nwords);

    // The most significant bytes come first
    logic_val.at(0) = s_xsi_vlog_logicval{static_cast<uint32_t>(value), 0};
    logic_val.at(1) = s_xsi_vlog_logicval{static_cast<uint32_t>(value >> 32), 0};

    for (int i = 2; i < nwords; i++) {
        logic_val.at(i) = s_xsi_vlog_logicval{0, 0};
    }

    m_xsi->put_value(m_port_map[port_name].port_id, logic_val.data());
}

void Tb::doWrites() {
    for (const auto& [k, v] : toWriteOnNextCycle) {
        doWrite(k, v);
    }
    toWriteOnNextCycle.clear();

    for (const auto& [k, v] : toWriteOnNextCycle64) {
        doWrite64(k, v);
    }
    toWriteOnNextCycle64.clear();
}

void Tb::write(const std::string& port_name, uint32_t value) {
    toWriteOnNextCycle.insert({port_name, value});
}

void Tb::write64(const std::string& port_name, uint64_t value) {
    toWriteOnNextCycle64.insert({port_name, value});
}

uint32_t Tb::read(const std::string& port_name) {
    unsigned int nwords = (m_port_map[port_name].port_bits + 31) / 32;

    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (nwords > 1)
        throw std::invalid_argument(port_name + " uint = read(string name) applies only to signals of 32b or less");

    std::vector<s_xsi_vlog_logicval> logic_val(nwords);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());

    if (logic_val.at(0).bVal != 0) {
        logWork.print(fmt::format(
            "Warning: Reading from port {} which has X or Z bits set: {}\n",
            port_name,
            formatSimValue(&logic_val.at(0), m_port_map[port_name].port_bits)));
    }

    return logic_val.at(0).aVal;
}

uint64_t Tb::read64(const std::string& port_name) {
    unsigned int nwords = (m_port_map[port_name].port_bits + 31) / 32;

    if (!m_port_map.count(port_name))
        throw std::invalid_argument(port_name + " doesn't exist");

    if (nwords > 2)
        throw std::invalid_argument(port_name + " uint = read64(string name) applies only to signals of 64b or less");

    std::vector<s_xsi_vlog_logicval> logic_val(2);
    m_xsi->get_value(m_port_map[port_name].port_id, logic_val.data());

    if (logic_val.at(0).bVal != 0 || logic_val.at(1).bVal != 0) {
        logWork.print(fmt::format(
            "Warning: Reading from port {} which has X or Z bits set: {}{}\n",
            port_name,
            formatSimValue(&logic_val.at(0)),
            formatSimValue(&logic_val.at(1))));
    }

    return static_cast<uint64_t>(logic_val.at(1).aVal) << 32 | static_cast<uint64_t>(logic_val.at(0).aVal);
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

void Tb::runClockCycle() {
    m_xsi->run(2 * m_clock_half_period);
    doWrites();

    if (getSimSteps() >= m_max_sim_steps) {
	logWork.print(fmt::format("ERROR: Reached timeout simulation time. Quitting.\n"));
	exit(1);
    }
}

void Tb::runClockCycles(int _numberOfCycles) {
    for (int _i = 0; _i < _numberOfCycles; _i++) {
        runClockCycle();
    }
}

uint64_t Tb::getSimSteps() const {
    return m_xsi->get_time();
}

uint64_t Tb::getSimCycles() const {
    return m_xsi->get_time() / (2 * m_clock_half_period);
}

void Tb::setMaxSimSteps(uint64_t _maxSimSteps) {
    m_max_sim_steps = _maxSimSteps;
}

void Tb::setMaxSimCycles(uint64_t _maxSimCycles) {
    m_max_sim_steps = _maxSimCycles * 2 * m_clock_half_period;
}

void Tb::doResetInactive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::one_val);
}

void Tb::doResetActive() {
    m_xsi->put_value(m_port_map[m_reset].port_id, &constants::zero_val);
}

void Tb::generateClock(unsigned int period) {
    int clock_port = m_port_map[m_clock].port_id;
    m_xsi->generate_clock(clock_port, period, period);
}

void Tb::init(bool debugFilePrint) {
    generateClock(m_clock_half_period);

    doWrite("is_simulation_final_clock_cycle", 0);
    doWrite("has_debug_file_print", debugFilePrint ? 1 : 0);
    // Algin on posedge
    m_xsi->run(m_clock_half_period);

    AXI_init();

    reset();

}

void Tb::AXI_init() {
    logInit.print("AXI STREAM INIT\n");
    std::regex regex_axi("^(s|m){1}[_00_axi_|_00_axis_]*[a-z]*");
    for (int i = 0; i < m_xsi->get_num_ports(); i++) {
        if (m_port_map[static_cast<std::string>(m_xsi->get_port_name(i))].is_input
            && std::regex_match(m_xsi->get_port_name(i), regex_axi)) {
            write(m_xsi->get_port_name(i), 0);
        }
    }
}

#if 0

//-------------------------------------------------------------------------------------
void Tb::axiWrite(uint32_t wAddr, uint32_t wData) {
#ifdef XRT_FULL_IO_LOG
    uint64_t _beginTime = m_xsi->get_time() / 1000;
#endif

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
    runClockCycles(1);

    for (unsigned attempts = 0; read("s00_axi_bvalid") == 0; attempts++) {
        runClockCycles(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    for (unsigned attempts = 0; read("s00_axi_bvalid") == 1; attempts++) {
        runClockCycles(1);

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

    runClockCycles(1);

#ifdef XRT_FULL_IO_LOG
    uint64_t _endTime = m_xsi->get_time() / 1000;

    logAXILite.out().print("[{} ns -> {} ns] ", _beginTime, _endTime);
    logAXILite.out().print("write addr:{:08x} value:{}\n", wAddr, wData);
#endif
}

//-------------------------------------------------------------------------------------
unsigned int Tb::axiRead(uint32_t rAddr) {
#ifdef XRT_FULL_IO_LOG
    uint64_t _beginTime = m_xsi->get_time() / 1000;
#endif

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
    runClockCycles(1);

    for (unsigned attempts = 0; read("s00_axi_arready") == 0; attempts++) {
        runClockCycles(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    for (unsigned attempts = 0; read("s00_axi_arready") == 1; attempts++) {
        runClockCycles(1);

        if (attempts >= cMaxAttemptsAxiIO) {
            throw std::runtime_error("Simulator AXI Write timed out");
        }
    }

    write("s00_axi_arvalid", 0);

    runClockCycles(2);

    write("s00_axi_araddr", 0);
    write("s00_axi_rready", 0);

    runClockCycles(1);

    uint32_t ret = read("s00_axi_rdata");

#ifdef XRT_FULL_IO_LOG
    uint64_t _endTime = m_xsi->get_time() / 1000;

    logAXILite.out().print("[{} ns -> {} ns] ", _beginTime, _endTime);
    logAXILite.out().print("read  addr:{:08x} value:{}\n", rAddr, ret);
#endif

    return ret;
}

//-------------------------------------------------------------------------------------
void Tb::axiStreamWrite(std::span<const uint64_t> data) {
    assert(data.size() > 1);

#ifdef XRT_FULL_IO_LOG
    uint64_t _beginTime = m_xsi->get_time() / 1000;
#endif

    for (std::size_t i = 0; i < data.size(); i++) {
        write64("s00_axis_tdata", data[i]);
        write("s00_axis_tvalid", 1);
        write("s00_axis_tlast", (i == data.size() - 1) ? 1 : 0);

        unsigned attempts = 0;

        do {
            runClockCycles(1);

            if (attempts++ >= cMaxAttemptsAxiIO) {
                throw std::runtime_error("Simulator AXI Stream Write timed out");
            }
        } while (read("s00_axis_tready") == 0);
    }

    write("s00_axis_tdata", 0);
    write("s00_axis_tvalid", 0);
    write("s00_axis_tlast", 0);

#ifdef XRT_FULL_IO_LOG
    uint64_t _endTime = m_xsi->get_time() / 1000;

    logAXIStreamWrite.out().print("[{} ns -> {} ns]\n", _beginTime, _endTime);
#endif
}

//-------------------------------------------------------------------------------------
std::vector<uint64_t> Tb::axiStreamRead(std::size_t nvalues) {
    std::vector<uint64_t> data(nvalues);

#ifdef XRT_FULL_IO_LOG
    uint64_t _beginTime = m_xsi->get_time() / 1000;
#endif

    write("m00_axis_tready", 1);
    runClockCycles(1); // Maybe not??

    for (std::size_t i = 0; i < nvalues; ++i) {
        unsigned attempts = 0;

        while (read("m00_axis_tvalid") == 0) {
            runClockCycles(1);

            if (attempts++ >= cMaxAttemptsAxiIO) {
                throw std::runtime_error("Simulator AXI Stream Read timed out");
            }
        }

        data.at(i) = read64("m00_axis_tdata");

        // tlast signal is optional; and not set correctly set by the accelerator

        runClockCycles(1);
    }

    write("m00_axis_tready", 0);

#ifdef XRT_FULL_IO_LOG
    uint64_t _endTime = m_xsi->get_time() / 1000;

    logAXIStreamRead.out().print("[{} ns -> {} ns]\n", _beginTime, _endTime);
#endif

    return data;
}

#endif

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
