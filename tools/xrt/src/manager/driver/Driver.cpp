///
/// \file Driver.hpp
///
/// \brief Implementation of class Driver.
#include <common/CodeGen.hpp>
#include <common/Constants.hpp>
#include <common/arch/Arch.hpp>
#include <common/arch/generated/ArchConstants.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <targets/Targets.hpp>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>

#include <fmt/core.h>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

Driver::Driver(Manager& ctx, Targets& targets, Arch& arch) : targets_(targets), ctx_(ctx), arch_(arch) {
    std::string _hwArch = fmt::format(
        "xpu_{:08X}{:08X}{:08X}{:08X}",
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR));

    logInit.println<InfoLow>("Detected HW architecture {} will overwrite specified or default architecture", _hwArch);

    parseArchFile(arch, _hwArch);

    breakpoints_.resize(arch_.get(ArchConstant::DEBUG_NR_BREAKPOINTS));

    targets_.setInterruptCallback([this] {
        logWork.println<InfoHigh>("Got interrupt");
        targets_.setReportInterrupt(false);
        handleInterrupt();
        targets_.setReportInterrupt(true);
    });

    if constexpr (cClearMemoryAtStartup) {
        Matrix _matrix(arch_.get(ArchConstant::ARRAY_CELL_MEM_SIZE), arch_.get(ArchConstant::ARRAY_NR_CELLS));

        for (uint32_t i = 0; i < _matrix.numRows(); i++) {
            for (uint32_t j = 0; j < _matrix.numColumns(); j++) {
                _matrix.at(i, j) = cClearMemoryAtStartupValue;
            }
        }

        for (uint32_t i = 0; i < arch_.get(ArchConstant::ARRAY_CELL_MEM_SIZE); i += 128) {
            MatrixView view(_matrix, i, 0, 128, arch_.get(ArchConstant::ARRAY_NR_CELLS));

            writeMatrixArray(i, view);

            // confirm propogation
            readMatrixArray(i, view, false);
        }
    }
}

Driver::~Driver() = default;

void Driver::reset() {
    targets_.reset();

    writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 1);
    runClockCycles(100);
    writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 0);
    runClockCycles(100);
}

void Driver::runClockCycle() const {
    targets_.runClockCycle();
}

void Driver::runClockCycles(const unsigned n) const {
    targets_.runClockCycles(n);
}

void Driver::resetBreakpoints() {
    if (arch_.get(ArchConstant::RESOURCE_ACCELERATOR_HAS_DEBUG_VALUE) == 1) {
        for (unsigned i = 0; i < arch_.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
            clearBreakpoint(i);
        }
    }
}

void Driver::run(const std::uint32_t address, std::span<const uint32_t> args) {
    writeInstruction(arch_.INSTRB_prun, address);
    writeInstruction(arch_.INSTRB_nop);

    for (const auto arg : args) {
        writeInstruction(arg);
        writeInstruction(arch_.INSTRB_nop);
    }
}

auto Driver::readRegister(const std::uint32_t address) const -> std::uint32_t {
    return targets_.readRegister(address);
}

void Driver::writeRegister(const std::uint32_t address, const std::uint32_t value) const {
    targets_.writeRegister(address, value);
}

void Driver::readMatrix(
    const std::uint32_t accMemStart,
    MatrixView& view,
    const bool accRequireResultReady,
    const std::uint32_t reorderCommand,
    const bool controller) {
    logWork.print<InfoHigh>(
        "Reading matrix of size {}x{} at address {}:{}",
        view.numRows(),
        view.numColumns(),
        controller ? "controller" : "array",
        accMemStart);

    if (accRequireResultReady) {
        logWork.println<InfoHigh>(" (waiting for result)");
    } else {
        logWork.println<InfoHigh>(" (not waiting for result)");
    }

    targets_.readMatrixBefore(view);
    uint32_t numRows    = view.numRows();
    uint32_t numColumns = view.numColumns();

    if (reorderCommand == arch_.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE)) {
        std::swap(numRows, numColumns);
    }
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), reorderCommand);
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), view.numRows());
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), view.numColumns());

    writeTransferInstruction(
        controller ? (accRequireResultReady ? arch_.INSTR_get_ctrl_array_w_result_ready
                                            : arch_.INSTR_get_ctrl_array_wo_result_ready)
                   : (accRequireResultReady ? arch_.INSTR_get_matrix_array_w_result_ready
                                            : arch_.INSTR_get_matrix_array_wo_result_ready));
    writeTransferInstruction(accMemStart);
    writeTransferInstruction(numRows);
    writeTransferInstruction(numColumns);
    targets_.readMatrixAfter(view);
}

