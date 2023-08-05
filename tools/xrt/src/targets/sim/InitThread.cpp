#include <bitset>
#include <fstream>

#include "targets/sim/InitThread.hpp"

void InitThread::run()
{
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Started: " << getCurrentThreadName() << std::endl;

    pDUT->init();
    pDUT->AXI_init();
    pDUT->doResetActive();
    wait_clock_cycles(9);
    wait_clock_hcycle();
    pDUT->doResetInactive();
    wait_clock_cycles(30);

    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Ended: " << getCurrentThreadName() << std::endl;
}

InitThread::InitThread(Sync& sync, DUT* pdut, std::vector<std::string>& dataIn):
    ClockConsumerThread(sync, pdut, "InitThread"),
    _dataIn(dataIn){

    std::string simulation_accelerator_setup(std::to_string(ARRAY_NR_CELLS) +
                                             '_' +
                                             std::to_string(NR_PARTIAL_MULTICELLS_PER_MULTICELL) +
                                             '_' +
                                             std::to_string(NR_CELLS_PER_PARTIAL_MULTICELL));

    std::string axi_stream_in_file_path(SIMULATION_FILE_BASEPATH +
                                        "cmake-build-debug/io_simulation_data/input/axi_stream_input_" +
                                        simulation_accelerator_setup +
                                        "_" +
                                        SIMULATION_FUNCTION_PERFORMED +
                                        "_" +
                                        SIMULATION_TEST_NAME +
                                        ".txt");


#ifdef SIMULATION_GOLDEN_MODEL_CHECKS
    std::string golden_model_file_path(SIMULATION_FILE_BASEPATH +
                                       "cmake-build-debug/io_simulation_data/golden_model_results/golden_model_results_" +
                                       simulation_accelerator_setup +
                                       "_" +
                                       SIMULATION_FUNCTION_PERFORMED +
                                       "_" +
                                       SIMULATION_TEST_NAME +
                                       ".txt");
    pDUT->golden_model_out_file.open(golden_model_file_path, std::ios::in);
#endif

    axi_stream_in_file.open(axi_stream_in_file_path, std::ios::in);
    //write data in (AXI Stream)
    static int k{0};
    int data_read_from_data_in_file;

    std::bitset<NR_CELLS_PER_PARTIAL_MULTICELL * DATA_SIZE> assembled_file_data{0};
    std::bitset<NR_CELLS_PER_PARTIAL_MULTICELL * DATA_SIZE> data_in_file{0};
    std::vector<std::string> assembled_data;

    if(!axi_stream_in_file.is_open()) {
        std::cerr << "ERROR: axi_stream_in_file is not open!" << std::endl;
        std::cout << axi_stream_in_file_path << std::endl;
        exit(0);
    }

    if(SIMULATION_HAS_DATA_INPUT_FILE == 1)
    {
        while(!axi_stream_in_file.eof())
        {
            axi_stream_in_file >> data_read_from_data_in_file;
            data_in_file = data_read_from_data_in_file;
            assembled_file_data = (data_in_file <<= ((DATA_SIZE) * k)) |= assembled_file_data;
            ++k;

            if (k % NR_CELLS_PER_PARTIAL_MULTICELL == 0)
            {
                _dataIn.push_back(assembled_file_data.to_string());

                assembled_file_data = 0;
                k = 0;
            }
        }
    }

    axi_stream_in_file.close();
}