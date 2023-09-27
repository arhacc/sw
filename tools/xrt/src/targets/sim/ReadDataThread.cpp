#include "targets/sim/ReadDataThread.hpp"

void ReadDataThread::run() {
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Started: " << getCurrentThreadName() << std::endl;

    std::string simulation_accelerator_setup(std::to_string(ARRAY_NR_CELLS) +
                                             '_' +
                                             std::to_string(NR_PARTIAL_MULTICELLS_PER_MULTICELL) +
                                             '_' +
                                             std::to_string(NR_CELLS_PER_PARTIAL_MULTICELL));

    std::string axi_stream_out_file_path(SIMULATION_FILE_BASEPATH +
                                         "io_simulation_data/output/axi_stream_output_" +
                                         simulation_accelerator_setup +
                                         "_" +
                                         SIMULATION_FUNCTION_PERFORMED +
                                         "_" +
                                         SIMULATION_TEST_NAME +
                                         ".txt");

    axi_stream_out_file.open(axi_stream_out_file_path, std::ios::out);
//    std::fstream t;
//    t.open("test.txt", std::ios::out);

    //read data out
    int i3{0};
    int i2{0};

    Thread::sleepm(10);

    std::vector<int> test;

    for(i2 = 0; i2 < SIMULATION_NR_DATA_OUT_READ; i2 += NR_CELLS_PER_PARTIAL_MULTICELL) {
        AxiStreamReceiveData();
    }
//
//        test.push_back(0);
//        //std::vector<s_xsi_vlog_logicval> data_write_to_data_out_file_assembled;
//        //data_write_to_data_out_file_assembled = pDUT->get64Value("m00_axis_tdata");
//        unsigned int data_write_to_data_out_file;
//
//        unsigned int m_tlast = pDUT->read("m00_axis_tlast");
////        for(i3 = 0; i3 < NR_CELLS_PER_PARTIAL_MULTICELL; i3 = i3 + 1)
////        {
////            //data_write_to_data_out_file = data_write_to_data_out_file_assembled[i3].aVal >> (i3 * DATA_SIZE) & 0x0001;
////            data_write_to_data_out_file = 0;
////        }
////        if( m_tlast == 1)
////        {
////            axi_stream_out_file << "\n";
////#ifdef SIMULATION_DATA_OUT_FILE_ADD_NEW_LINE_AFTER_NR_LINES_READ
////            if ((i2 + i3) % (ARRAY_NR_CELLS * SIMULATION_DATA_OUT_FILE_ADD_NEW_LINE_AFTER_NR_LINES_READ) == 0)
////                axi_stream_out_file << "\n";
////#endif
////        }
//        //data_write_to_data_out_file_assembled.clear();
//
//#ifndef SIMULATION_NR_DATA_OUT_READ
//        i2 = i2 + NR_CELLS_PER_PARTIAL_MULTICELL;
//#endif
//    }
    axi_stream_out_file.close();
    wait_clock_cycles(5);
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Ended: " << getCurrentThreadName() << std::endl;
}

void ReadDataThread::AxiStreamReceiveData()
{
//    std::fstream t;
//    t.open("test.txt", std::ios::out);

    pDUT->write("m00_axis_tready", 1);
    wait_clock_cycle();

//    unsigned int m_tvalid = pDUT->read("m00_axis_tvalid");
//    unsigned int m_tready = pDUT->read("m00_axis_tready");
//
//    while( m_tready == 0 || m_tvalid == 0 ){
//        m_tvalid = pDUT->read("m00_axis_tvalid");
//        m_tready = pDUT->read("m00_axis_tready");
//        std::cout << "Time: " << pDUT->getTime() << " m_tready: " << m_tready << " m_tvalid: " << m_tvalid << std::endl;
//        wait_clock_cycle();
//    }
//    std::cout << "Time " << pDUT->getTime() <<  " Ended AxiStreamReceiveData " << " m_tready "<< m_tready << " m_tvalid " << m_tvalid
//              << std::endl
//              << "Status: " << pDUT->getStatus() << " Error: " << pDUT->getError() << std::endl;
}

//void ReadDataThread::AxiStreamStopReceiveData()
//{
//    pDUT->write("m00_axis_tready", 0);
//    //wait_clock_cycle();
//}

ReadDataThread::ReadDataThread(Sync& sync, DUT* pdut) :
        AxiLite(sync, pdut, "ReadDataThread") {

}

