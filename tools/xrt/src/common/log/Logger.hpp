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
#include <iostream>

#include <fmt/os.h>


#define XPU_DEBUG_VERBOSITY_LEVEL_NONE 1
#define XPU_DEBUG_VERBOSITY_LEVEL_LOW 100
#define XPU_DEBUG_VERBOSITY_LEVEL_MEDIUM 200
#define XPU_DEBUG_VERBOSITY_LEVEL_HIGH 300
#define XPU_DEBUG_VERBOSITY_LEVEL_FULL 400
#define XPU_DEBUG_VERBOSITY_LEVEL_DEBUG 500

enum LogLevel {
  FatalError = 0,
  Error = 0,
  Warn = 1,
  InfoLow = 100,
  InfoMedium = 200,
  InfoHigh = 300,
  InfoFull = 400,
  Debug = 500,
};

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

    template <LogLevel L, typename... T>
    inline auto print(fmt::format_string<T...> fmt, T&&... args) -> void {
        if constexpr (XPU_DEBUG_VERBOSITY_LEVEL >= L) {
            std::string _message = fmt::format(fmt, std::forward<T>(args)...);

            if (console) {
                std::cout.write(_message.data(), _message.length());
		std::cout.flush();
            }

            if (out_) {
                out_->print("{}", _message);
                out_->flush();
            }
        }
    }

    template <LogLevel L, typename... T>
    inline auto println(fmt::format_string<T...> fmt, T&&... args) -> void {
        if constexpr (XPU_DEBUG_VERBOSITY_LEVEL >= L) {
            std::string _message = fmt::format(fmt, std::forward<T>(args)...);

            if (console) {
                std::cout.write(_message.data(), _message.length());
                std::cout.write("\n", 1);
		std::cout.flush();
            }

            if (out_) {
                out_->print("{}\n", _message);
                out_->flush();
            }
          }
    }

    inline void print(std::string_view _message) {
        if (console) {
            std::cout << fmt::format("{}", _message);
	    std::cout.flush();
        }

        if (out_) {
            out_->print("{}", _message);
            out_->flush();
        }
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
