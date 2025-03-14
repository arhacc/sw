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
#include <targets/sim/statelogparser/ParsedContext.hpp>
#include <targets/sim/statelogparser/Parser.gen.hpp>
#define YYSTYPE         XPU_STATE_LOG_STYPE
#include <targets/sim/statelogparser/Lexer.gen.hpp>
#include <targets/sim/XSimFS.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <filesystem>
#include <memory>
#include <vector>

#include <fmt/core.h>

const std::filesystem::path SimTarget::cDesignDirPath = "xsim.dir";
//-------------------------------------------------------------------------------------
SimTarget::SimTarget(const Arch& _arch, bool enableWdb, bool _haveAcceleratorImageFromLog, std::string_view _logSuffix, uint32_t _clockPeriodNs)
  : arch(_arch), haveAcceleratorImageFromLog(_haveAcceleratorImageFromLog), acceleratorImageFromLog(std::make_unique<AcceleratorImage>()) {
      logInit.println<InfoLow>("Starting SimTarget...");

      XSimFS::setup(arch);
	
	if (haveAcceleratorImageFromLog) {
      processAcceleratorImageFromLogThread = std::make_unique<std::thread>([this]() {
          processAcceleratorImageFromLog();
      });
  }

  tb = new Tb(
      std::filesystem::current_path() / cDesignDirPath / "simulator_axi" / "xsimk.so",
      "librdi_simulator_kernel.so",
      "clock",
      "resetn",
      _arch,
      enableWdb,
      haveAcceleratorImageFromLog,
      _logSuffix,
      _clockPeriodNs);

  simStreams = new SimStreams(*this, _arch, tb, (1 << (arch.get(ArchConstant::IO_INTF_PROG_AXILITE_DATA_SIZE) / 8)) - 1);
}

//-------------------------------------------------------------------------------------
SimTarget::~SimTarget() {

    delete tb;
    if (haveAcceleratorImageFromLog) {
      processAcceleratorImageFromLogThread->join();
    }
    delete simStreams;
}