void Driver::readMatrixArray(
    const std::uint32_t accMemStart,
    MatrixView& view,
    const bool accRequireResultReady,
    const std::uint32_t reorderCommand) {
    readMatrix(accMemStart, view, accRequireResultReady, reorderCommand, false);
}

void Driver::readMatrixController(
    const std::uint32_t accMemStart,
    MatrixView& view,
    const bool accRequireResultReady,
    const std::uint32_t reorderCommand) {
    readMatrix(accMemStart, view, accRequireResultReady, reorderCommand, true);
}

void Driver::writeMatrix(
    const std::uint32_t accMemStart,
    const MatrixView& view,
    const std::uint32_t reorderCommand,
    const bool controller) {
    logWork.println<InfoHigh>(
        "Writing array matrix of size {}x{} at address {}:{}",
        view.numRows(),
        view.numColumns(),
        controller ? "controller" : "array",
        accMemStart);

    targets_.writeMatrixBefore(view);
    uint32_t numRows    = view.numRows();
    uint32_t numColumns = view.numColumns();

    if (reorderCommand == arch_.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE)) {
        std::swap(numRows, numColumns);
    }
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), reorderCommand);
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), view.numRows());
    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR), view.numColumns());

    writeTransferInstruction(controller ? (arch_.INSTR_send_ctrl_array) : (arch_.INSTR_send_matrix_array));
    writeTransferInstruction(accMemStart);
    writeTransferInstruction(numRows);
    writeTransferInstruction(numColumns);

    targets_.writeMatrixAfter(view);
}

void Driver::writeMatrixArray(
    const std::uint32_t accMemStart, const MatrixView& view, const std::uint32_t reorderCommand) {
    writeMatrix(accMemStart, view, reorderCommand, false);
}

void Driver::writeMatrixController(
    const std::uint32_t accMemStart, const MatrixView& view, const std::uint32_t reorderCommand) {
    writeMatrix(accMemStart, view, reorderCommand, true);
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(const std::uint32_t instruction) {
    writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR), instruction);
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(const std::uint8_t instructionByte, const std::uint32_t argument) {
    writeInstruction(makeInstruction(arch_, instructionByte, argument));
}

//-------------------------------------------------------------------------------------
void Driver::writeTransferInstruction(const std::uint32_t instruction) {
    writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_DTE_FIFO_IN_ADDR), instruction);
}

//-------------------------------------------------------------------------------------
void Driver::writeCode(const std::uint32_t address, std::span<const std::uint32_t> code) {
    writeInstruction(arch_.INSTRB_pload, address);
    writeInstruction(arch_.INSTR_nop);

    for (const std::uint32_t instruction : code) {
        writeInstruction(instruction);
    }

    writeInstruction(arch_.INSTRB_prun, 0);
    writeInstruction(arch_.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void Driver::registerBreakpoint(Breakpoint breakpoint, unsigned breakpointID) {
    if (arch_.get(ArchConstant::RESOURCE_ACCELERATOR_HAS_DEBUG_VALUE) == 1) {
        assert(breakpoint.conditions.size() == arch_.get(ArchConstant::DEBUG_BP_NR_CONDITIONS));

        if (breakpointID >= arch_.get(ArchConstant::DEBUG_NR_BREAKPOINTS)) {
            throw std::runtime_error("Breakpoint ID out of range");
        }

        unsigned _lastConditionID = arch_.get(ArchConstant::DEBUG_BP_NR_CONDITIONS) - 1;

        for (unsigned _conditionID = 0; _conditionID <= _lastConditionID; _conditionID++) {
            logWork.println<InfoHigh>(
                "Writing condition {} for hw breakpoint {}: operation {} operand {} value {}",
                _conditionID,
                breakpointID,
                breakpoint.conditions.at(_conditionID).condition,
                breakpoint.conditions.at(_conditionID).operand,
                breakpoint.conditions.at(_conditionID).value);

            writeRegister(
                arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_OPERATION_ADDR),
                breakpoint.conditions.at(_conditionID).condition);
            writeRegister(
                arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_SEL_ADDR),
                breakpoint.conditions.at(_conditionID).operand);
            writeRegister(
                arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_COMP_VAL_ADDR),
                breakpoint.conditions.at(_conditionID).value);
            writeRegister(
                arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_MASK_ADDR), 0xFFFF'FFFF);
            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WANT_IN_DEPTH_DEBUG_ADDR), 0);
            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), breakpointID);
            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_COND_ADDR), _conditionID);

            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 1);

            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
            writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);
        }

        breakpoints_.at(breakpointID) = std::make_unique<Breakpoint>(breakpoint);
    }
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoint(unsigned breakpointID) {
    if (arch_.get(ArchConstant::RESOURCE_ACCELERATOR_HAS_DEBUG_VALUE) == 1) {
        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), breakpointID);

        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 0);

        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);

        breakpoints_.at(breakpointID) = nullptr;
    }
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoints() {
    for (unsigned i = 0; i < arch_.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
        clearBreakpoint(i);
    }
}

