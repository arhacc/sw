//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <targets/Targets.hpp>
#include <targets/common/Future.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

#include "common/Constants.hpp"
#include "common/arch/generated/ArchConstants.hpp"
#include "fmt/core.h"
#include "targets/sim/SimTarget.hpp"
#include <fmt/printf.h>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

//-------------------------------------------------------------------------------------
Driver::Driver(Manager* _ctx, Targets* _targets, Arch& _arch) : targets(_targets), ctx(_ctx), arch(_arch) {
    std::string _hwArch = fmt::format(
        "xpu_{:08X}{:08X}{:08X}{:08X}",
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR));

    logInit.print(
        fmt::format("Detected HW architecture {} will overwrite specified or default architecture\n", _hwArch));

    parseArchFile(_arch, _hwArch);

    breakpoints.resize(arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS));

    if constexpr (cClearMemoryAtStartup) {
        Matrix _matrix(arch.get(ArchConstant::ARRAY_CELL_MEM_SIZE), arch.get(ArchConstant::ARRAY_NR_CELLS));

        for (uint32_t i = 0; i < _matrix.numRows(); i++) {
            for (uint32_t j = 0; j < _matrix.numColumns(); j++) {
                _matrix.at(i, j) = cClearMemoryAtStartupValue;
            }
        }

        for (uint32_t i = 0; i < arch.get(ArchConstant::ARRAY_CELL_MEM_SIZE); i += 128) {
            auto _matrixView = std::make_shared<MatrixView>(_matrix, i, 0, 128, arch.get(ArchConstant::ARRAY_NR_CELLS));

            writeMatrixArray(i, _matrixView);

            // confirm propogation
            readMatrixArray(i, _matrixView, false);
        }
    }
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    std::shared_ptr<Future> _future = writeMatrixArrayAsync(_accMemStart, _matrixView, _reorderCommand);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixArray(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    std::shared_ptr<Future> _future = readMatrixArrayAsync(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::writeMatrixController(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    std::shared_ptr<Future> _future = writeMatrixControllerAsync(_accMemStart, _matrixView, _reorderCommand);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::readMatrixController(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    std::shared_ptr<Future> _future = readMatrixControllerAsync(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::reset() {
    targets->reset();

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 1);
    runClockCycles(100);
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR), 0);
    runClockCycles(100);
}

//-------------------------------------------------------------------------------------
void Driver::runClockCycle() {
    try {
        targets->runClockCycle();
    } catch (SimInterrupt&) {
        logWork.print("Got interrupt\n");
        targets->setReportInterrupt(false);
        handleInterrupt();
        targets->setReportInterrupt(true);
    }
}

//-------------------------------------------------------------------------------------
void Driver::runClockCycles(unsigned _n) {
    targets->runClockCycles(_n);
}

//-------------------------------------------------------------------------------------
void Driver::resetBreakpoints() {
    for (unsigned i = 0; i < arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
        clearBreakpoint(i);
    }
}

//-------------------------------------------------------------------------------------
void Driver::run(uint32_t _address, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _future = runAsync(_address, _args);
    _future->wait();
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::runAsync(uint32_t _address, std::span<const uint32_t> _args) {
    std::vector<std::shared_ptr<Future>> _futures;

    _futures.push_back(writeInstructionAsync(arch.INSTRB_prun, _address));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    for (uint32_t _arg : _args) {
        _futures.push_back(writeInstructionAsync(_arg));
        _futures.push_back(writeInstructionAsync(arch.INSTR_nop));
    }

    return make_shared<AndFuture>(ctx, std::move(_futures));
}

//-------------------------------------------------------------------------------------
uint32_t Driver::readRegister(uint32_t _address) {
    uint32_t _data;

    std::shared_ptr<Future> _future = readRegisterAsync(_address, &_data);
    _future->wait();

    return _data;
}

//-------------------------------------------------------------------------------------
void Driver::writeRegister(uint32_t _address, uint32_t _register) {
    std::shared_ptr<Future> _future = writeRegisterAsync(_address, _register);
    _future->wait();
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::readRegisterAsync(uint32_t _address, uint32_t* _dataLocation) {
    std::shared_ptr<Future> _future = std::make_shared<RegisterReadFuture>(ctx, _address, _dataLocation);

    targets->process(_future);

    return _future;
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::writeRegisterAsync(uint32_t _address, uint32_t _data) {
    std::shared_ptr<Future> _future = std::make_shared<RegisterWriteFuture>(ctx, _address, _data);

    targets->process(_future);

    return _future;
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::readMatrixArrayAsync(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    logWork.print(fmt::format(
        "Reading array matrix of size {}x{} at address {}",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

    if (_accRequireResultReady) {
        logWork.print(" (waiting for result)\n");
    } else {
        logWork.print(" (not waiting for result)\n");
    }
    
	uint32_t num_rows = _matrixView->numRows();
	uint32_t num_cols = _matrixView->numColumns();
	
	if(_reorderCommand == arch.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE))
	{
		num_rows = _matrixView->numColumns();
		num_cols = _matrixView->numRows();
	}

	std::shared_ptr<Future> _f0 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_reorderCommand
													);
	std::shared_ptr<Future> _f1 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numRows()
													);
	std::shared_ptr<Future> _f2 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numColumns()
													);
	
    std::shared_ptr<Future> _f3 = writeTransferInstructionAsync(
        _accRequireResultReady ? arch.INSTR_get_matrix_array_w_result_ready
                               : arch.INSTR_get_matrix_array_wo_result_ready);
    std::shared_ptr<Future> _f4 = writeTransferInstructionAsync(_accMemStart);
    std::shared_ptr<Future> _f5 = writeTransferInstructionAsync(num_rows);
    std::shared_ptr<Future> _f6 = writeTransferInstructionAsync(num_cols);
	
    std::shared_ptr<Future> _f7 = std::make_shared<MatrixViewReadFuture>(ctx, _matrixView);
    targets->process(_f7);

    return std::make_shared<AndFuture>(ctx, std::vector<std::shared_ptr<Future>>({_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7}));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::readMatrixControllerAsync(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    logWork.print(fmt::format(
        "Reading controller matrix of size {}x{} at address {}",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

    if (_accRequireResultReady) {
        logWork.print(" (waiting for result)\n");
    } else {
        logWork.print(" (not waiting for result)\n");
    }

    uint32_t num_rows = _matrixView->numRows();
	uint32_t num_cols = _matrixView->numColumns();
	
	if(_reorderCommand == arch.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE))
	{
		num_rows = _matrixView->numColumns();
		num_cols = _matrixView->numRows();
	}

	std::shared_ptr<Future> _f0 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_reorderCommand
													);
	std::shared_ptr<Future> _f1 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numRows()
													);
	std::shared_ptr<Future> _f2 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_OUTPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numColumns()
													);
	
    std::shared_ptr<Future> _f3 = writeTransferInstructionAsync(
        _accRequireResultReady ? arch.INSTR_get_ctrl_array_w_result_ready : arch.INSTR_get_ctrl_array_wo_result_ready);
    std::shared_ptr<Future> _f4 = writeTransferInstructionAsync(_accMemStart);
    std::shared_ptr<Future> _f5 = writeTransferInstructionAsync(num_rows);
    std::shared_ptr<Future> _f6 = writeTransferInstructionAsync(num_cols);
	
    std::shared_ptr<Future> _f7 = std::make_shared<MatrixViewReadFuture>(ctx, _matrixView);
    targets->process(_f7);

    return std::make_shared<AndFuture>(ctx, std::vector<std::shared_ptr<Future>>({_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7}));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future>
Driver::writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    logWork.print(fmt::format(
        "Writing array matrix of size {}x{} at address {}\n",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

	uint32_t num_rows = _matrixView->numRows();
	uint32_t num_cols = _matrixView->numColumns();
	
	if(_reorderCommand == arch.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE))
	{
		num_rows = _matrixView->numColumns();
		num_cols = _matrixView->numRows();
	}
	
	std::shared_ptr<Future> _f0 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_reorderCommand
													);
	std::shared_ptr<Future> _f1 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numRows()
													);
	std::shared_ptr<Future> _f2 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numColumns()
													);
	
    std::shared_ptr<Future> _f3 = writeTransferInstructionAsync(arch.INSTR_send_matrix_array);
    std::shared_ptr<Future> _f4 = writeTransferInstructionAsync(_accMemStart);
    std::shared_ptr<Future> _f5 = writeTransferInstructionAsync(num_rows);
    std::shared_ptr<Future> _f6 = writeTransferInstructionAsync(num_cols);
	
    std::shared_ptr<Future> _f7 = std::make_shared<MatrixViewWriteFuture>(ctx, _matrixView);
    targets->process(_f7);

    return std::make_shared<AndFuture>(ctx, std::vector<std::shared_ptr<Future>>({_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7}));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future>
Driver::writeMatrixControllerAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    logWork.print(fmt::format(
        "Writing controller matrix of size {}x{} at address {}\n",
        _matrixView->numRows(),
        _matrixView->numColumns(),
        _accMemStart));

	uint32_t num_rows = _matrixView->numRows();
	uint32_t num_cols = _matrixView->numColumns();
	
	if(_reorderCommand == arch.get(ArchConstant::REORDER_BUFFER_COMMAND_TRANSPOSE))
	{
		num_rows = _matrixView->numColumns();
		num_cols = _matrixView->numRows();
	}

	std::shared_ptr<Future> _f0 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_reorderCommand
													);
	std::shared_ptr<Future> _f1 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numRows()
													);
	std::shared_ptr<Future> _f2 = writeRegisterAsync( 	arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INPUT_DATA_REORDER_BUFFER_FIFO_IN_ADDR),
														_matrixView->numColumns()
													);
	
	std::shared_ptr<Future> _f3 = writeTransferInstructionAsync(arch.INSTR_send_ctrl_array);
	std::shared_ptr<Future> _f4 = writeTransferInstructionAsync(_accMemStart);
    std::shared_ptr<Future> _f5 = writeTransferInstructionAsync(num_rows);
    std::shared_ptr<Future> _f6 = writeTransferInstructionAsync(num_cols);
	
    std::shared_ptr<Future> _f7 = std::make_shared<MatrixViewWriteFuture>(ctx, _matrixView);
    targets->process(_f7);

    return std::make_shared<AndFuture>(ctx, std::vector<std::shared_ptr<Future>>({_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7}));
	
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::writeInstructionAsync(uint32_t _instruction) {
    return writeRegisterAsync(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR), _instruction);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::writeInstructionAsync(uint8_t _instructionByte, uint32_t _argument) {
    return writeInstructionAsync(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::writeTransferInstructionAsync(uint32_t _instruction) {
    return writeRegisterAsync(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_DTE_FIFO_IN_ADDR), _instruction);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Driver::writeCodeAsync(uint32_t _address, std::span<const uint32_t> _code) {
    std::vector<std::shared_ptr<Future>> _futures;

    _futures.push_back(writeInstructionAsync(arch.INSTRB_pload, _address));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    for (uint32_t _instruction : _code) {
        _futures.push_back(writeInstructionAsync(_instruction));
    }

    _futures.push_back(writeInstructionAsync(arch.INSTRB_prun, 0));
    _futures.push_back(writeInstructionAsync(arch.INSTR_nop));

    return std::make_shared<AndFuture>(ctx, std::move(_futures));
}

//-------------------------------------------------------------------------------------
void Driver::writeCode(uint32_t _address, std::span<const uint32_t> _code) {
    std::shared_ptr<Future> _future = writeCodeAsync(_address, _code);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::writeInstruction(uint32_t _instruction) {
    std::shared_ptr<Future> _future = writeInstructionAsync(_instruction);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::writeTransferInstruction(uint32_t _instruction) {
    std::shared_ptr<Future> _future = writeInstructionAsync(_instruction);
    _future->wait();
}

//-------------------------------------------------------------------------------------
void Driver::writeInstructions(std::span<const uint32_t> _instructions) {
    for (uint32_t _instruction : _instructions) {
        writeInstruction(_instruction);
    }
}

//-------------------------------------------------------------------------------------
void Driver::registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID) {
    assert(_breakpoint.conditions.size() == arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS));

    if (_breakpointID >= arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS)) {
        throw std::runtime_error("Breakpoint ID out of range");
    }

    unsigned _lastConditionID = arch.get(ArchConstant::DEBUG_BP_NR_CONDITIONS) - 1;

    for (unsigned _conditionID = 0; _conditionID <= _lastConditionID; _conditionID++) {
        logWork.print(fmt::format(
            "Writing condition {} for hw breakpoint {}: operation {} operand {} value {}\n",
            _conditionID,
            _breakpointID,
            _breakpoint.conditions.at(_conditionID).condition,
            _breakpoint.conditions.at(_conditionID).operand,
            _breakpoint.conditions.at(_conditionID).value));

        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_OPERATION_ADDR),
            _breakpoint.conditions.at(_conditionID).condition);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_SEL_ADDR),
            _breakpoint.conditions.at(_conditionID).operand);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_COMP_VAL_ADDR),
            _breakpoint.conditions.at(_conditionID).value);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_COND_INTERNAL_REG_MASK_ADDR), 0xFFFF'FFFF);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WANT_IN_DEPTH_DEBUG_ADDR), 0);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), _breakpointID);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_COND_ADDR), _conditionID);

        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 1);

        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);
    }

    breakpoints.at(_breakpointID) = std::make_unique<Breakpoint>(_breakpoint);
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoint(unsigned _breakpointID) {
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_ADDR_BP_ADDR), _breakpointID);

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_BP_ENABLE_ADDR), 0);

    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 1);
    writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_SAVE_REGISTERS_CMD_ADDR), 0);

    breakpoints.at(_breakpointID) = nullptr;
}

