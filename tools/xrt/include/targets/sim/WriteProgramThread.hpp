#pragma once
#include <cstdint>
#include <span>

#include "AxiLite.hpp"
#include "nlohmann/json.hpp"

class WriteProgramThread : public AxiLite {
    std::fstream
        axi_lite_program_file_combined; /*only let for not getting a compilation error*/
    std::fstream axi_lite_function_arguments_file;

    virtual void run() override;

    std::span<uint32_t> programFile;

  public:
    WriteProgramThread(Sync& sync, DUT* pdut, std::span<uint32_t> programFile);
};
