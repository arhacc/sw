//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <targets/common/Future.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/SimStreams.hpp>
#include <targets/sim/SimTarget.hpp>
#include <targets/sim/Tb.hpp>

#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

#include <fmt/core.h>

// const std::filesystem::path SimTarget::cDesignDirPath = getXpuHome() / "lib" /
// "xsim.dir";
const std::filesystem::path SimTarget::cDesignDirPath = "xsim.dir";
//-------------------------------------------------------------------------------------
SimTarget::SimTarget(const Arch& _arch, bool enableWdb, std::string_view _logSuffix) : arch(_arch) {
    logInit.print("Starting SimTarget...\n");

	// TODO: make this work
	auto _newWorkingDirectory = getXpuHome() / "lib" /"designs" / arch.IDString;
    logInit.print(fmt::format("Changing working directory path to {}\n", _newWorkingDirectory.string()));
//     std::filesystem::current_path(_newWorkingDirectory);
	
	// Evil stuff
	std::filesystem::current_path(getXpuHome() / "lib");
	if (std::filesystem::exists("xsim.dir")) {
		std::filesystem::remove("xsim.dir");
	}
	std::filesystem::create_directory_symlink(getXpuHome() / "lib" / "designs" / arch.IDString / "xsim.dir", "xsim.dir");

    tb = new Tb(
        cDesignDirPath / "simulator_axi" / "xsimk.so",
        "librdi_simulator_kernel.so",
        "clock",
        "resetn",
        _arch,
        enableWdb,
        _logSuffix);

    simStreams = new SimStreams(tb, (1 << (arch.get(ArchConstant::IO_INTF_PROG_AXILITE_DATA_SIZE) / 8)) - 1);
}

//-------------------------------------------------------------------------------------
SimTarget::~SimTarget() {
    delete tb;

    delete simStreams;
}

//-------------------------------------------------------------------------------------
void SimTarget::reset() {
    tb->reset();
}

//-------------------------------------------------------------------------------------
void SimTarget::process(std::shared_ptr<Future> _future) {
    simStreams->process(_future);
}

//-------------------------------------------------------------------------------------
void SimTarget::runClockCycles(unsigned _n) {
    for (unsigned _i = 0; _i < _n; _i++) {
        runClockCycle();
    }
}

//-------------------------------------------------------------------------------------
void SimTarget::runClockCycle() {
    simStreams->step();

    tb->runClockCycle();

    bool _thisClockInterrupt = static_cast<bool>(tb->read("interrupt"));

    // only on posedge
    if (reportInterrupt && _thisClockInterrupt == true && lastClockInterrupt == false) {
        lastClockInterrupt = _thisClockInterrupt;
        throw SimInterrupt();
    }

    lastClockInterrupt = _thisClockInterrupt;
}

//-------------------------------------------------------------------------------------
void SimTarget::setReportInterrupt(bool _reportInterrupt) {
    reportInterrupt = _reportInterrupt;
}

//-------------------------------------------------------------------------------------
uint64_t SimTarget::getTime() const {
    return tb->getTime();
}

//-------------------------------------------------------------------------------------