//-------------------------------------------------------------------------------------
void Driver::clearBreakpoints() {
    for (unsigned i = 0; i < arch.get(ArchConstant::DEBUG_NR_BREAKPOINTS); i++) {
        clearBreakpoint(i);
    }
}

//-------------------------------------------------------------------------------------
unsigned Driver::nextAvailableBreakpoint() {
    for (unsigned _i = 0; _i < breakpoints.size(); ++_i) {
        if (!breakpoints[_i]) {
            return _i;
        }
    }

    throw std::runtime_error("All breakpoints");
}

//-------------------------------------------------------------------------------------
void Driver::handleInterrupt() {
    logWork.print("Driver::handleInterrupt\n");

    if ((readRegister(arch.get(ArchConstant::IO_INTF_AXILITE_READ_REGS_INTERRUPT_STATUS_REG_ADDR))
         >> arch.get(ArchConstant::XPU_INTERRUPT_STATUS_REG_SOFTWARE_INT_LOC_LOWER))
        & 1) {
        logWork.print("Software interrupt\n");
        // writeRegister(
        //     arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
        //     1 << arch.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_SOFTWARE_INT_LOC_LOWER));
    } else if (
        (readRegister(arch.get(ArchConstant::IO_INTF_AXILITE_READ_REGS_INTERRUPT_STATUS_REG_ADDR))
         >> arch.get(ArchConstant::XPU_INTERRUPT_STATUS_REG_DEBUG_INT_LOC_LOWER))
        & 1) {
        logWork.print("Breakpoint interrupt\n");
        handleBreakpointHit();
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
            1 << arch.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_DEBUG_INT_LOC_LOWER));
    } else {
        logWork.print("Warning: Unknown interrupt\n");
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_INT_CLEAR_ADDR),
            1 << arch.get(ArchConstant::XPU_INT_CLEAR_REG_CLEAR_GLOBAL_INT_LOC_LOWER));
    }
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHit() {
    assert(accImage == nullptr);

    accImage = std::make_shared<AcceleratorImage>();

    handleBreakpointHitFillAcceleratorImage(*accImage);

    accImage->print();

    unsigned _breakpointID                   = handleBreakpointHitGetBreakpointID();
    std::unique_ptr<Breakpoint>& _breakpoint = breakpoints.at(_breakpointID);

    bool _continue = true;
    if (_breakpoint && _breakpoint->callback != nullptr) {
        _continue = _breakpoint->callback(accImage, _breakpointID);
    }

    if (_continue) {
        continueAfterBreakpoint();
    }
}