//-------------------------------------------------------------------------------------
auto Driver::nextAvailableBreakpoint() const -> unsigned {
    for (unsigned _i = 0; _i < breakpoints_.size(); ++_i) {
        if (!breakpoints_[_i]) {
            return _i;
        }
    }

    throw std::runtime_error("All breakpoints");
}

//-------------------------------------------------------------------------------------
void Driver::handleInterrupt() {
    logWork.println<InfoHigh>("Driver::handleInterrupt");

    if ((readRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_READ_REGS_INTERRUPT_STATUS_REG_ADDR))
             >> arch_.get(ArchConstant::XPU_INTERRUPT_STATUS_REG_SOFTWARE_INT_LOC_LOWER)
        & 1) != 0U) {
        logWork.println<InfoHigh>("Software interrupt");
        // writeRegister(
        //     arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
        //     1 << arch.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_SOFTWARE_INT_LOC_LOWER));
    } else if (
        (readRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_READ_REGS_INTERRUPT_STATUS_REG_ADDR))
             >> arch_.get(ArchConstant::XPU_INTERRUPT_STATUS_REG_DEBUG_INT_LOC_LOWER)
        & 1) != 0U) {
        logWork.println<InfoHigh>("Breakpoint interrupt");
        handleBreakpointHit();
        writeRegister(
            arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
            1 << arch_.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_DEBUG_INT_LOC_LOWER));
    } else {
        logWork.println<InfoHigh>("Warning: Unknown interrupt");
        writeRegister(
            arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
            1 << arch_.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_GLOBAL_INT_LOC_LOWER));
    }
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHit() {
    assert(accImage_ == nullptr);

    accImage_ = std::make_shared<AcceleratorImage>();

    handleBreakpointHitFillAcceleratorImage(*accImage_);

    accImage_->print();

    const unsigned _breakpointID                   = handleBreakpointHitGetBreakpointID();
    const std::unique_ptr<Breakpoint>& _breakpoint = breakpoints_.at(_breakpointID);

    bool _continue = true;
    if (_breakpoint && _breakpoint->callback != nullptr) {
        _continue = _breakpoint->callback(accImage_, _breakpointID);
    }

    if (_continue) {
        continueAfterBreakpoint();
    }
}

//-------------------------------------------------------------------------------------
void Driver::continueAfterBreakpoint() {
    assert(accImage_ != nullptr);

    handleBreakpointHitDumpAcceleratorImage(*accImage_);

    accImage_ = nullptr;

    writeRegister(
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR),
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_DONE));

    logWork.println<InfoHigh>("Breakpoint done\n");
}

