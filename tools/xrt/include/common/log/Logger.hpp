//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <stdexcept>

#include <fmt/os.h>

class Logger {
    std::string name;
    std::filesystem::path outPath;
    fmt::ostream* out_;
    bool console;

  public:
    Logger(std::string_view _name, std::filesystem::path _outPath, bool console = false);
    ~Logger();

    void init(std::string_view _logSuffix);

    inline fmt::ostream& out() {
        return *out_;
    }

    inline void print(std::string_view _message) {
        if (out_ == nullptr) {
            throw std::runtime_error("printing to uninitialized logger " + name);
        }

        if (console) {
            fmt::print("{}", _message);
        }
        out_->print("{}", _message);
#ifndef NDEBUG
        out_->flush();
#endif
    }
};

#ifdef XRT_FULL_IO_LOG
extern Logger logAXIStreamRead;
extern Logger logAXIStreamWrite;
extern Logger logAXILite;
#endif

extern Logger logNet;

extern Logger logInit;
extern Logger logWork;

#ifndef XRT_NO_LOG_CODEMEM
extern Logger logCodeMem;
#endif

void initLogs(std::string_view _logSuffix);