//-------------------------------------------------------------------------------------
void Driver::continueAfterBreakpoint() {
    assert(accImage != nullptr);

    handleBreakpointHitDumpAcceleratorImage(*accImage);

    accImage = nullptr;

    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR),
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_DONE));

    logWork.print(fmt::format("Breakpoint done\n"));
}

//-------------------------------------------------------------------------------------
unsigned Driver::handleBreakpointHitGetBreakpointID() {
    uint32_t _bpStatusReg = readRegister(arch.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_BPs_STATUS_ADDR));

    unsigned _breakpointID = 0;
    for (unsigned _bitLocation = arch.get(ArchConstant::DEBUG_CORE_STATUS_REG_BREAKPOINT_ENABLED_LOC_LOWER);
         _bitLocation <= arch.get(ArchConstant::DEBUG_CORE_STATUS_REG_BREAKPOINT_ENABLED_LOC_UPPER);
         _bitLocation++, _breakpointID++) {
        if ((_bpStatusReg >> _bitLocation) & 1) {
            return _breakpointID;
        }
    }

    throw std::runtime_error("breakpoint hit but no breeakpoint is set in status reg");
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHitFillAcceleratorImage(AcceleratorImage& _accImage) {
    const unsigned IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR =
        arch.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    logWork.print("Driver::handleBreakpointHitFillAcceleratorImage\n");

    // These must be done in order
    _accImage.pc = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    fmt::println("Recieved pc {}\n", _accImage.pc);
    _accImage.prevPc1        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.prevPc2        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.prevPc3        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.nextPc         = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.cc             = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.nextInstrCtrl  = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.nextInstrArray = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.ctrlFlags      = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.ctrlAcc        = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    _accImage.ctrlStack.resize(arch.get(ArchConstant::RESOURCE_CTRL_STACK_SIZE));
    for (uint32_t& _ctrlStackValue : _accImage.ctrlStack) {
        _ctrlStackValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.ctrlActiveLoopCounter = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    _accImage.ctrlLoopCounters.resize(arch.get(ArchConstant::CTRL_NR_LOOP_COUNTERS));
    for (uint32_t& _ctrlLoopConter : _accImage.ctrlLoopCounters) {
        _ctrlLoopConter = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.ctrlDecrReg          = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.reduceNetOut         = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.boolScanOr           = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    _accImage.ctrlAddrRegsSelector = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);

    _accImage.ctrlAddrRegs.resize(arch.get(ArchConstant::CONTROLLER_ADDR_REG_NR_LOCATIONS));
    for (uint32_t& _ctrlAddrReg : _accImage.ctrlAddrRegs) {
        _ctrlAddrReg = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.ctrlMem.resize(arch.get(ArchConstant::CONTROLLER_MEM_SIZE));
    for (uint32_t& _ctrlMemValue : _accImage.ctrlMem) {
        _ctrlMemValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.ctrlInstrMemCtrl.resize(arch.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    for (uint32_t& _ctrlInstrMemCtrlValue : _accImage.ctrlInstrMemCtrl) {
        _ctrlInstrMemCtrlValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.ctrlInstrMemArray.resize(arch.get(ArchConstant::CONTROLLER_INSTR_MEM_SIZE));
    for (uint32_t& _ctrlInstrMemArrayValue : _accImage.ctrlInstrMemArray) {
        _ctrlInstrMemArrayValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayActivationReg.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayActivationRegValue : _accImage.arrayActivationReg) {
        _arrayActivationRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayBool.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayBoolValue : _accImage.arrayBool) {
        _arrayBoolValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayAcc.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayAccValue : _accImage.arrayAcc) {
        _arrayAccValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayIORegData.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayIORegDataValue : _accImage.arrayIORegData) {
        _arrayIORegDataValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayAddrReg.resize(2);
    for (std::vector<uint32_t>& _arrayAddrRegLayer : _accImage.arrayAddrReg) {
        _arrayAddrRegLayer.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
        for (uint32_t& _arrayAddrRegValue : _arrayAddrRegLayer) {
            _arrayAddrRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
        }
    }

    _accImage.arrayGlobalShiftReg.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    for (uint32_t& _arrayGlobalShiftRegValue : _accImage.arrayGlobalShiftReg) {
        _arrayGlobalShiftRegValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
    }

    _accImage.arrayStack.resize(arch.get(ArchConstant::RESOURCE_ARRAY_CELL_STACK_SIZE));
    for (std::vector<uint32_t>& _arrayStackRow : _accImage.arrayStack) {
        _arrayStackRow.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));

        for (uint32_t& _arrayStackValue : _arrayStackRow) {
            _arrayStackValue = readRegister(IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR);
        }
    }

    _accImage.arrayMemValidRows = accImageArrayMemValidRows;
    handleBreakpointHitFillAcceleratorImageArrayMem(_accImage, accImageArrayMemValidRows);

    fmt::println("Memory is {}x{}", _accImage.arrayMem.size(), _accImage.arrayMem.at(0).size());

    _accImage.rereadArrayMem = [this, &_accImage]() {
        accImageArrayMemValidRows = _accImage.arrayMemValidRows;
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_READ_ADDR_START_ADDR),
            accImageArrayMemValidRows.first);

        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_READ_ADDR_STOP_ADDR),
            accImageArrayMemValidRows.second);
        writeRegister(
            arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR),
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_REREAD_ARRAY_MEM));

        handleBreakpointHitFillAcceleratorImageArrayMem(_accImage, accImageArrayMemValidRows);
    };
}

