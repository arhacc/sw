#include "targets/sim/WriteProgramThread.hpp"


void WriteProgramThread::run() {
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Started: " << getCurrentThreadName() << std::endl;

    std::string simulation_accelerator_setup(std::to_string(ARRAY_NR_CELLS) +
                                             '_' +
                                             std::to_string(NR_PARTIAL_MULTICELLS_PER_MULTICELL) +
                                             '_' +
                                             std::to_string(NR_CELLS_PER_PARTIAL_MULTICELL));

    std::string axi_lite_program_file_path(SIMULATION_FILE_BASEPATH +
                                           "cmake-build-debug/io_simulation_data/program_file/program_file_" +
                                           simulation_accelerator_setup +
                                           "_" +
                                           SIMULATION_FUNCTION_PERFORMED +
                                           "_" +
                                           SIMULATION_TEST_NAME +
                                           ".txt");

    std::string axi_lite_program_file_combined_path(SIMULATION_FILE_BASEPATH +
                                                    "cmake-build-debug/io_simulation_data/program_file/program_file_combined_" +
                                                    simulation_accelerator_setup +
                                                    "_" +
                                                    SIMULATION_FUNCTION_PERFORMED +
                                                    "_" +
                                                    SIMULATION_TEST_NAME +
                                                    ".txt");

    std::string axi_lite_function_arguments_file_path(SIMULATION_FILE_BASEPATH +
                                                      "cmake-build-debug/io_simulation_data/function_arguments/function_arguments_" +
                                                      simulation_accelerator_setup +
                                                      "_" +
                                                      SIMULATION_FUNCTION_PERFORMED +
                                                      "_" +
                                                      SIMULATION_TEST_NAME +
                                                      ".txt");

    //std::fstream test1("test1.txt", std::ios::out);

    axi_lite_program_file_combined.open(axi_lite_program_file_combined_path, std::ios::out);
    axi_lite_program_file.open(axi_lite_program_file_path, std::ios::in);
    axi_lite_function_arguments_file.open(axi_lite_function_arguments_file_path, std::ios::in);

    std::string prog_data_array;
    //prog_data_controller is used for extracting a line within fstream axi_lite_program_file and then is going to represent prog_data_string
    //after prog_data_array was extracted
    int count_lines{0};

    if(!axi_lite_program_file.is_open()) {
        std::cerr << "ERROR: axi_lite_program_file is not open!" << std::endl;
        std::cerr << axi_lite_program_file_path << std::endl;
        exit(0);
    }

    if(!axi_lite_function_arguments_file.is_open()) {
        std::cerr << "ERROR: axi_lite_function_arguments_file is not open!" << std::endl;
        std::cerr << axi_lite_function_arguments_file_path << std::endl;
        exit(0);
    }

    if(!axi_lite_program_file_combined.is_open()) {
        std::cerr << "ERROR: axi_lite_program_file_combined is not open!" << std::endl;
        std::cerr << axi_lite_program_file_combined_path << std::endl;
        exit(0);
    }

    Thread::sleepm(10);
    //pDUT->write("m00_axis_tready", 1);

    std::bitset<(INSTR_OPERAND_LOC_UPPER)-(INSTR_OPERAND_LOC_LOWER)+1> isa_ctl(ISA_ctl);
    std::bitset<(INSTR_OPCODE_LOC_UPPER)-(INSTR_OPCODE_LOC_LOWER)+1> isa_prun(ISA_prun); //TO LOOK IN DEFINES AND CORRECT THEM
    std::bitset<(INSTR_OPERAND_LOC_UPPER)-(INSTR_OPERAND_LOC_LOWER)+1> prog_data_operand;
    std::bitset<(INSTR_OPCODE_LOC_UPPER)-(INSTR_OPCODE_LOC_LOWER)+1> prog_data_opcode;



    for(std::string prog_data_controller; std::getline(axi_lite_program_file, prog_data_controller, '\n'); )
    {
        count_lines++;
        prog_data_array = prog_data_controller.substr(9,17);
        prog_data_controller.replace(8, prog_data_controller.size(), " ");
        prog_data_controller.pop_back();

        AxiStreamWrite(0, std::stoul(prog_data_controller, nullptr, 16));
        AxiStreamWrite(0, std::stoul(prog_data_array, nullptr, 16));


        //write prog_data_controller and prog_data_array to file: axi_lite_program_file_combined
        axi_lite_program_file_combined << prog_data_controller << " " << prog_data_array << '\n';

        //Create a bitset class for easier comparations
        prog_data_operand = extractBits(std::stoul(prog_data_controller, nullptr, 16),
                                        INSTR_OPERAND_LOC_UPPER-INSTR_OPERAND_LOC_LOWER+1,
                                        INSTR_OPERAND_LOC_LOWER);


        prog_data_opcode = extractBits(std::stoul(prog_data_controller, nullptr, 16),
                                       INSTR_OPCODE_LOC_UPPER-INSTR_OPCODE_LOC_LOWER+1,
                                       INSTR_OPCODE_LOC_LOWER);

        //std::cout << "line: " << count_lines << " prog_data_operand: " << prog_data_operand << ' ' << " prog_data_opcode: " << prog_data_opcode << std::endl;

        if(SIMULATION_HAS_FUNCTION_ARGUMENTS_FILE)
        {
            // if had prun, read parameters; each prun requires its own line with arguments (even if it is an empty line)
            if(prog_data_operand == isa_ctl && prog_data_opcode == isa_prun)
            {
                //std::cout << "Line: " << count_lines << " prog_data_controller: " << prog_data_controller << std::endl;

                int read_function_arguments_error_code{0};

                // Read each line of the input file
                std::string line;
                std::getline(axi_lite_function_arguments_file, line);
                read_function_arguments_error_code = (line == "\r") ? 0 : 1;

                if(read_function_arguments_error_code != 0 )
                {
                    int found_space_position{0};

                    while((read_function_arguments_error_code != 0) & (found_space_position != -1))
                    {
                        std::istringstream iss(line);
                        int number{0};

                        // Read each integer in the line
                        iss >> number;
                        //std::cout << "number: " << (int)number << std::endl;

                        AxiStreamWrite(0, (int)number);
                        AxiStreamWrite(0, 0);
                        //test1 << pDUT->getTime() << ' ' << pDUT->read("s00_axi_wdata") << '\n';

                        axi_lite_program_file_combined << std::hex << (int)number << ' ' << 0 << '\n';

                        found_space_position = line.find_first_of(" ");
                        line = line.substr(found_space_position + 1, line.length() - 1);
                    }
                }
            }
        }
     }
    //std::cout << std::endl << "Counted " << std::dec << count_lines << " program file lines" << std::endl;

    axi_lite_program_file.close();
    axi_lite_program_file_combined.close();
    axi_lite_function_arguments_file.close();
    //test1.close();
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Start: AxiLiteRead" << std::endl;

    //wait interrupt
    unsigned int axilite_read_data{0};
    unsigned int extractBit{extractAxiBits(axilite_read_data, 1, XPU_STATUS_REG_WAIT_ACK_LOC + 1)};
    //std::fstream test1("test1.txt", std::ios::out);
    unsigned int val;
//    std::map<unsigned int, unsigned int> map;

    while(extractBit == 0)
    {
        //test1 << "BEFORE: axilite_read_data " << axilite_read_data << " extractBit: " << extractBit << std::endl;
        axilite_read_data = AxiStreamRead(IO_INTF_AXILITE_READ_REGS_STATUS_REG_ADDR);
        wait_clock_cycle();

        extractBit = extractAxiBits(axilite_read_data, 1, XPU_STATUS_REG_WAIT_ACK_LOC + 1 );
        if(pDUT->getTime() >= 100000000)
            break;
        //test1 << "AFTER: axilite_read_data " << axilite_read_data << " extractBit: " << extractBit << std::endl;
    }

    //interrupt ack
    uint32_t value = 0x0000'0001;
    AxiStreamWrite(IO_INTF_AXILITE_WRITE_REGS_INT_ACK_ADDR, value);
    wait_clock_cycles(5);
    wait_clock_cycles(5);

#ifndef SIMULATION_NR_DATA_OUT_READ
    wait_clock_cycles(SIMULATION_DRAIN_TIME_DATA_OUT_CLOCK_CYCLES);
#endif
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Ended: " << getCurrentThreadName() << std::endl;
}

WriteProgramThread::WriteProgramThread(Sync& sync, DUT* pdut) :
        AxiLite(sync, pdut, "WriteProgramThread"){
}
