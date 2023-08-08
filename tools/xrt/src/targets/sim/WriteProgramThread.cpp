#include "targets/sim/WriteProgramThread.hpp"

#include <cstdint>
#include <sstream>

void WriteProgramThread::run() {
    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Started: " << getCurrentThreadName() << std::endl;

    Thread::sleepm(10);

    std::bitset<(INSTR_OPERAND_LOC_UPPER) - (INSTR_OPERAND_LOC_LOWER) + 1> isa_ctl(
        ISA_ctl);
    std::bitset<(INSTR_OPCODE_LOC_UPPER) - (INSTR_OPCODE_LOC_LOWER) + 1> isa_prun(
        ISA_prun); // TO LOOK IN DEFINES AND CORRECT THEM
    std::bitset<(INSTR_OPERAND_LOC_UPPER) - (INSTR_OPERAND_LOC_LOWER) + 1>
        prog_data_operand;
    std::bitset<(INSTR_OPCODE_LOC_UPPER) - (INSTR_OPCODE_LOC_LOWER) + 1> prog_data_opcode;

    unsigned long prog_data_controller = 0;
    unsigned long prog_data_array      = 0;

    /*for iterating in program data vector*/
    for (size_t i = 0; i < programFile.size(); i += 2) {
        assert(i + 1 < programFile.size());

        prog_data_controller = programFile[i];
        prog_data_array      = programFile[i + 1];

        AxiStreamWrite(
            0,
            prog_data_controller); /*if the vector contains prog data from controller and
                                    *array one after another we can leave only v[i];*/
        AxiStreamWrite(0, prog_data_array);

#if 0

        // Create a bitset class for easier comparations
        prog_data_operand = extractBits(
            prog_data_controller,
            INSTR_OPERAND_LOC_UPPER - INSTR_OPERAND_LOC_LOWER + 1,
            INSTR_OPERAND_LOC_LOWER);

        prog_data_opcode = extractBits(
            prog_data_controller,
            INSTR_OPCODE_LOC_UPPER - INSTR_OPCODE_LOC_LOWER + 1,
            INSTR_OPCODE_LOC_LOWER);

        // std::cout << "line: " << count_lines << " prog_data_operand: " <<
        // prog_data_operand << ' ' << " prog_data_opcode: " << prog_data_opcode <<
        // std::endl;

        if (SIMULATION_HAS_FUNCTION_ARGUMENTS_FILE) {
            // if had prun, read parameters; each prun requires its own line with
            // arguments (even if it is an empty line)
            if (prog_data_operand == isa_ctl && prog_data_opcode == isa_prun) {
                // std::cout << "Line: " << count_lines << " prog_data_controller: " <<
                // prog_data_controller << std::endl;

                int read_function_arguments_error_code{0};

                // Read each line of the input file
                std::string line;
                std::getline(axi_lite_function_arguments_file, line);
                read_function_arguments_error_code = (line == "\r") ? 0 : 1;

                if (read_function_arguments_error_code != 0) {
                    int found_space_position{0};

                    while ((read_function_arguments_error_code != 0)
                           & (found_space_position != -1)) {
                        std::istringstream iss(line);
                        int number{0};

                        // Read each integer in the line
                        iss >> number;
                        // std::cout << "number: " << (int)number << std::endl;

                        AxiStreamWrite(0, (int) number);
                        AxiStreamWrite(0, 0);
                        // test1 << pDUT->getTime() << ' ' << pDUT->read("s00_axi_wdata")
                        // << '\n';

                        axi_lite_program_file_combined << std::hex << (int) number << ' '
                                                       << 0 << '\n';

                        found_space_position = line.find_first_of(" ");
                        line = line.substr(found_space_position + 1, line.length() - 1);
                    }
                }
            }
        }
#endif
    }
    // std::cout << std::endl << "Counted " << std::dec << count_lines << " program file
    // lines" << std::endl;

    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Start: AxiLiteRead" << std::endl;

    // wait interrupt
    unsigned int axilite_read_data{0};
    unsigned int extractBit{
        extractAxiBits(axilite_read_data, 1, XPU_STATUS_REG_WAIT_ACK_LOC + 1)};

    while (extractBit == 0) {
        // test1 << "BEFORE: axilite_read_data " << axilite_read_data << " extractBit: "
        // << extractBit << std::endl;
        axilite_read_data = AxiStreamRead(IO_INTF_AXILITE_READ_REGS_STATUS_REG_ADDR);
        wait_clock_cycle();

        extractBit =
            extractAxiBits(axilite_read_data, 1, XPU_STATUS_REG_WAIT_ACK_LOC + 1);
        if (pDUT->getTime() >= 100000000)
            break;
        // test1 << "AFTER: axilite_read_data " << axilite_read_data << " extractBit: " <<
        // extractBit << std::endl;
    }

    // interrupt ack
    uint32_t value = 0x0000'0001;
    AxiStreamWrite(IO_INTF_AXILITE_WRITE_REGS_INT_ACK_ADDR, value);
    wait_clock_cycles(5);
    wait_clock_cycles(5);

#ifndef SIMULATION_NR_DATA_OUT_READ
    wait_clock_cycles(SIMULATION_DRAIN_TIME_DATA_OUT_CLOCK_CYCLES);
#endif
    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Ended: " << getCurrentThreadName() << std::endl;
}

WriteProgramThread::WriteProgramThread(
    Sync& sync, DUT* pdut, std::span<uint32_t> programFile)
    : AxiLite(sync, pdut, "WriteProgramThread"), programFile(programFile) {}