//-------------------------------------------------------------------------------------
auto Driver::handleBreakpointHitGetBreakpointID() const -> unsigned {
    uint32_t _bpStatusReg = readRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_BPs_STATUS_ADDR));

    unsigned _breakpointID = 0;
    for (unsigned _bitLocation = arch_.get(ArchConstant::DEBUG_CORE_STATUS_REG_BREAKPOINT_ENABLED_LOC_LOWER);
         _bitLocation <= arch_.get(ArchConstant::DEBUG_CORE_STATUS_REG_BREAKPOINT_ENABLED_LOC_UPPER);
         _bitLocation++, _breakpointID++) {
        if ((_bpStatusReg >> _bitLocation & 1) != 0U) {
            return _breakpointID;
        }
    }

    throw std::runtime_error("breakpoint hit but no breeakpoint is set in status reg");
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHitFillAcceleratorImage(AcceleratorImage& accImage) {
    const unsigned IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR =
        arch_.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    // These must be done in order
    accImage.pc             = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.prevPc1        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.prevPc2        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.prevPc3        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.nextPc         = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.cc             = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.nextInstrCtrl  = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.nextInstrArray = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.ctrlFlags      = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.ctrlAcc        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    accImage.ctrlStack.resize(arch_.get(ArchConstant::RESOURCE_CTRL_STACK_SIZE));
    for (uint32_t& _ctrlStackValue : accImage.ctrlStack) {
        _ctrlStackValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.ctrlActiveLoopCounter = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    accImage.ctrlLoopCounters.resize(arch_.get(ArchConstant::CTRL_NR_LOOP_COUNTERS));
    for (uint32_t& _ctrlLoopCounter : accImage.ctrlLoopCounters) {
        _ctrlLoopCounter = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.ctrlDecrReg          = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.reduceNetOut         = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.boolScanOr           = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    accImage.ctrlAddrRegsSelector = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    accImage.ctrlAddrRegs.resize(arch_.get(ArchConstant::CONTROLLER_ADDR_REG_NR_LOCATIONS));
    for (uint32_t& _ctrlAddrReg : accImage.ctrlAddrRegs) {
        _ctrlAddrReg = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.ctrlMem.resize(arch_.get(ArchConstant::CONTROLLER_MEM_SIZE));
    for (uint32_t& _ctrlMemValue : accImage.ctrlMem) {
        _ctrlMemValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.ctrlInstrMemCtrl.resize(arch_.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    for (uint32_t& _ctrlInstrMemCtrlValue : accImage.ctrlInstrMemCtrl) {
        _ctrlInstrMemCtrlValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.ctrlInstrMemArray.resize(arch_.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    for (uint32_t& _ctrlInstrMemArrayValue : accImage.ctrlInstrMemArray) {
        _ctrlInstrMemArrayValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayActivationReg.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayActivationRegValue : accImage.arrayActivationReg) {
        _arrayActivationRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayBool.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayBoolValue : accImage.arrayBool) {
        _arrayBoolValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayAcc.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayAccValue : accImage.arrayAcc) {
        _arrayAccValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayIORegData.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayIORegDataValue : accImage.arrayIORegData) {
        _arrayIORegDataValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayAddrReg.resize(2);
    for (std::vector<uint32_t>& _arrayAddrRegLayer : accImage.arrayAddrReg) {
        _arrayAddrRegLayer.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
        for (uint32_t& _arrayAddrRegValue : _arrayAddrRegLayer) {
            _arrayAddrRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
        }
    }

    accImage.arrayGlobalShiftReg.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayGlobalShiftRegValue : accImage.arrayGlobalShiftReg) {
        _arrayGlobalShiftRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    accImage.arrayStack.resize(arch_.get(ArchConstant::RESOURCE_ARRAY_CELL_STACK_SIZE));
    for (std::vector<uint32_t>& _arrayStackRow : accImage.arrayStack) {
        _arrayStackRow.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));

        for (uint32_t& _arrayStackValue : _arrayStackRow) {
            _arrayStackValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
        }
    }

    accImage.arrayMemValidRows = accImageArrayMemValidRows_;
    handleBreakpointHitFillAcceleratorImageArrayMem(accImage, accImageArrayMemValidRows_);

    accImage.rereadArrayMem = [this, &accImage]() {
        accImageArrayMemValidRows_ = accImage.arrayMemValidRows;
        writeRegister(
            arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_READ_ADDR_START_ADDR),
            accImageArrayMemValidRows_.first);

        writeRegister(
            arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_READ_ADDR_STOP_ADDR),
            accImageArrayMemValidRows_.second);
        writeRegister(
            arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR),
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_REREAD_ARRAY_MEM));

        handleBreakpointHitFillAcceleratorImageArrayMem(accImage, accImageArrayMemValidRows_);
    };
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHitFillAcceleratorImageArrayMem(
    AcceleratorImage& accImage, std::pair<uint32_t, uint32_t> _accImageArrayMemValidRows) {
    logWork.println<InfoHigh>(
        "Reading array memory from row {} to row {}",
        _accImageArrayMemValidRows.first,
        _accImageArrayMemValidRows.second);

    indicators::show_console_cursor(false);

    indicators::ProgressBar _bar{
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::Fill{"="},
        indicators::option::Lead{">"},
        indicators::option::Remainder{"-"},
        indicators::option::End{" ]"},
        indicators::option::PostfixText{"Reading array memory"},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    accImage.arrayMem.resize(arch_.get(ArchConstant::ARRAY_CELL_MEM_SIZE));
    for (std::vector<uint32_t>& _arrayMemRow : accImage.arrayMem) {
        _arrayMemRow.resize(arch_.get(ArchConstant::ARRAY_NR_CELLS));
    }

    logWork.println<InfoHigh>("Reading array memory from");

    uint32_t _totalRows = _accImageArrayMemValidRows.second - _accImageArrayMemValidRows.first + 1;
    for (uint32_t _row = _accImageArrayMemValidRows.first; _row <= _accImageArrayMemValidRows.second; _row++) {
        uint32_t _relativeRow = _row - _accImageArrayMemValidRows.first + 1;

        _bar.set_progress(_relativeRow * 100 / _totalRows);
        _bar.set_option(
            indicators::option::PostfixText{fmt::format("Reading array memory {}/{}", _relativeRow, _totalRows)});

        for (uint32_t& _arrayMemValue : accImage.arrayMem.at(_row)) {
            _arrayMemValue = readRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR));
            logWork.println<InfoHigh>("Value at row {}: {}", _row, _arrayMemValue);
        }

        // TODO: Remove
        if (_row == _accImageArrayMemValidRows.second - 1) {
            break;
        }
    }

    _bar.set_option(indicators::option::PostfixText{fmt::format("Reading array memory {}/{}", _totalRows, _totalRows)});
    _bar.set_progress(100);
    indicators::show_console_cursor(true);
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHitDumpAcceleratorImage(const AcceleratorImage& accImage) {
    const unsigned IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR =
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR);
    const unsigned IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR =
        arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR);
    const unsigned DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC =
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
    const unsigned DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC =
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);

    // WRITE ARRAY

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, accImage.prevPc2);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_PC));

    for (unsigned int _ctrlStackValue : std::ranges::reverse_view(accImage.ctrlStack)) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlStackValue);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_STACK_PUSH));
    }

    uint32_t _ctrlAddrRegIndex = 0;
    for (uint32_t _ctrlAddrReg : accImage.ctrlAddrRegs) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlAddrRegIndex++);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG_SELECTOR));
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlAddrReg);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, accImage.ctrlAddrRegsSelector);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG_SELECTOR));

    uint32_t _ctrlLoopCounterIndex = 0;
    for (uint32_t _ctrlLoopCounter : accImage.ctrlLoopCounters) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlLoopCounter);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlLoopCounterIndex++);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_LOOP_COUNTER));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, accImage.ctrlDecrReg);
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_DECREMENT_REG));

    uint32_t _ctrlMemAddress = 0;
    for (uint32_t _ctrlMemValue : accImage.ctrlMem) {
        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR), _ctrlMemAddress++);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlMemValue);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_MEMORY));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, accImage.ctrlAcc);
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);

    // WRITE ARRAY
    std::for_each(accImage.arrayStack.rbegin(), accImage.arrayStack.rend(), [=, this](auto& _arrayStackLayer) {
        std::for_each(_arrayStackLayer.begin(), _arrayStackLayer.end(), [=, this](auto& _arrayStackValue) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayStackValue);
        });

        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_STACK_PUSH));
    });

    // TODO: reverse
    for (auto& _arrayAddrRegLayer : accImage.arrayAddrReg) {
        for (uint32_t _arrayAddrRegValue : _arrayAddrRegLayer) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAddrRegValue);
        }

        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_ACC_X_ADDR_REG));

        // TODO: ask
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_ADDR_REG_STACK_DUPLICATE));
    }

    for (uint32_t _arrayAddrRegValue : accImage.arrayGlobalShiftReg) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAddrRegValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WROTE_ACC_X_GLOBAL_SHIFT_REG));

    uint32_t _addr = 0;
    for (const std::vector<uint32_t>& _arrayMemRow : accImage.arrayMem) {
        for (uint32_t _arrayMemValue : _arrayMemRow) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayMemValue);
        }
        writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_WRITE_ADDR_ADDR), _addr++);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_ACC_X_MEMORY));
    }

    for (uint32_t _arrayActivationRegValue : accImage.arrayActivationReg) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayActivationRegValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch_.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACTIVATION_REG));

    for (uint32_t _arrayAccValue : accImage.arrayAcc) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAccValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
}

std::uint64_t Driver::getSimSteps() const {
    return targets_.getSimSteps();
}

std::uint64_t Driver::getSimCycles() const {
    return targets_.getSimCycles();
}

void Driver::setMaxSimSteps(const std::uint64_t steps) {
    targets_.setMaxSimSteps(steps);
}

void Driver::setMaxSimCycles(const std::uint64_t max) {
    targets_.setMaxSimCycles(max);
}

std::shared_ptr<AcceleratorImage> Driver::getAcceleratorImageFromLog() {
    return targets_.getAcceleratorImageFromLog();
}
