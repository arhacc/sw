//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>

Logger::Logger(std::string_view _name, const std::filesystem::path& _path, bool _console)
    : name(_name), console(_console) {
    createDirIfNotExists(_path.parent_path());

    out_ = new fmt::ostream(fmt::output_file(_path.c_str()));
}

Logger::~Logger() {
    fmt::println("Flushing out for {}", name);
    out_->flush();
    delete out_;
}

#ifdef XRT_FULL_IO_LOG
Logger logAXIStreamRead("axi_stream_read", getXpuHome() / "logs" / "axi_stream_read.log");
Logger
    logAXIStreamWrite("axi_stream_write", getXpuHome() / "logs" / "axi_stream_write.log");
Logger logAXILite("axi_lite", getXpuHome() / "logs" / "axi_lite.log");
#endif

Logger logInit("xrt_init", getXpuHome() / "logs" / "xrt_init.log", true);
Logger logWork("xrt_work", getXpuHome() / "logs" / "xrt_work.log", true);