//-------------------------------------------------------------------------------------
void Driver::handleBreakpointHitFillAcceleratorImageArrayMem(
    AcceleratorImage& _accImage, std::pair<uint32_t, uint32_t> _accImageArrayMemValidRows) {
    logWork.print(fmt::format(
        "Reading array memory from row {} to row {}\n",
        _accImageArrayMemValidRows.first,
        _accImageArrayMemValidRows.second));

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

    _accImage.arrayMem.resize(arch.get(ArchConstant::ARRAY_CELL_MEM_SIZE));
    for (std::vector<uint32_t>& _arrayMemRow : _accImage.arrayMem) {
        _arrayMemRow.resize(arch.get(ArchConstant::ARRAY_NR_CELLS));
    }

    logWork.print(fmt::format("Reading array memory from \n"));

    uint32_t _totalRows = _accImageArrayMemValidRows.second - _accImageArrayMemValidRows.first + 1;
    for (uint32_t _row = _accImageArrayMemValidRows.first; _row <= _accImageArrayMemValidRows.second; _row++) {
        uint32_t _relativeRow = _row - _accImageArrayMemValidRows.first + 1;

        _bar.set_progress(_relativeRow * 100 / _totalRows);
        _bar.set_option(
            indicators::option::PostfixText{fmt::format("Reading array memory {}/{}", _relativeRow, _totalRows)});

        for (uint32_t& _arrayMemValue : _accImage.arrayMem.at(_row)) {
            _arrayMemValue = readRegister(arch.get(ArchConstant::IO_INTF_AXILITE_READ_DEBUG_DATA_OUT_ADDR));
            logWork.print(fmt::format("Value at row {}: {}\n", _row, _arrayMemValue));
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
void Driver::handleBreakpointHitDumpAcceleratorImage(const AcceleratorImage& _accImage) {
    const unsigned IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR =
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR);
    const unsigned IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR =
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR);
    const unsigned DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC =
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
    const unsigned DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC =
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);

    // WRITE ARRAY

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _accImage.prevPc2);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_PC));

    for (auto _ctrlStackIt = _accImage.ctrlStack.rbegin(); _ctrlStackIt != _accImage.ctrlStack.rend(); _ctrlStackIt++) {
        uint32_t _ctrlStackValue = *_ctrlStackIt;

        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlStackValue);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_STACK_PUSH));
    }

    uint32_t _ctrlAddrRegIndex = 0;
    for (uint32_t _ctrlAddrReg : _accImage.ctrlAddrRegs) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlAddrRegIndex++);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG_SELECTOR));
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlAddrReg);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _accImage.ctrlAddrRegsSelector);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_ADDR_REG_SELECTOR));

    uint32_t _ctrlLoopCounterIndex = 0;
    for (uint32_t _ctrlLoopCounter : _accImage.ctrlLoopCounters) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlLoopCounter);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlLoopCounterIndex++);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_LOOP_COUNTER));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _accImage.ctrlDecrReg);
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_DECREMENT_REG));

    uint32_t _ctrlMemAddress = 0;
    for (uint32_t _ctrlMemValue : _accImage.ctrlMem) {
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR), _ctrlMemAddress++);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _ctrlMemValue);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_CTRL_WRITE_ACC_X_MEMORY));
    }

    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _accImage.ctrlAcc);
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_CTRL_WRITE_DEBUG_X_ACC);

    // WRITE ARRAY
    std::for_each(_accImage.arrayStack.rbegin(), _accImage.arrayStack.rend(), [=, this](auto& _arrayStackLayer) {
        std::for_each(_arrayStackLayer.begin(), _arrayStackLayer.end(), [=, this](auto& _arrayStackValue) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayStackValue);
        });

        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_STACK_PUSH));
    });

    // TODO: reverse
    for (auto& _arrayAddrRegLayer : _accImage.arrayAddrReg) {
        for (uint32_t _arrayAddrRegValue : _arrayAddrRegLayer) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAddrRegValue);
        }

        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_ACC_X_ADDR_REG));

        // TODO: ask
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_ADDR_REG_STACK_DUPLICATE));
    }

    for (uint32_t _arrayAddrRegValue : _accImage.arrayGlobalShiftReg) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAddrRegValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WROTE_ACC_X_GLOBAL_SHIFT_REG));

    uint32_t _addr = 0;
    for (const std::vector<uint32_t>& _arrayMemRow : _accImage.arrayMem) {
        for (uint32_t _arrayMemValue : _arrayMemRow) {
            writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayMemValue);
        }
        writeRegister(arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_DEBUG_MEM_WRITE_ADDR_ADDR), _addr++);
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
        writeRegister(
            IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
            arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_ACC_X_MEMORY));
    }

    for (uint32_t _arrayActivationRegValue : _accImage.arrayActivationReg) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayActivationRegValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
    writeRegister(
        IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR,
        arch.get(ArchConstant::DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACTIVATION_REG));

    for (uint32_t _arrayAccValue : _accImage.arrayAcc) {
        writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_DATA_IN_ADDR, _arrayAccValue);
    }
    writeRegister(IO_INTF_AXILITE_WRITE_DEBUG_WRITE_MODE_CMD_ADDR, DEBUG_WRITE_MODE_CMD_ARRAY_WRITE_DEBUG_X_ACC);
}

//-------------------------------------------------------------------------------------
uint64_t Driver::getTime() const {
    return targets->getTime();
}

//-------------------------------------------------------------------------------------
