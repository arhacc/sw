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
#include <targets/sim/SimStreams.hpp>
#include <targets/sim/SimTarget.hpp>
#include <targets/sim/Tb.hpp>
#include <targets/sim/statelogparser/ParsedContext.hpp>
#include <targets/sim/statelogparser/Parser.gen.hpp>
#define YYSTYPE XPU_STATE_LOG_STYPE
#include <targets/sim/XSimFS.hpp>
#include <targets/sim/statelogparser/Lexer.gen.hpp>

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

#include <fmt/core.h>

SimTarget::SimTarget(
    const Arch& arch,
    const bool enableWdb,
    const bool haveAcceleratorImageFromLog,
    const std::string_view logSuffix,
    const std::uint32_t clockPeriodNs)
    : arch_(arch),
      designLibPath_(XSimFS::getDesignLibPath(arch_)),
      haveAcceleratorImageFromLog_(haveAcceleratorImageFromLog),
      processAcceleratorImageFromLogThread_(
          haveAcceleratorImageFromLog_ ? std::make_unique<std::thread>([this] {
              processAcceleratorImageFromLog();
          })
                                       : nullptr),
      tb_(designLibPath_,
          "librdi_simulator_kernel.so",
          "clock",
          "resetn",
          arch_,
          enableWdb,
          haveAcceleratorImageFromLog_,
          logSuffix,
          clockPeriodNs),
      simStreams_(
          [this] {
              runClockCycle();
          },
          arch_,
          tb_,
          (1 << (arch_.get(ArchConstant::IO_INTF_PROG_AXILITE_DATA_SIZE) / 8)) - 1) {}

SimTarget::~SimTarget() {
    if (haveAcceleratorImageFromLog_) {
        processAcceleratorImageFromLogThread_->join();
    }
}

void SimTarget::reset() {
    tb_.reset();
}

std::uint32_t SimTarget::computeWstrb(const Arch& arch) {
    return 1 << arch.get(ArchConstant::IO_INTF_PROG_AXILITE_DATA_SIZE) / 8 - 1;
}

void SimTarget::runClockCycles(const unsigned cycles) {
    for (unsigned _i = 0; _i < cycles; _i++) {
        runClockCycle();
    }
}

void SimTarget::runClockCycle() {
    tb_.runClockCycle();

    const bool thisClockInterrupt = static_cast<bool>(tb_.read("interrupt"));

    // only on posedge
    if (reportInterrupt_ && thisClockInterrupt == true && lastClockInterrupt_ == false) {
        lastClockInterrupt_ = thisClockInterrupt;
        interruptCallback_();
    }

    lastClockInterrupt_ = thisClockInterrupt;
}

void SimTarget::setReportInterrupt(const bool reportInterrupt) {
    reportInterrupt_ = reportInterrupt;
}

void SimTarget::setInterruptCallback(const std::function<void()>& callback) {
    interruptCallback_ = callback;
}

std::uint64_t SimTarget::getSimSteps() const {
    return tb_.getSimSteps();
}

std::uint64_t SimTarget::getSimCycles() const {
    return tb_.getSimCycles();
}

void SimTarget::setMaxSimSteps(const std::uint64_t cycles) {
    tb_.setMaxSimSteps(cycles);
}

void SimTarget::setMaxSimCycles(const std::uint64_t cycles) {
    tb_.setMaxSimCycles(cycles);
}

std::uint32_t SimTarget::readRegister(const std::uint32_t address) {
    return simStreams_.readRegister(address);
}

void SimTarget::writeRegister(const std::uint32_t address, const std::uint32_t data) {
    simStreams_.writeRegister(address, data);
}

void SimTarget::readMatrixBefore([[maybe_unused]] MatrixView& view) {}

void SimTarget::readMatrixAfter(MatrixView& view) {
    simStreams_.readMatrix(view);
}

void SimTarget::writeMatrixBefore([[maybe_unused]] const MatrixView& view) {}

void SimTarget::writeMatrixAfter(const MatrixView& view) {
    simStreams_.writeMatrix(view);
}

