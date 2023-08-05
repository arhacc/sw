#include "targets/sim/WriteDataThread.hpp"

void WriteDataThread::run()  {
    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Started: " << getCurrentThreadName() << std::endl;

    Thread::sleepm(1);

    for(auto it=_dataIn.begin(); it != _dataIn.end(); it++) {
        AxiStreamSendData(*it);
    }

    AxiStreamSendStop();
    //return assembled_data;
    wait_clock_cycles(5);

    std::cout << std::dec << "[" << pDUT->getTime() << "]" << " Ended: " << getCurrentThreadName() << std::endl;
}

void WriteDataThread::AxiStreamSendData(const std::string_view& data)
{
    //std::cout << "Written data: " << data << " at : " << pDUT->getTime() << std::endl;
    pDUT->write("s00_axis_tdata", data);
    pDUT->write("s00_axis_tvalid", 1);
    pDUT->write("s00_axis_tlast", 1);
    pDUT->write("m00_axis_tready", 1);

    wait_clock_cycle();
}

void WriteDataThread::AxiStreamSendStop()
{
    pDUT->write("s00_axis_tdata", 0);
    pDUT->write("s00_axis_tvalid", 0);
    pDUT->write("s00_axis_tlast", 0);
    wait_clock_cycle();
}

WriteDataThread::WriteDataThread(Sync& sync, DUT* pdut, std::vector<std::string>& dataIn) :
        ClockConsumerThread(sync, pdut, "WriteDataThread"),
        _dataIn(dataIn){
}

