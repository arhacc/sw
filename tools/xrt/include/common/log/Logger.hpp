//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <filesystem>

#include <fmt/os.h>

class Logger {
    std::string name;
    fmt::ostream* out_;
    bool console;

  public:
    Logger(
        std::string_view _name,
        const std::filesystem::path& _outPath,
        bool console = false);
    ~Logger();

    inline fmt::ostream& out() {
        return *out_;
    }

    inline void print(std::string_view _message) {
        if (console) {
            fmt::print("{}", _message);
        }
        out_->print("{}", _message);
    }
};

#ifdef XRT_FULL_IO_LOG
extern Logger logAXIStreamRead;
extern Logger logAXIStreamWrite;
extern Logger logAXILite;
#endif

extern Logger logInit;
extern Logger logWork;