void SimTarget::processAcceleratorImageFromLog() {
    // TODO: make a resource directory out of this
    const std::filesystem::path logFilePath =
        getXpuHome() / "logs" / "simulation_files" / "print_log_debug_file_function.txt";

    if (!is_fifo(logFilePath)) {
        logWork.println<FatalError>("FATAL: Could not open state log, not a directory or not a FIFO");
        logWork.println<FatalError>("RUN THE FOLLOWING IN YOUR TERMINAL\n");
        logWork.println<FatalError>(
            "mkdir -p {} && rm -f {} && mkfifo {}\n",
            logFilePath.parent_path().string(),
            logFilePath.string(),
            logFilePath.string());
        std::exit(EXIT_FAILURE);
    }

    logFile_ = std::fopen(logFilePath.c_str(), "r");

    void* scanner;
    xpu_state_log_lex_init(&scanner);
    xpu_state_log_set_in(logFile_, scanner);

    ParsedContext context;
    context.callback = [this](ParsedContext& context) {
        auto putInt = [&](const std::string& logFileName, std::uint32_t& destination) {
            const auto& v = context.result[logFileName];
            assert(v.type == ParsedContext::Value::Type::INT);
            destination = v.v.i;
        };

        auto putIntVector = [&](const std::string& logFileName, std::vector<std::uint32_t>& destination) {
            const auto& v = context.result[logFileName];
            assert(v.type == ParsedContext::Value::Type::INT_VECTOR);
            destination = v.v.iv;
        };

        putInt("pc", acceleratorImageFromLog_->pc);
        putInt("next_pc", acceleratorImageFromLog_->nextPc);
        putInt("clk_counter", acceleratorImageFromLog_->cc);

        putInt("next_instr_ctrl", acceleratorImageFromLog_->nextInstrCtrl);
        putInt("next_instr_array", acceleratorImageFromLog_->nextInstrArray);
        putInt("acc", acceleratorImageFromLog_->ctrlAcc);
        putIntVector("ctrl_stack", acceleratorImageFromLog_->ctrlStack);
        putInt("jmp_val_reg_actual", acceleratorImageFromLog_->ctrlActiveLoopCounter);
        putIntVector("jmp_val_regs", acceleratorImageFromLog_->ctrlLoopCounters);
        putInt("decrement_reg", acceleratorImageFromLog_->ctrlDecrReg);
        putInt("reduce_out", acceleratorImageFromLog_->reduceNetOut);
        putInt("bool", acceleratorImageFromLog_->boolScanOr);
        putInt("ctrl_addr_regs_selector", acceleratorImageFromLog_->ctrlAddrRegsSelector);
        putIntVector("ctrl_addr_regs", acceleratorImageFromLog_->ctrlAddrRegs);

        putIntVector("ctrl_mem", acceleratorImageFromLog_->ctrlMem);

        putIntVector("array_activation_reg", acceleratorImageFromLog_->arrayActivationReg);
        putIntVector("array_bool", acceleratorImageFromLog_->arrayBool);
        putIntVector("array_acc", acceleratorImageFromLog_->arrayAcc);
        putIntVector("array_io_reg_data", acceleratorImageFromLog_->arrayIORegData);
        putIntVector("array_global_shift_reg", acceleratorImageFromLog_->arrayGlobalShiftReg);

        acceleratorImageFromLog_->arrayAddrReg.resize(2);
        putIntVector("array_addr_reg_layer0", acceleratorImageFromLog_->arrayAddrReg.at(0));
        putIntVector("array_addr_reg_layer1", acceleratorImageFromLog_->arrayAddrReg.at(1));

        acceleratorImageFromLog_->arrayStack.resize(4);
        putIntVector("array_acc", acceleratorImageFromLog_->arrayStack.at(0));
        putIntVector("array_stack_layer1", acceleratorImageFromLog_->arrayStack.at(1));
        putIntVector("array_stack[0]", acceleratorImageFromLog_->arrayStack.at(2));
        putIntVector("array_stack[1]", acceleratorImageFromLog_->arrayStack.at(3));

        const auto& memSize = arch_.get(ArchConstant::ARRAY_CELL_MEM_SIZE);
        acceleratorImageFromLog_->arrayMem.resize(memSize);
        for (std::size_t i = 0; i < memSize; i++) {
            putIntVector(fmt::format("array_mem[{}]", i), acceleratorImageFromLog_->arrayMem.at(i));
        }
        acceleratorImageFromLog_->arrayMemValidRows.first  = 0;
        acceleratorImageFromLog_->arrayMemValidRows.second = memSize - 1;
    };
    xpu_state_log_set_extra(&context, scanner);

    xpu_state_log_parse(scanner);
}

std::shared_ptr<AcceleratorImage> SimTarget::getAcceleratorImageFromLog() {
    runClockCycle();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto accImage = std::make_shared<AcceleratorImage>();
    *accImage     = *acceleratorImageFromLog_;

    return accImage;
}