//-------------------------------------------------------------------------------------
void SimTarget::reset() {
    tb->reset();
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> SimTarget::readRegisterAsync(const std::uint32_t address, std::uint32_t* dataLocation) {
    return simStreams->createReadRegisterFuture(address, dataLocation);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> SimTarget::writeRegisterAsync(const std::uint32_t address, std::uint32_t data) {
    return simStreams->createWriteRegisterFuture(address, data);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> SimTarget::readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) {
    return simStreams->createReadMatrixViewFuture(view);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> SimTarget::writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) {
    return simStreams->createWriteMatrixViewFuture(view);
}

//-------------------------------------------------------------------------------------
void SimTarget::runClockCycles(const unsigned _n) {
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
        interruptCallback();
    }

    lastClockInterrupt = _thisClockInterrupt;
}

//-------------------------------------------------------------------------------------
void SimTarget::setReportInterrupt(bool _reportInterrupt) {
    reportInterrupt = _reportInterrupt;
}

void SimTarget::setInterruptCallback(const std::function<void()>& callback) {
    interruptCallback = callback;
}

//-------------------------------------------------------------------------------------
uint64_t SimTarget::getSimSteps() const {
    return tb->getSimSteps();
}

//-------------------------------------------------------------------------------------
uint64_t SimTarget::getSimCycles() const {
    return tb->getSimCycles();
}

//-------------------------------------------------------------------------------------
void SimTarget::setMaxSimSteps(const uint64_t _max) {
    tb->setMaxSimSteps(_max);
}

//-------------------------------------------------------------------------------------
void SimTarget::setMaxSimCycles(uint64_t _max) {
    tb->setMaxSimCycles(_max);
}

//-------------------------------------------------------------------------------------
void SimTarget::processAcceleratorImageFromLog() {
  // TODO: make a resource directory out of this
  std::filesystem::path _logFilePath = getXpuHome() / "logs" / "simulation_files" / "print_log_debug_file_function.txt";

  if (!std::filesystem::is_fifo(_logFilePath)) {
    logWork.println<FatalError>("FATAL: Could not open state log, not a directory or not a FIFO");
    logWork.println<FatalError>("RUN THE FOLLOWING IN YOUR TERMINAL\n");
    logWork.println<FatalError>("mkdir -p {} && rm -f {} && mkfifo {}\n", _logFilePath.parent_path().string(), _logFilePath.string(), _logFilePath.string());
    std::exit(EXIT_FAILURE);
  }

  logFile = std::fopen(_logFilePath.c_str(), "r");

  void* _scanner;
  xpu_state_log_lex_init(&_scanner);
  xpu_state_log_set_in(logFile, _scanner);

  ParsedContext _context;
  _context.callback = [this] (ParsedContext &_context) {
    auto _putInt = [&](std::string _logFileName, uint32_t& _destination) {
      auto& v = _context.result[_logFileName];
      assert(v.type == ParsedContext::Value::Type::INT);
      _destination = v.v.i;
    };

    auto _putIntVector = [&](std::string _logFileName, std::vector<uint32_t>& _destination) {
      auto& v = _context.result[_logFileName];
      assert(v.type == ParsedContext::Value::Type::INT_VECTOR);
      _destination = v.v.iv;
    };

    _putInt("pc", acceleratorImageFromLog->pc);
    _putInt("next_pc", acceleratorImageFromLog->nextPc);
    _putInt("clk_counter", acceleratorImageFromLog->cc);
    
    _putInt("next_instr_ctrl", acceleratorImageFromLog->nextInstrCtrl);
    _putInt("next_instr_array", acceleratorImageFromLog->nextInstrArray);
    _putInt("acc", acceleratorImageFromLog->ctrlAcc);
    _putIntVector("ctrl_stack", acceleratorImageFromLog->ctrlStack);
    _putInt("jmp_val_reg_actual", acceleratorImageFromLog->ctrlActiveLoopCounter);
    _putIntVector("jmp_val_regs", acceleratorImageFromLog->ctrlLoopCounters);
    _putInt("decrement_reg", acceleratorImageFromLog->ctrlDecrReg);
    _putInt("reduce_out", acceleratorImageFromLog->reduceNetOut);
    _putInt("bool", acceleratorImageFromLog->boolScanOr);
    _putInt("ctrl_addr_regs_selector", acceleratorImageFromLog->ctrlAddrRegsSelector);
    _putIntVector("ctrl_addr_regs", acceleratorImageFromLog->ctrlAddrRegs);

    _putIntVector("ctrl_mem", acceleratorImageFromLog->ctrlMem);

    _putIntVector("array_activation_reg", acceleratorImageFromLog->arrayActivationReg);
    _putIntVector("array_bool", acceleratorImageFromLog->arrayBool);
    _putIntVector("array_acc", acceleratorImageFromLog->arrayAcc);
    _putIntVector("array_io_reg_data", acceleratorImageFromLog->arrayIORegData);
    _putIntVector("array_global_shift_reg", acceleratorImageFromLog->arrayGlobalShiftReg);

    acceleratorImageFromLog->arrayAddrReg.resize(2);
    _putIntVector("array_addr_reg_layer0", acceleratorImageFromLog->arrayAddrReg.at(0));
    _putIntVector("array_addr_reg_layer1", acceleratorImageFromLog->arrayAddrReg.at(1));

    acceleratorImageFromLog->arrayStack.resize(4);
    _putIntVector("array_acc", acceleratorImageFromLog->arrayStack.at(0));
    _putIntVector("array_stack_layer1", acceleratorImageFromLog->arrayStack.at(1));
    _putIntVector("array_stack[0]", acceleratorImageFromLog->arrayStack.at(2));
    _putIntVector("array_stack[1]", acceleratorImageFromLog->arrayStack.at(3));

    const auto& _memSize = arch.get(ArchConstant::ARRAY_CELL_MEM_SIZE);
    acceleratorImageFromLog->arrayMem.resize(_memSize);
    for (size_t _i = 0; _i < _memSize; _i++) {
      _putIntVector(fmt::format("array_mem[{}]", _i), acceleratorImageFromLog->arrayMem.at(_i));
    }
    acceleratorImageFromLog->arrayMemValidRows.first = 0;
    acceleratorImageFromLog->arrayMemValidRows.second = _memSize - 1;
  };
  xpu_state_log_set_extra(&_context, _scanner);

  xpu_state_log_parse(_scanner);

}

//-------------------------------------------------------------------------------------
std::shared_ptr<AcceleratorImage> SimTarget::getAcceleratorImageFromLog() {
  runClockCycle();
  std::this_thread::sleep_for(std::chrono::seconds(1));

  auto _accImage = std::make_shared<AcceleratorImage>();
  *_accImage = *acceleratorImageFromLog;

  return _accImage;
}

//-------------------------------------------------------------------------------------
