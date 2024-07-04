//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>

#include "fmt/os.h"

Logger::Logger(std::string_view _name, std::filesystem::path _path, bool _console)
    : name(_name), outPath(std::move(_path)), out_(nullptr), console(_console) {
    createDirIfNotExists(outPath.parent_path());
}

Logger::~Logger() {
    if (out_ != nullptr) {
        fmt::println("Flushing out for {}", name);
        out_->flush();
        delete out_;
    }
}

void Logger::init(std::string_view _logSuffix) {
    if (_logSuffix != "") {
        outPath += ".";
        outPath += _logSuffix;
    }
    outPath += ".log";

    out_ = new fmt::ostream(fmt::output_file(outPath.c_str()));
}

#ifdef XRT_FULL_IO_LOG
Logger logAXIStreamRead("axi_stream_read", getXpuHome() / "logs" / "axi_stream_read");
Logger logAXIStreamWrite("axi_stream_write", getXpuHome() / "logs" / "axi_stream_write");
Logger logAXILite("axi_lite", getXpuHome() / "logs" / "axi_lite");
#endif

Logger logInit("xrt_init", getXpuHome() / "logs" / "xrt_init", true);
Logger logWork("xrt_work", getXpuHome() / "logs" / "xrt_work", true);
Logger logNet("xrt_net", getXpuHome() / "logs" / "xrt_net");

Logger logCodeMem("xrt_code_mem", getXpuHome() / "logs" / "xrt_code_mem");

void initLogs(std::string_view _logSuffix) {
#ifdef XRT_FULL_IO_LOG
    logAXIStreamRead.init(_logSuffix);
    logAXIStreamWrite.init(_logSuffix);
    logAXILite.init(_logSuffix);
#endif

    logInit.init(_logSuffix);
    logWork.init(_logSuffix);
    logNet.init(_logSuffix);

#ifndef XRT_NO_LOG_CODEMEM
    logCodeMem.init(_logSuffix);
#endif
}
