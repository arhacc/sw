#pragma once
#include "AxiLite.hpp"
#include "nlohmann/json.hpp"

class WriteProgramThread : public AxiLite {

    std::fstream axi_lite_program_file_combined; /*only let for not getting a compilation error*/
    std::fstream axi_lite_function_arguments_file;

    virtual void run() override;

public:
    WriteProgramThread(Sync& sync, DUT* pdut);
};
