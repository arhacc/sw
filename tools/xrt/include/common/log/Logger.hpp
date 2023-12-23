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

  public:
    Logger(std::string_view _name, const std::filesystem::path& _outPath);
    ~Logger();

    template<typename... T>
    inline void print(fmt::format_string<T...> fmt, T&&... args) {
        out_->print(args...);
    }

    inline fmt::ostream& out() {
        return *out_;
    }
};

#ifdef XRT_FULL_IO_LOG
extern Logger logAXIStreamRead;
extern Logger logAXIStreamWrite;
extern Logger logAXILite;
#endif
