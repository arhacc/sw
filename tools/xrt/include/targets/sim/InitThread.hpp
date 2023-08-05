#pragma once
#include "targets/sim/ClockConsumerThread.hpp"
#include "nlohmann/json.hpp"

class InitThread : public ClockConsumerThread {

private:
    std::fstream axi_stream_in_file;
    std::fstream axi_lite_program_file;
    std::fstream axi_lite_program_file_combined;
    std::fstream axi_lite_function_arguments_file;

    virtual void run() override;

public:
    InitThread(Sync& sync, DUT* pdut, std::vector<std::string>& dataIn);

    std::vector<std::string>& _dataIn;
};
