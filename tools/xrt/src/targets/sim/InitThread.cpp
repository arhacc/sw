#include "targets/sim/InitThread.hpp"

#include <bitset>
#include <cstdint>
#include <fstream>

void InitThread::run() {
    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Started: " << getCurrentThreadName() << std::endl;

    pDUT->init();
    pDUT->AXI_init();
    pDUT->doResetActive();
    wait_clock_cycles(9);
    wait_clock_hcycle();
    pDUT->doResetInactive();
    wait_clock_cycles(30);

    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Ended: " << getCurrentThreadName() << std::endl;
}

InitThread::InitThread(
    Sync& sync, DUT* pdut, std::vector<std::string>& dataIn, std::span<uint32_t> dataFile)
    : ClockConsumerThread(sync, pdut, "InitThread"), _dataIn(dataIn) {
    // write data in (AXI Stream)
    static int k{0}; // TOASK: why static

    std::bitset<NR_CELLS_PER_PARTIAL_MULTICELL * DATA_SIZE> assembled_file_data{0};
    std::bitset<NR_CELLS_PER_PARTIAL_MULTICELL * DATA_SIZE> data_in_file{0};

    if (SIMULATION_HAS_DATA_INPUT_FILE == 1) {
        for (uint32_t data_word : dataFile) /*while or for used for iterating over data*/
        {
            data_in_file        = data_word; /*data vector/matrix from xrt*/
            assembled_file_data = (data_in_file <<= ((DATA_SIZE) *k)) |=
                assembled_file_data;
            ++k;

            if (k % NR_CELLS_PER_PARTIAL_MULTICELL == 0) {
                _dataIn.push_back(assembled_file_data.to_string());

                assembled_file_data = 0;
                k                   = 0;
            }
        }
    }
}